#ifndef EM_H_
#define EM_H_

#include "device.h"

/*!
 * \brief Базовый класс счётчика электроэнергии
 * Наследник класса Device использующий его структуру и методы хранения банка параметров
 * имеющий свою карту параметров класс не имеющий объектов, а использующийся как базовый для
 * классов конкретных электросчётчиков
*/
class Em: public Device
{
public:
  Em();
  virtual ~Em();

  void initParameters();

  /*!
   * \brief Метод задания коэффициентов трансформации
   * \return
   */
  int setCoefficientTransformation();

  /*!
   * \brief Метод задания коэффициента трансформации тока
   * \return
   */
  int setCoefficientTransforamationCurrent();

  /*!
   * \brief Метод задания коэффициента трансформации напряжения
   * \return
   */
  int setCoefficientTransforamationVoltage();

private:
  //! Массив параметров устройства
  parameter parametersArray_[EM_END - EM_BEGIN];

};

#endif /* EM_H_ */
