#include "user_debug.h"
#include "user_main.h"

void getRegistersFromStack(uint32_t *pulFaultStackAddress)
{
  volatile uint32_t r0;
  volatile uint32_t r1;
  volatile uint32_t r2;
  volatile uint32_t r3;
  volatile uint32_t r12;
  volatile uint32_t lr; /* Link register. */
  volatile uint32_t pc; /* Program counter. */
  volatile uint32_t psr;/* Program status register. */

  r0 = pulFaultStackAddress[ 0 ];
  r1 = pulFaultStackAddress[ 1 ];
  r2 = pulFaultStackAddress[ 2 ];
  r3 = pulFaultStackAddress[ 3 ];

  r12 = pulFaultStackAddress[ 4 ];
  lr = pulFaultStackAddress[ 5 ];
  pc = pulFaultStackAddress[ 6 ];
  psr = pulFaultStackAddress[ 7 ];


#ifdef USE_RTT
  SEGGER_RTT_printf(0, "HardFault: r0 = %x, r1 = %x, r2 = %x, r3 = %x, r12 = %x, lr = %x, pc = %x, psr = %x\n",
                    r0, r1, r2, r3, r12, lr, pc, psr);
#endif

  /* When the following line is hit, the variables contain the register values. */
  while (1) { }
}
