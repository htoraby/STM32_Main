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
  void getOtherSetpointProt();
  bool checkAlarm();
  bool checkPrevent();
  void addEventReactionProt();
};

#endif // PROTECTIONHARDWAREVSD_H
