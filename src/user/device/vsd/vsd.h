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
   * \brief Метод получения текущего значения частоты из массива параметров
   * \return Текущее значение частоты
   */
  float getCurrentFreq();

  /*!
   * \brief Метод получения уставки частоты из массива параметров
   * \return Текущее значение частоты
   */
  float getSetpointFreq();

  /*!
   * \brief Метод получения номинальной частоты двигателя из массива параметров
   * \return Метод получения номинальной частоты двигателя
   */
  float getNominalFreq();

  /*!
   * \brief Метод задания типа двигателя в массив параметров
   * \param value Тип двигателя
   * \return
   */
  virtual int setMotorType(float value);

  /*!
   * \brief Метод задания направления вращения
   * \param value - направление вращения
   * \return Код результата операции
   */
  virtual int setRotation(float value);

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
  virtual bool checkVsdStatus(uint8_t bit);

  /*!
   * \brief Метод задания частоты
   * \param value
   * \return Код результата операции
   */
  virtual int setFrequency(float value);

  /*!
   * \brief Метод задания минимальной частоты
   * \param value
   * \return Код результата операции
   */
  virtual int setMinFrequency(float value);

  /*!
   * \brief Метод задания максимальной частоты
   * \param value
   * \return Код результата операции
   */
  virtual int setMaxFrequency(float value);

  /*!
   * \brief Метод задания темпа набора частоты Гц/с
   * \param value
   * \return
   */
  virtual int setTempSpeedUp(float value);

  /*!
   * \brief Метод задания темпа снижения частоты Гц/с
   * \param value
   * \return
   */
  virtual int setTempSpeedDown(float value);

  /*!
   * \brief Метод задания времени набора частоты
   * \return Код результата операции
   */
  virtual int setSpeedUp(float value);

  /*!
   * \brief Метод задания времени снижения частоты
   * \param SpeedDown
   * \return Код результата операции
   */
  virtual int setSpeedDown(float SpeedDown);


  /*!
   * \brief Метод задания типа управления двигателем
   * \param value
   * \return Код результата операции
   */
  virtual int setMotorControl(float value);

  /*!
   * \brief Метод задания предела тока
   * \return Код результата операции
   */
  virtual int setCurrentLim(float value);

  /*!
   * \brief Функция задания частоты коммутации (Частота ШИМ)
   * \param value задаваемая частота коммутации (Частота ШИМ)
   * \return Код результата операции
   */
  virtual int setSwitchingFrequency(float value);

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
   * \brief Задание в регистр номинальной частоты двигатля
   * \param freq Задаваемая частота
   * \return
   */
  int setNominalFreq(float freq);

private:
  //! Массив параметров устройства
  parameter parametersArray_[VSD_END - VSD_BEGIN];

};

#endif /* VSD_H_ */
