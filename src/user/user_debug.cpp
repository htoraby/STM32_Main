#include "user_debug.h"
#include "user_main.h"
#include "rcause.h"

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
  SEGGER_RTT_printf(0, "HardFault (RT): r0 = %x, r1 = %x, r2 = %x, r3 = %x, r12 = %x, lr = %x, pc = %x, psr = %x\n",
                    r0, r1, r2, r3, r12, lr, pc, psr);
#endif
  backupSaveParameter(RTC_BKP_DR2, r0);
  backupSaveParameter(RTC_BKP_DR3, r1);
  backupSaveParameter(RTC_BKP_DR4, r2);
  backupSaveParameter(RTC_BKP_DR5, r3);
  backupSaveParameter(RTC_BKP_DR6, lr);
  backupSaveParameter(RTC_BKP_DR7, pc);

  /* When the following line is hit, the variables contain the register values. */
  while (1) { }
}

void checkRegistersFromStack()
{
  volatile uint32_t r0 = backupRestoreParameter(RTC_BKP_DR2);
  volatile uint32_t r1 = backupRestoreParameter(RTC_BKP_DR3);
  volatile uint32_t r2 = backupRestoreParameter(RTC_BKP_DR4);
  volatile uint32_t r3 = backupRestoreParameter(RTC_BKP_DR5);
  volatile uint32_t lr = backupRestoreParameter(RTC_BKP_DR6);
  volatile uint32_t pc = backupRestoreParameter(RTC_BKP_DR7);

  if (pc != 0) {
    logDebug.add(FatalMsg, "HardFault: r0 = %x, r1 = %x, r2 = %x, r3 = %x, lr = %x, pc = %x",
                 r0, r1, r2, r3, lr, pc);

    backupSaveParameter(RTC_BKP_DR2, 0);
    backupSaveParameter(RTC_BKP_DR3, 0);
    backupSaveParameter(RTC_BKP_DR4, 0);
    backupSaveParameter(RTC_BKP_DR5, 0);
    backupSaveParameter(RTC_BKP_DR6, 0);
    backupSaveParameter(RTC_BKP_DR7, 0);
  }
}

void checkRcauseCounters()
{
  RCAUSE_COUNTS counts = rcauseCountersGet();
  RCAUSE_COUNTS countsOld = rcauseCountersGetOld();
  if (counts.iwdg != countsOld.iwdg)
    logDebug.add(FatalMsg, "Watchdog reset (%d)", counts.iwdg);
}
