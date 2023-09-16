/* Copyright (c) 2007 Altera Corporation, San Jose, California, USA. All rights 
 * reserved. All use of this software and documentation is subject to the License
 * Agreement located at the end of this file below.
 *******************************************************************************                                                                             *
 * Author - S. O'Reilly                                                        *
 * Date - July 6, 2007                                                         *
 * Module - timer_interrupt_latency.c                                                             *
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
 
#include <string.h>

/* Interrupt definitions.
 */
 
#include "sys/alt_irq.h"

/* Include HAL device accessor macros for the timer.
 */
 
#include "altera_avalon_timer_regs.h"

/* Include tcm software example specific header.
 */
 
#include "timer_interrupt_latency.h"

/* Define counter for calculating interrupt latency.
 */
 
volatile alt_u32 elapsed_ticks_since_interrupt;

/* 
 * timer_interrupt_latency_irq() is the interrupt handler used to calculate
 * interrupt response time in clock ticks by measuring the number of ticks 
 * elapsed between the interrupt and the entry to this interrupt handler. The 
 * function first reads the current snapshot of the timer register, 
 * calculates the elapsed ticks since the interrupt fired by comparing the 
 * current snapshot against the period value loaded upon interrupt trigger, 
 * then clears the interrupt condition.
 */

void timer_interrupt_latency_irq (void* base, alt_u32 id)
{
  
  /* Temporary holders of the high and low 16 bits of the timer snapshot
   * and period registers. 
   */
   
  alt_u32 snapshot_lower;
  alt_u32 snapshot_upper;
  alt_u32 period_lower;
  alt_u32 period_upper;
  
  snapshot_lower = 0;
  snapshot_upper = 0;
  period_lower = 0;
  period_upper = 0;
  
  /* Read the timer snapshot and store in a global variable to be analyzed 
   * outside the context of this interrupt handler. 
   */
   
  /* First, request a coherent snapshot of the entire 32 bit timer
   * snapshot value by writing to the lower snapshot register. 
   */
   
  IOWR_ALTERA_AVALON_TIMER_SNAPL (base, 0);
  
  /* Next, read the lower 16 bits of the timer snap shot value. 
   */
   
  snapshot_lower = IORD_ALTERA_AVALON_TIMER_SNAPL(base);
    
  /* Followed by reading the upper 16 bits of the timer snapshot value.
   */    
   
  snapshot_upper = IORD_ALTERA_AVALON_TIMER_SNAPH(base);
  
  /* Upon interrupt, the timer is reloaded with the original period values.
   * Subtract the snapshot from the period to determine the number of 
   * elapsed ticks since the interrupt fired and this interrupt service 
   * routine was invoked. 
   */
   
  period_lower = IORD_ALTERA_AVALON_TIMER_PERIODL(base);
  period_upper = IORD_ALTERA_AVALON_TIMER_PERIODH(base);    
         
  elapsed_ticks_since_interrupt = ( 
     ((period_upper << 16) | period_lower) -
     ((snapshot_upper << 16) | snapshot_lower));
     
  /* Finally, clear the interrupt. 
   */

  IOWR_ALTERA_AVALON_TIMER_STATUS (base, 0);
 
  /* Stop the Timer.
   */
  
  IOWR_ALTERA_AVALON_TIMER_CONTROL (base, 
             ALTERA_AVALON_TIMER_CONTROL_STOP_MSK);
 
}

/*
 * timer_interrupt_latency_init() initialises the timer that will be 
 * used to measure interrupt latency. 
 */

void timer_interrupt_latency_init (void* base, alt_u32 irq)
{
  
  /* Register the interrupt handler, and enable the interrupt.
   * Locate the irq code in the .exceptions section, which will be placed into
   * the tightly coupled instruction memory, per the SOPC Builder 
   * Nios II More "cpu" Setting - Processor Configuration: 
   * Processor Function = "Exception Address", 
   * Memory Module = "tightly_coupled_instruction_memory". 
   */
    
  alt_irq_register (irq, base, timer_interrupt_latency_irq);    
  
  /* Start the interrupt latency timer 
   * set to generate an interrupt and run continuously 
   */
  
  IOWR_ALTERA_AVALON_TIMER_CONTROL (base, 
            ALTERA_AVALON_TIMER_CONTROL_ITO_MSK  |
            ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
            ALTERA_AVALON_TIMER_CONTROL_START_MSK);
}

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
