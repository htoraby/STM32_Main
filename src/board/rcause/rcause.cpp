#include "rcause.h"
#include "fram.h"
#include <string.h>

static RCAUSE_COUNTS rcauseCounts;

void resetCauseCheck()
{
  framReadData(FRAM_SIZE-sizeof(RCAUSE_COUNTS), (uint8_t*)&rcauseCounts, sizeof(RCAUSE_COUNTS));

  if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST) == SET) {
    rcauseCounts.bor++;
  }
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) == SET) {
    rcauseCounts.sft++;
  }
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) == SET) {
    rcauseCounts.iwdg++;
  }
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) == SET) {
    rcauseCounts.wwdg++;
  }
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) == SET) {
    rcauseCounts.pin++;
  }

  if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) == SET) {
    rcauseCounts.por++;
  }
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST) == SET) {
    rcauseCounts.lpwr++;
  }

  __HAL_RCC_CLEAR_RESET_FLAGS();

  framWriteData(FRAM_SIZE-sizeof(RCAUSE_COUNTS), (uint8_t*)&rcauseCounts, sizeof(RCAUSE_COUNTS));
}

RCAUSE_COUNTS rcauseCountersGet()
{
  return rcauseCounts;
}

void rcauseCountersClear()
{
  memset(&rcauseCounts, 0, sizeof(RCAUSE_COUNTS));
}
