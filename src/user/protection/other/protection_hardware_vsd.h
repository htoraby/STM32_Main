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
  bool checkAlarm();
  bool checkPrevent();
};

#endif // PROTECTIONHARDWAREVSD_H
