#ifndef PROTECTIONFLOWDISCHARGE_H
#define PROTECTIONFLOWDISCHARGE_H

#include "protection.h"

/*!
 * \brief Класс защиты "Минимальный расход на выкиде"
 */
class ProtectionFlowDischarge : public Protection
{
public:
  ProtectionFlowDischarge();
  ~ProtectionFlowDischarge();

private:
  bool checkAlarm();
  bool checkPrevent();
  float calcValue();

  bool isProtect();

};

#endif // PROTECTIONFLOWDISCHARGE_H
