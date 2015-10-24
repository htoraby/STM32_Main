#ifndef VSDNOVOMETLOG_H
#define VSDNOVOMETLOG_H

#include "vsd_log.h"

class VsdNovometLog : public VsdLog
{
public:
  VsdNovometLog();
  ~VsdNovometLog();

   bool checkAlarm();

   void readAlarmLog(uint16_t *ia, uint16_t *ib, uint16_t *ic,
                     uint16_t *ud);
   void readRunningLog();

};

#endif // VSDNOVOMETLOG_H
