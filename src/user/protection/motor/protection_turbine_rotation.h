#ifndef PROTECTIONTURBINEROTATION_H
#define PROTECTIONTURBINEROTATION_H

#include "protection.h"

/*!
 * \brief Класс автомата защиты "Турбинное вращение"
 */
class ProtectionTurbineRotation : public Protection
{
public:
  ProtectionTurbineRotation();
  ~ProtectionTurbineRotation();

private:
  bool checkPrevent();
  void automatProtection();

};

#endif // PROTECTIONTURBINEROTATION_H
