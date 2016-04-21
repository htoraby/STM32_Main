#ifndef PROTECTIONOVERNUMBEROFSTART_H
#define PROTECTIONOVERNUMBEROFSTART_H

#include "protection.h"

/*!
 * \brief Класс защиты "Превышение числа пусков"
 */
class ProtectionOvernumberOfStart : public Protection
{
public:
  ProtectionOvernumberOfStart();
  ~ProtectionOvernumberOfStart();

private:
  void getOtherSetpointProt();
  void setOtherParamProt();
  bool checkAlarm();
  void addEventReactionProt();
  void processingStateRun();
  void proccessingStateStop();
  void automatProtection();

};

#endif // PROTECTIONOVERNUMBEROFSTART_H
