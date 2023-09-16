/* Copyright (c) 2007 Altera Corporation, San Jose, California, USA. All rights 
 * reserved. All use of this software and documentation is subject to the License
 * Agreement located at the end of this file below.
 *******************************************************************************
 * Author - S. O'Reilly                                                        *
 * Date - July 6, 2007                                                         *
 * Module - tcm.c                                                              *
 *******************************************************************************
 *
 * TCM (Tightly Coupled Memories) example. 
 *
 * This example requires a performance_counter, as found in full_featured 
 * hardware designs. Also, the system clock timer must be set to sys_clk_timer.
 * 
 * This program is an example which demonstrates measurement of interrupt 
 * latency and memory access speed in a Nios II system.  Tightly Coupled
 * Memories in particular are highlighted as the fastest accessible memory in
 * a Nios II system, perfect for storing things such as exception handlers and 
 * the exception stack.
 * See the Hello Freestanding software example and 
 * associated readme.txt for additional details on alt_main() and low-level
 * system initialization step descriptions.
 *
 * Please refer to this TCM software example's file readme.txt for notes on 
 * this software example, including notes on system library settings for 
 * locating the exception stack separately from other program data, the
 * fastest location is the tightly coupled data memory, configured as a 
 * dual-port, Avalon channel, onchip memory in the full-featured Nios II hardware
 * design.
 */
 
/* Common C Includes.
 */
 
#include <stdio.h>
#include <stdlib.h>

/* Description of the SOPC Builder configured Nios II system. 
 */
 
#include "system.h"
 
/* Performance_counter defined in full_featured design (Required).
 */
 
#ifndef PERFORMANCE_COUNTER_NAME
#error This example requires a performance counter in the design (such as full_featured).
#else

/* Definition for the Performance Counter Peripheral.
 */
 
#include "altera_avalon_performance_counter.h"

/* Low level HAL system initialization and cache includes.
 */
 
#include "sys/alt_sys_init.h"
#include "sys/alt_irq.h"
#include "sys/alt_cache.h"
#include "priv/alt_file.h"

/* Include tcm software example specific include.
 */
 
#include "timer_interrupt_latency.h"

/* Define the size of each memory block which will be used for
 * measuring the performance of each of the tightly coupled memory (M4K), 
 * on-chip memory (M-RAM), and off-chip memory S-DRAM. 
 * 
 * The value of the CHECKSUM_MEMORY_BLOCK_SIZE can be varied to observe the
 * effects of varied blocksizes on data cache hits and misses and how that 
 * compares to tightly coupled memories.  Take care that the size of the block 
 * is not defined to be so large that it overlaps the exception stack defined
 * at the top of the tightly coupled data memory.
 * 
 * Note: The chance of a data cache miss could be greatly increased
 * with the selection of particular block sizes which, when combined with
 * particular data cache line sizes and tag bits per line, could cause
 * cache tag bit values resulting in a worst case situation.
 */
 
#define CHECKSUM_MEMORY_BLOCK_SIZE 320

/*
 * The checksum loop is performed multiple times in order to heat up the 
 * instruction cache.  This prevents any instruction cache misses from impacting 
 * measurement results we are interested in, which is strictly the data memory
 * access times.
 * 
 * Note:  If this number is increased to a large value, it is possible that 
 * the impact of a timer interrupt could skew a single calculation.
 */

#define CHECKSUM_LOOP_MEASURED_ITERATION 3

/* MEMORY_ACCESS_SPEED_TEST_SECTION Performance Counter Section # 1.
 */
 
#define MEMORY_ACCESS_SPEED_TEST_SECTION 1

/* CACHE_MEMORY_ACCESS_SPEED_TEST_SECTION Performance Counter Section # 2.
 */
 
#define CACHE_MEMORY_ACCESS_SPEED_TEST_SECTION 2

/* Only Stratix, not Cyclone, have ONCHIP 64K memory defined in full_featured
 * design.
 */
 
#ifdef ONCHIP_RAM_64_KBYTES_NAME
#define CHECK_ONCHIP_64K
#endif

/*
 * Function prototypes
 */
void fill_memory_block (alt_u32 *memory_block_base_ptr);
alt_u32 calculate_checksum (alt_u32 *memory_block_base_ptr);
void measure_checksum(alt_u32 *memory_block_base_ptr,
                      alt_u32 *checksum_value, 
                      alt_u32 *checksum_time,
                      char clear_data_cache);

/*
 * Fill 3 memory blocks with values for later checksum.
 */
 
void fill_memory_block (alt_u32 *memory_block_base_ptr)
{
  alt_u32 fill_value;
  
  for (fill_value = 0; fill_value < CHECKSUM_MEMORY_BLOCK_SIZE; fill_value++) 
  {  
    *memory_block_base_ptr = (alt_u32)(fill_value % 0xFF);
     memory_block_base_ptr += 1;
  }
}

  /* Time how long the checksum calculation takes for a data block.
   * The checksum calculation loops have been "unrolled" to maximize 
   * parallelization during data read latency periods.  Memory reads are
   * done 4 at a time.  Then a sum is performed. This ordering of 4 reads 
   * followed by 4 summations streamlines data reads on the Avalon bus 
   * (or Avalon channel in the case of tightly coupled data memory).
   * The first data element will have some amount of read latency associated
   * with the data access, so the instructions are issued to start reading
   * the other 3 data elements, since the summation instruction cannot operate
   * on the first data element until the data value has been retrieved from
   * memory.  The other data element values are retreived from memory in 
   * parallel during the summation of the first data element values. 
   */
   
alt_u32 calculate_checksum (alt_u32 *memory_block_base_ptr)
{
  register alt_u32 checksum_calculation_bucket_A;
  register alt_u32 checksum_calculation_bucket_B;
  register alt_u32 checksum_calculation_bucket_C;
  register alt_u32 checksum_calculation_bucket_D;
  
  alt_u32 *memory_block_index_ptr;
  alt_u32 checksum;

  /* Initialize the checksum and calculation buckets variables to zero.
   */ 
 
  checksum = 0;

  checksum_calculation_bucket_A = 0;
  checksum_calculation_bucket_B = 0;
  checksum_calculation_bucket_C = 0;
  checksum_calculation_bucket_D = 0;
  
  /* Start the memory block index at the memory block base.
   */
   
  memory_block_index_ptr = memory_block_base_ptr;
  
  /* Calculate the checksum.  Load values from possibly external RAM locations in
   * parallel via 4 separate checksum summation buckets (unrolled loop optimization). 
   */ 
   
  for (memory_block_index_ptr = memory_block_base_ptr;
       memory_block_index_ptr < (memory_block_base_ptr+CHECKSUM_MEMORY_BLOCK_SIZE);
       memory_block_index_ptr += 4)
  {
    checksum_calculation_bucket_A += *memory_block_index_ptr;
    checksum_calculation_bucket_B += *(memory_block_index_ptr + 1);
    checksum_calculation_bucket_C += *(memory_block_index_ptr + 2);
    checksum_calculation_bucket_D += *(memory_block_index_ptr + 3);
  }

  checksum += checksum_calculation_bucket_A;
  checksum += checksum_calculation_bucket_B;
  checksum += checksum_calculation_bucket_C;
  checksum += checksum_calculation_bucket_D; 

  return checksum;

}
  
/* Time how long the checksum 
 * calculation takes for each type of data block, thereby measuring
 * the performance of each of the data memory blocks. 
 */
 
void measure_checksum(alt_u32 *memory_block_base_ptr,
                      alt_u32 *checksum_value, 
                      alt_u32 *checksum_time,
                      char clear_data_cache)
{
  
  alt_u32 checksum_loop_iterator;
  
  /* Reset (initialize to zero) all section counters and the global 
   * counter of the "performance_counter" peripheral. 
   */
   
  PERF_RESET (PERFORMANCE_COUNTER_BASE);
   
  /* Start the performance counter peripheral's global counter, thereby
   * enabling all of the section counters of performance_counter. 
   */
   
  PERF_START_MEASURING (PERFORMANCE_COUNTER_BASE);
   
  /* Flush the Data Cache.
   */
   
  alt_dcache_flush_all();
  
  for (checksum_loop_iterator = 1; 
       checksum_loop_iterator <= CHECKSUM_LOOP_MEASURED_ITERATION; 
       checksum_loop_iterator ++) 
  {
        
    /* Determine the performance for read from memory.
     */
    if (checksum_loop_iterator == CHECKSUM_LOOP_MEASURED_ITERATION) 
    {

      /* Flush the Data Cache.
       */
      if (clear_data_cache) 
      {
        alt_dcache_flush_all();
      }

      PERF_BEGIN (PERFORMANCE_COUNTER_BASE, 
                  MEMORY_ACCESS_SPEED_TEST_SECTION);
    }
            
    *checksum_value = calculate_checksum(memory_block_base_ptr);
  
    if (checksum_loop_iterator == CHECKSUM_LOOP_MEASURED_ITERATION) 
    {
      PERF_END (PERFORMANCE_COUNTER_BASE, MEMORY_ACCESS_SPEED_TEST_SECTION);
    }
  }
  
  /* Stop all performance_counter activity via its global_counter. 
   */
   
  PERF_STOP_MEASURING (PERFORMANCE_COUNTER_BASE);
  
  /* Store performance counter checksum calculation measurement
   * results. 
   */

  *checksum_time = perf_get_section_time( 
     (void *)PERFORMANCE_COUNTER_BASE, /* Peripheral's HW base address   */         
      MEMORY_ACCESS_SPEED_TEST_SECTION);  /* Section number          */
}

/*
 * The following statement defines "main()" so that when the Nios II IDE 
 * debugger is set to break at "main()", it will break at the appropriate
 * place in this program, which does not contain a function called "main()".
 * Note that the Nios II IDE debugger can also be set to break at "alt_main()",
 * in which case the following statement would be unneccessary since 
 * "alt_main()" is defined in this program.
 */
 
int main (void) __attribute__ ((weak, alias ("alt_main")));

int alt_main(void)
{  
  
  /* Declare an array to use as the off-chip sdram memory block.
   */
  alt_u32 sdram_memory_block[CHECKSUM_MEMORY_BLOCK_SIZE];
  
#ifdef CHECK_ONCHIP_64K
  alt_u32 *onchip_memory_ptr;
  alt_u32 onchip_checksum;
  alt_u32 onchip_cache_checksum;
  alt_u32 onchip_time;
  alt_u32 onchip_cache_time;
#endif

  alt_u32 *data_tcm_memory_ptr;  
  alt_u32 data_tcm_checksum;
  alt_u32 data_tcm_time;
  
  alt_u32 *sdram_memory_ptr;
  alt_u32 sdram_checksum;
  alt_u32 sdram_cache_checksum;
  alt_u32 sdram_time;
  alt_u32 sdram_cache_time;

  /* 
   * Enable Interrupts
   *
   * Turn on interrupts, and initialize the low-level interrupt handler:
   */
   
  alt_irq_init (ALT_IRQ_BASE);
  
  /* 
   * Device-Driver Initialization
   *
   * Initialize all the device drivers for every piece of hardware
   * in the current system via AUTOMATICALLY-GENERATED file alt_sys_init.c.  
   * Please refer to the "readme.html" and "hello_alt_main.c" source code files 
   * in the Hello Freestanding example for more details. 
   */
   
  alt_sys_init();

  /* Validate that System clock timer has been set to sys_clk_timer.  A timer is required
   * to calculate the interrupt latency.  The high res timer cannot be used to time the 
   * interrupt latency, since the high res timer is used to generate the interrupt for 
   * which we will measure the interrupt latency.
   */

  CHECK_ALT_SYS_CLK(PERIPHERAL_SUBSYSTEM_SYS_CLK_TIMER, PERIPHERAL_SUBSYSTEM_SYS_CLK_TIMER);
  CHECK_ALT_SYS_CLK_TIME_STAMP(PERIPHERAL_SUBSYSTEM_SYS_CLK_TIMER, PERIPHERAL_SUBSYSTEM_SYS_CLK_TIMER);
  CHECK_ALT_HIGH_RES(PERIPHERAL_SUBSYSTEM_HIGH_RES_TIMER, PERIPHERAL_SUBSYSTEM_HIGH_RES_TIMER); /*new line*/
  /* Set elapsed_ticks_since_interrupt to a unique value to be watched in a 
   * busy loop until the value is changed by the interrupt latency timer 
   * interrupt handler. 
   */
    
  elapsed_ticks_since_interrupt = 0xFEEDFACE;
   
  /* Initialize the high_res_timer to measure interrupt latency. 
   */

  timer_interrupt_latency_init ((void *)PERIPHERAL_SUBSYSTEM_HIGH_RES_TIMER_BASE, 
                                 PERIPHERAL_SUBSYSTEM_HIGH_RES_TIMER_IRQ);
   
  /* Wait for the interrupt latency to be measured.
   */

  while (elapsed_ticks_since_interrupt == 0xFEEDFACE) {
    
    /* Do nothing, except wait for the value of 
     * elapsed_ticks_since_interrupt to be changed
     * by the interrupt latency timer isr. */
  }
  
  /* NOTE: The exception stack, for maximum speed, should be located 
   * via the system library properties page to reside within 
   * tightly coupled data memory. The default maximum exception stack size of 
   * 0x400 is chosen when that selection is made.  The exception stack will then
   * grow downward from the top of tightly coupled data memory.  
   * Care must be taken when using other memory in the same tightly coupled data 
   * memory not to grow into the exception stack residing in memory at the top 
   * of the tightly coupled data memory.  In this application note, there are a few  
   * kilobytes separating the exception stack and the default sized memory block 
   * pointed to by data_tcm_memory_ptr.
   */
   
  /* Initialize 3 pointers to the start addresses of each of the 3
   * memory blocks for which access speed will be measured, and fill the 
   * memory blocks with a series oh incrementing values.
   */
   
#ifdef CHECK_ONCHIP_64K  
  onchip_memory_ptr = (alt_u32 *)ONCHIP_RAM_64_KBYTES_BASE;
  fill_memory_block (onchip_memory_ptr);
#endif  
  data_tcm_memory_ptr = (alt_u32 *)(TIGHTLY_COUPLED_DATA_MEMORY_BASE);
  fill_memory_block (data_tcm_memory_ptr);
  sdram_memory_ptr = (alt_u32 *)sdram_memory_block;
  fill_memory_block (sdram_memory_ptr);

  /* Begin perfermance measurements.
   */
   
  /* Time how long the checksum 
   * calculation takes for each type of data block, thereby measuring
   * the performance of each of the data TCM (tightly coupled memory) (M4K), 
   * on-chip memory (M-RAM), and off-chip memory SDRAM. 
   */

#ifdef CHECK_ONCHIP_64K    
  
  /* Measure Onchip Memory Block.
   */
   
  measure_checksum(onchip_memory_ptr, 
                   &onchip_checksum, 
                   &onchip_time, 1);
                 
  measure_checksum(onchip_memory_ptr, 
                   &onchip_cache_checksum, 
                   &onchip_cache_time, 0);
#endif

  /* Measure Tightly Coupled Data Memory Block.
   */
   
  measure_checksum(data_tcm_memory_ptr, 
                   &data_tcm_checksum, 
                   &data_tcm_time, 0);

  /* Measure SDRAM Memory Block.
   */
   
  measure_checksum(sdram_memory_ptr, 
                   &sdram_checksum, 
                   &sdram_time, 1);

  measure_checksum(sdram_memory_ptr, 
                   &sdram_cache_checksum, 
                   &sdram_cache_time, 0);

  /* Performance measurements complete. 
   */
    
  /* No devices may be used (including printf to STDIO) which may generate 
   * interrupts before this point, except for the high_res_timer (used to 
   * perform the interrupt latency calculation) and a 
   * 10 millisecond system clock timer (default for the full-featured
   * hardware design), or the interrupt latency calculation may 
   * be affected.  The high_res_timer must NOT be configured as the
   * Timestamp timer on the system properties page, since it is used to 
   * do the interrupt latency calculation instead. 
   */
   
  /* 
   * I/O Stream Initialization.
   * 
   * Initialize the STDOUT stream, and associate it with the 
   * System Library's designated STDOUT device.  Note that the symbols
   * ALT_STDOUT (etc) are defined in the (generated) file system.h. 
   *
   */
   
  alt_io_redirect (ALT_STDOUT, ALT_STDIN, ALT_STDERR);
  
  printf("Tightly Coupled Memory vs. Cache Example Real-Time Measurements:\n\n");
 
  
  /* Identify the board type the TCM example is executed on for
   * record keeping purposes.  This software example is executed for each
   * Nios II release (and interim release) and compared against previous 
   * releases to watch out for any increases in interrupt latency overhead.
   */
   
  printf("%s (%s). \n", ALT_DEVICE_FAMILY, ALT_SYSTEM_NAME);

/* Print the interrupt response time measurement in clock cycles calculated in 
 * the high resolution timer ISR. The interrupt response time is commonly 
 * referred to as interrupt latency time; therefore, the ISR has been named 
 * timer_interrupt_latency_irq().
 */
 
 printf("Interrupt response time:        %7d clock cycles.\n\n", 
          (unsigned int)elapsed_ticks_since_interrupt);
     
  /* Verify that all 5 (or 3, in the case of Cyclone) checksums match each 
   * other. 
   */
  
#ifdef CHECK_ONCHIP_64K
  printf("Checksum Times:                        %smatch.\n", 
    (sdram_checksum == sdram_cache_checksum) ?
      ( (onchip_checksum == onchip_cache_checksum) ?
        ( (onchip_checksum == data_tcm_checksum)  ?
          ( (data_tcm_checksum == sdram_checksum) ?
              "All 5 memories "
            : "data_tcm vs. sdram mis-"
          )
          : "on-chip vs. data_tcm mis-"
        )
        : "on-chip vs. onchip_cache mis-"
      )
    : "sdram vs. sdram_cache mis-"
  );
#else 
  printf("Checksum Times:                          %smatch.\n", 
    (sdram_checksum == sdram_cache_checksum) ?
          ( (data_tcm_checksum == sdram_checksum) ?
              "All 3 memories "
            : "data_tcm vs. sdram mis-"
          )
    : "sdram vs. sdram_cache mis-"
  );
#endif

  printf("Tightly coupled memory:    %6.2f microseconds, %8lld clocks.\n",
         (((double) data_tcm_time) / ALT_CPU_FREQ * 1000000), 
         (unsigned long long)data_tcm_time);

#ifdef CHECK_ONCHIP_64K          
  printf("On-chip memory:            %6.2f microseconds, %8lld clocks.\n",
         (((double) onchip_time) / ALT_CPU_FREQ * 1000000), 
         (unsigned long long)onchip_time);
         
  printf("On-chip memory (cached):   %6.2f microseconds, %8lld clocks.\n",
         (((double) onchip_cache_time) / ALT_CPU_FREQ * 1000000), 
         (unsigned long long)onchip_cache_time);
#endif
         
  printf("SDRAM memory:              %6.2f microseconds, %8lld clocks.\n",
         (((double) sdram_time) / ALT_CPU_FREQ * 1000000), 
         (unsigned long long)sdram_time);
         
  printf("SDRAM memory (cached):     %6.2f microseconds, %8lld clocks.\n\n",
         (((double) sdram_cache_time) / ALT_CPU_FREQ * 1000000), 
         (unsigned long long)sdram_cache_time);
                
  printf("Checksum loop measured iteration:  %d.\n", 
         CHECKSUM_LOOP_MEASURED_ITERATION);
                
  printf("Checksum value:              %10ld total.\n", 
         (unsigned long)data_tcm_checksum);
  printf("Checksum block size:         %10ld words (32 bits).\n", 
         (unsigned long)CHECKSUM_MEMORY_BLOCK_SIZE);
         
  printf("CPU Frequency:               %10.4f Mhz.\n", 
         (double)ALT_CPU_FREQ / 1000000);
          
  /* 
   * Exit gracefully.
   *
   * Many embedded programs run as long as the machine is powered-up.
   * Those programs don't need to call exit().  This example terminates, so
   * needs to call exit().  The exit() function, amongst many other
   * things, flushes the I/O buffers--a singularly-important service
   * for this example:
   */
   
  exit(0);  // Return code for success
}
#endif // PERFORMANCE_COUNTER_NAME

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2007 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *                                                                 *
******************************************************************************/
