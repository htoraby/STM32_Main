/*
 * vsd.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef VSD_H_
#define VSD_H_

#include "device.h"

#define VSD_CMD_NUMBER_REPEATS 10 //!< Количество повторов
#define VSD_CMD_TIMEOUT 1000      //!< Время ожидания подтверждения команды

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
   * \brief Метод получения уставки частоты
   * \return
   */
  float getSetpointFreq();

  /*!
   * \brief Метод получения номинальной частоты двигателя
   * \return
   */
  float getNominalFreq();

  /*!
   * \brief Метод задания типа двигателя
   * \param TypeMotor - Тип двигателя
   * \return Код результата операции
   */
  int setMotorType(float TypeMotor);

  /*!
   * \brief Метод задания направления вращения
   * \param value - направление вращения
   * \return Код результата операции
   */
  int setRotation(float value);

  /*!
   * \brief Метод запуска ЧРП
   * \return Код результата операции
   */
  virtual int start() = 0;

  /*!
   * \brief Метод останова ЧРП
   * \return Код результата операции
   */
  virtual int stop() = 0;

  /*!
   * \brief Метод проверки запуска ЧРП
   * \return true - запуск, false - нет
   */
  virtual bool checkStart() = 0;

  /*!
   * \brief Метод проверки останова ЧРП
   * \return true - останов, false - нет
   */
  virtual bool checkStop() = 0;

  /*!
   * \brief Метод проверки флага в регистре статуса инвертора
   * \param bit проверяемый бит
   * \return 0 - бит не установлен 1 - бит установлен
   */
  bool checkVsdStatus(uint8_t bit);

  /*!
   * \brief Метод задания частоты
   * \param value
   * \return Код результата операции
   */
  int setFrequency(float value);

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
   * \brief Метод задания темпа набора частоты Гц/с
   * \param value
   * \return
   */
  int setTempSpeedUp(float value);

  /*!
   * \brief Метод задания темпа снижения частоты Гц/с
   * \param value
   * \return
   */
  int setTempSpeedDown(float value);

  /*!
   * \brief Метод задания времени набора частоты
   * \return Код результата операции
   */
  int setSpeedUp(float value);

  /*!
   * \brief Метод задания времени снижения частоты
   * \param SpeedDown
   * \return Код результата операции
   */
  int setSpeedDown(float SpeedDown);

  /*!
   * \brief Метод задания противоЭДС
   * \param BackEmf
   * \return Код результата операции
   */
  int setBackEmf(double BackEmf);

  /*!
   * \brief Метод задания типа управления двигателем
   * \param value
   * \return Код результата операции
   */
  int setMotorControl(float value);

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

  /*!
   * \brief Метод настройки толчкового режима
   * \return
   */
  virtual int onRegimePush() = 0;

  /*!
   * \brief Метод выключения толчкогового режима
   * \return
   */
  virtual int offRegimePush() = 0;

  /*!
   * \brief onRegimeSwing
   * \return
   */
  virtual int onRegimeSwing() = 0;

  /*!
   * \brief offRegimeSwing
   * \return
   */
  virtual int offRegimeSwing() = 0;

  /*!
   * \brief onRegimeJarring
   * \return
   */
  virtual int onRegimeJarring() = 0;

  /*!
   * \brief offRegimeJarring
   * \return
   */
  virtual int offRegimeJarring() = 0;

  /*!
   * \brief Метод настройки режима подхвата
   * \return
   */
  virtual int onRegimePickup() = 0;

  /*!
   * \brief Задание в массив параметров ЧРП номинальной частоты двигателя
   * \param freq Задаваемая номинальная частота двигателя
   * \return
   */
  int setNominalFreq(float freq);

private:
  //! Массив параметров устройства
  parameter parametersArray_[VSD_END - VSD_BEGIN];

};

#endif /* VSD_H_ */
