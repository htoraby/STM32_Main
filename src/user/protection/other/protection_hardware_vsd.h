#ifndef PROTECTIONHARDWAREVSD_H
#define PROTECTIONHARDWAREVSD_H

#include "protection.h"

/*!
 * \brief Класс аппаратных защит ЧРП
 */
class ProtectionHardwareVsd : public Protection
{
public:
  ProtectionHardwareVsd();
  ~ProtectionHardwareVsd();
private:
  void processing();
  void getOtherSetpointProt();
  bool checkAlarm();
  bool checkPrevent();
  bool checkWarning();
  void addEventReactionProt();

  void processingStateRun();

  bool warning_;
};

#endif // PROTECTIONHARDWAREVSD_H
