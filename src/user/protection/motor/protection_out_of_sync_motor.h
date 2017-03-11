#ifndef PROTECTIONOUTOFSYNCMOTOR_H
#define PROTECTIONOUTOFSYNCMOTOR_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Рассинхронизация ПВЭД"
 */
class ProtectionOutOfSyncMotor : public Protection
{
public:
  ProtectionOutOfSyncMotor();
  ~ProtectionOutOfSyncMotor();

private:
  bool checkAlarm();
  bool isProtect();
  void addEventReactionProt();

  float valueParameter2_;

};

#endif // PROTECTIONOUTOFSYNCMOTOR_H
