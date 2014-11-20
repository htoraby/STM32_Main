/*
 * vsd.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef VSD_H_
#define VSD_H_

#include "device.h"

// КЛАСС ЧРП
// Наследник класса Device использующий его структуру и методы хранения банка параметров
// имеющий свою карту параметров класс не имеющий объектов, а использующийся как базовый для
// классов конкретных ЧРП
/*!
 * \brief The Vsd class
 * Наследник класса Device использующий его структуру и методы хранения банка
 * параметров имеющий свою карту параметров класс не имеющий объектов,
 * а использующийся как базовый для классов конкретных ЧРП
 */
class Vsd: public Device
{
  // ОТКРЫТЫЕ ЧЛЕНЫ КЛАССА
public:
  /// Конструктор класса
  Vsd();
  /// Деструктор класса
  virtual ~Vsd();

  void initParameters();

  // ФУНКЦИИ ДЛЯ РАБОТЫ С ЧРП
  /*!
   * \brief setTypeMotor
   * Метод задания типа двигателя
   * \param TypeMotor - Тип двигателя
   * \return Код результата операции
   */
  int setTypeMotor(double TypeMotor);

  /*!
   * \brief setTypeControlMotor
   * Метод задания типа управления двигателя
   * \param ControlMotor - Тип управления двигателя
   * \return Код результата операции
   */
  int setTypeControlMotor(double ControlMotor);

  /*!
   * \brief setCurrentMotor
   * Метод задания номинального тока двигателя
   * \param CurrentMotor - номинальный ток двигателя
   * \return Код результата операции
   */
  int setCurrentMotor(double CurrentMotor);

  /*!
   * \brief setVoltageMotor
   * Метод записи номинального напряжения двигателя
   * \param VoltageMotor - номинальное напряжение двигателя
   * \return Код результата операции
   */
  int setVoltageMotor(double VoltageMotor);

  /*!
   * \brief setFrequencyMotor
   * Метод задания номинальной частоты двигателя
   * \param FrequencyMotor
   * \return Код результата операции
   */
  int setFrequencyMotor(double FrequencyMotor);

  /*!
   * \brief startVSD
   * Метод запуска ЧРП
   * \return Код результата операции
   */
  int startVSD(void);

  /*!
   * \brief stopVSD
   * Метод останова ЧРП
   * \return Код результата операции
   */
  int stopVSD();

  /*!
   * \brief setFrequency
   * Метод задания частоты
   * \param Frequency
   * \return Код результата операции
   */
  int setFrequency(double Frequency);

  /*!
   * \brief setMinFrequency
   * Метод задания минимальной частоты
   * \param MinFrequency
   * \return Код результата операции
   */
  int setMinFrequency(double Frequency);

  /*!
   * \brief setMaxFrequency
   * Метод задания максимальной частоты
   * \return Код результата операции
   */
  int setMaxFrequency(double Frequency);

  /*!
   * \brief setDirectRotation
   * Метод задания прямого направления вращения
   * \return Код результата операции
   */
  int setDirectRotation();

  /*!
   * \brief setReverseRotation
   * Метод задания обратного направления вращения
   * \return Код результата операции
   */
  int setReverseRotation();

  /*!
   * \brief setSpeedUp
   * Метод задания темпа набора частоты
   * \return Код результата операции
   */
  int setSpeedUp(double SpeedUp);

  /*!
   * \brief setSpeedDown
   * Метод задания снижения частоты
   * \param SpeedDown
   * \return Код результата операции
   */
  int setSpeedDown(double SpeedDown);

  /*!
   * \brief setBackEmf
   * Метод задания противоЭДС
   * \param BackEmf
   * \return Код результата операции
   */
  int setBackEmf(double BackEmf);

  /*!
   * \brief setCurrentLim
   * Метод задания предела тока
   * \return Код результата операции
   */
  int setCurrentLim(double CurrentLim);

  /*!
   * \brief setDAxisIndunstance
   * Метод задания индуктивности
   * \param Indunstance
   * \return Код результата операции
   */
  int setDAxisIndunstance(double Indunstance);

  /*!
   * \brief setFilterTimeCurrentLim
   * Метод задания времени фильтрации
   * \param FilterTimeCurrentLim
   * \return Код результата операции
   */
  int setFilterTimeCurrentLim(double FilterTimeCurrentLim);

  /*!
   * \brief setHighSpeedFilterTimeCurrentLim
   * Метод задания времени фильтрации на высокой скорости
   * \param HighSpeedFilterTimeCurrentLim
   * \return Код результата операции
   */
  int setHighSpeedFilterTimeCurrentLim(double HighSpeedFilterTimeCurrentLim);

  /*!
   * \brief setPolesMotor
   * Метод задания количества полюсов
   * \param PolesMotor
   * \return Код результата операции
   */
  int setPolesMotor(double PolesMotor);

  /*!
   * \brief setRateTorqueMotor
   * Метод задания номинального момента двигателя
   * \param RateTorqueMotor
   * \return Код результата операции
   */
  int setRateTorqueMotor(double RateTorqueMotor);

  /*!
   * \brief setResistanceStator
   * Метод задания сопротивления статора
   * \param ResistanceStator
   * \return Код результата операции
   */
  int setResistanceStator(double ResistanceStator);

  /*!
   * \brief setSpeedMotor
   * Метод записи скорости вращения двигателя
   * \param SpeedMotor
   * \return Код результата операции
   */
  int setSpeedMotor(double SpeedMotor);

  /*!
   * \brief setSwitchingFrequency
   * Метод задания частоты коммутации ШИМ
   * \param SwitchingFrequency
   * \return Код результата операции
   */
  int setSwitchingFrequency(double SwitchingFrequency);

  /*!
   * \brief setTorqueLimit
   * Метод задания момента
   * \param TorqueLimit
   * \return Код результата операции
   */
  int setTorqueLimit(double TorqueLimit);

protected:

};

#endif /* VSD_H_ */
