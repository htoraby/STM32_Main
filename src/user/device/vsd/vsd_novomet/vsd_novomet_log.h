#ifndef VSDNOVOMETLOG_H
#define VSDNOVOMETLOG_H

#include "vsd_log.h"

class VsdNovometLog : public VsdLog
{
public:
  VsdNovometLog();
  ~VsdNovometLog();

   bool checkAlarm();

   void readAlarmLog();
   void readRunningLog();

};

#endif // VSDNOVOMETLOG_H
