/*
 * vsd.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef VSD_H_
#define VSD_H_

#include "device.h"

/*!
 * \brief Базовый класс ЧРП
 * Наследник класса Device использующий его структуру и методы хранения банка
 * параметров имеющий свою карту параметров класс не имеющий объектов,
 * а использующийся как базовый для классов конкретных ЧРП
 */
class Vsd: public Device
{

public:

  Vsd();
  virtual ~Vsd();

  void initParameters();

  /*!
   * \brief Метод получения текущего значения частоты
   * \return
   */
  float getCurrentFreq();

  /*!
   * \brief Метод задания типа двигателя
   * \param TypeMotor - Тип двигателя
   * \return Код результата операции
   */
  int setTypeMotor(double TypeMotor);

  /*!
   * \brief Метод задания типа управления двигателя
   * \param ControlMotor - Тип управления двигателя
   * \return Код результата операции
   */
  int setTypeControlMotor(double ControlMotor);

  /*!
   * \brief Метод задания номинального тока двигателя
   * \param CurrentMotor - номинальный ток двигателя
   * \return Код результата операции
   */
  int setCurrentMotor(double CurrentMotor);

  /*!
   * \brief Метод записи номинального напряжения двигателя
   * \param VoltageMotor - номинальное напряжение двигателя
   * \return Код результата операции
   */
  int setVoltageMotor(double VoltageMotor);

  /*!
   * \brief Метод задания номинальной частоты двигателя
   * \param FrequencyMotor
   * \return Код результата операции
   */
  int setFrequencyMotor(double FrequencyMotor);

  /*!
   * \brief Метод запуска ЧРП
   * \return Код результата операции
   */
  virtual int startVSD() = 0;

  /*!
   * \brief Метод останова ЧРП
   * \return Код результата операции
   */
  virtual int stopVSD() = 0;

  /*!
   * \brief Метод задания частоты
   * \param value
   * \return Код результата операции
   */
  virtual int setFrequency(float value) = 0;

  /*!
   * \brief Метод задания минимальной частоты
   * \param value
   * \return Код результата операции
   */
  int setMinFrequency(float value);

  /*!
   * \brief Метод задания максимальной частоты
   * \param value
   * \return Код результата операции
   */
  int setMaxFrequency(float value);

  /*!
   * \brief Метод задания направления вращения
   * \param value - направление вращения
   * \return Код результата операции
   */
  int setRotation(uint8_t value);

  /*!
   * \brief Метод задания прямого направления вращения
   * \return Код результата операции
   */
  int setDirectRotation();

  /*!
   * \brief Метод задания обратного направления вращения
   * \return Код результата операции
   */
  int setReverseRotation();

  /*!
   * \brief Метод задания темпа набора частоты
   * \return Код результата операции
   */
  int setSpeedUp(double SpeedUp);

  /*!
   * \brief Метод задания снижения частоты
   * \param SpeedDown
   * \return Код результата операции
   */
  int setSpeedDown(double SpeedDown);

  /*!
   * \brief Метод задания противоЭДС
   * \param BackEmf
   * \return Код результата операции
   */
  int setBackEmf(double BackEmf);

  /*!
   * \brief Метод задания предела тока
   * \return Код результата операции
   */
  int setCurrentLim(double CurrentLim);

  /*!
   * \brief Метод задания индуктивности
   * \param Indunstance
   * \return Код результата операции
   */
  int setDAxisIndunstance(double Indunstance);

  /*!
   * \brief Метод задания времени фильтрации
   * \param FilterTimeCurrentLim
   * \return Код результата операции
   */
  int setFilterTimeCurrentLim(double FilterTimeCurrentLim);

  /*!
   * \brief Метод задания времени фильтрации на высокой скорости
   * \param HighSpeedFilterTimeCurrentLim
   * \return Код результата операции
   */
  int setHighSpeedFilterTimeCurrentLim(double HighSpeedFilterTimeCurrentLim);

  /*!
   * \brief Метод задания количества полюсов
   * \param PolesMotor
   * \return Код результата операции
   */
  int setPolesMotor(double PolesMotor);

  /*!
   * \brief Метод задания номинального момента двигателя
   * \param RateTorqueMotor
   * \return Код результата операции
   */
  int setRateTorqueMotor(double RateTorqueMotor);

  /*!
   * \brief Метод задания сопротивления статора
   * \param ResistanceStator
   * \return Код результата операции
   */
  int setResistanceStator(double ResistanceStator);

  /*!
   * \brief Метод записи скорости вращения двигателя
   * \param SpeedMotor
   * \return Код результата операции
   */
  int setSpeedMotor(double SpeedMotor);

  /*!
   * \brief Метод задания частоты коммутации ШИМ
   * \param SwitchingFrequency
   * \return Код результата операции
   */
  int setSwitchingFrequency(double SwitchingFrequency);

  /*!
   * \brief Метод задания момента
   * \param TorqueLimit
   * \return Код результата операции
   */
  int setTorqueLimit(double TorqueLimit);

private:
  //! Массив параметров устройства
  parameter parametersArray_[VSD_END - VSD_BEGIN];

};

#endif /* VSD_H_ */
