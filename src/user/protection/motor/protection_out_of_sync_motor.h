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

  /*!
   * \brief Инициализация автомата защиты
   */
  void init();

private:

  bool checkAlarm();
  bool checkBlock();
  void addEventReactionProt();

  float valueParameter2_;

};

#endif // PROTECTIONOUTOFSYNCMOTOR_H
