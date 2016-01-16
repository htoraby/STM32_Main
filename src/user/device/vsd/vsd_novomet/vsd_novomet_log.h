#ifndef VSDNOVOMETLOG_H
#define VSDNOVOMETLOG_H

#include "vsd_log.h"

class VsdNovometLog : public VsdLog
{
public:
  VsdNovometLog();
  ~VsdNovometLog();

  bool checkAlarm();
  bool checkReady();

  void setAlarm();

  void resetReady();
  void resetAlarm();

  void readAlarmLog(int16_t *ia, int16_t *ib, int16_t *ic,
                     int16_t *ud);
  void readRunningLog(int16_t *ia, int16_t *ib, int16_t *ic, int16_t *ud, int16_t *cos);

  osSemaphoreId semaphoreAnswer_;
};

#endif // VSDNOVOMETLOG_H
