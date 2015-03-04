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

  /*!
   * \brief Инициализация автомата защиты
   */
  void init();

private:
  bool checkBlock();
  void automatProtection();

  bool isStartProtect_;

};

#endif // PROTECTIONTURBINEROTATION_H
