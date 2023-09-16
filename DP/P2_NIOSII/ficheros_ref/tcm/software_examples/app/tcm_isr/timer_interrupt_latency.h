/* Copyright (c) 2007 Altera Corporation, San Jose, California, USA. All rights 
 * reserved. All use of this software and documentation is subject to the License
 * Agreement located at the end of this file below.
 *******************************************************************************                                                                             *
 * Author - S. O'Reilly                                                        *
 * Date - July 6, 2007                                                         *
 * Module - timer_interrupt_latency.h                                                               *
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
 
#ifndef __TIMER_INTERRUPT_LATENCY_H__
#define __TIMER_INTERRUPT_LATENCY_H__

#include "alt_types.h"
#include <io.h>

/* Include altera_avalon_timer.h for ALT_SYS_CLK_BASE macro construction. 
 */
 
#include "altera_avalon_timer.h"
 
/* External reference to counter for calculating interrupt latency.
 */
 
extern volatile alt_u32 elapsed_ticks_since_interrupt;

/* Declare the function prototype for the high_res_timer
 * interrupt handler, and locate it in the .exceptions 
 * section along with the main interrupt handling funnel.
 * This .exceptions section will be located in high speed
 * memory, the tightly coupled instruction memory.
 */
 
extern void timer_interrupt_latency_irq (void* base, alt_u32 id)
 __attribute__ ((section (".isrs"))); /*changed from exceptions to isrs*/

/*
 * timer_interrupt_latency_init() initializes the timer that will be 
 * used to measure interrupt response time. 
 */

void timer_interrupt_latency_init (void* base, alt_u32 irq);

/* Validate that System clock timer has been set to sys_clk_timer.  A timer is required
 * to calculate the interrupt latency.  The high res timer cannot be used to time the 
 * interrupt latency, since the high res timer is used to generate the interrupt for 
 * which we will measure the interrupt latency.
 */
 
#define CHECK_ALT_SYS_CLK(name, dev)                                             \
  if (name##_BASE != ALT_SYS_CLK_BASE)                                           \
  {                                                                              \
    ALT_LINK_ERROR("Error: System clock timer must be configured as sys_clk_timer.\n" \
                   "__alt_invalid will be used to invoke linker error."); \
  }                                                                         

#define CHECK_ALT_SYS_CLK_TIME_STAMP(name, dev)									 \
  if (name##_BASE == ALT_TIMESTAMP_CLK_BASE)                                     \
  {                                                                              \
    ALT_LINK_ERROR("Error: System clock and high res timer cannot be defined as timestamp timer. Please use a different timer.\n" \
                   "__alt_invalid will be used to invoke linker error."); \
  }                           
//#endif /* __TIMER_INTERRUPT_LATENCY__ */

/*added function*/
#define CHECK_ALT_HIGH_RES(name, dev)                                             \
  if (name##_BASE == ALT_TIMESTAMP_CLK_BASE)                                           \
  {                                                                              \
    ALT_LINK_ERROR("Error: System clock and high res timer cannot be defined as timestamp timer. Please use a different timer.\n" \
                   "__alt_invalid will be used to invoke linker error."); \
  }                                                                         

#endif /* __TIMER_INTERRUPT_LATENCY__ */