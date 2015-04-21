/*
 * vsd_novomet.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef VSD_ETALON_H_
#define VSD_ETALON_H_

#include "vsd.h"
#include "device_modbus.h"

class VsdEtalon: public Vsd
{
public:
  VsdEtalon();
  virtual ~VsdEtalon();

  void init();
  void initModbusParameters();
  void initParameters();

  void getNewValue(uint16_t id);
  uint8_t setNewValue(uint16_t id, float value);

  /*!
   * \brief Метод записи параметра в устройство
   * \param id
   * \param value
   * \return
   */
  void writeToDevice(int id, float value);

  /*!
   * \brief Метод запуска
   * \return Код выполнения операции
   */
  int start();

  /*!
   * \brief Метод останова
   * \return Код выполнения операции
   */
  int stop();

  /*!
   * \brief alarmstop
   * \return
   */
  int alarmstop();

  bool checkStart();
  bool checkStop();

  /*!
   * \brief Функция включения толчкового режима
   * \return
   */
  int onRegimePush();

  /*!
   * \brief Функция выключения толчкового режима
   * \return
   */
  int offRegimePush();

  /*!
   * \brief onRegimeSwing
   * \return
   */
  int onRegimeSwing();

  /*!
   * \brief offRegimeSwing
   * \return
   */
  int offRegimeSwing();

  /*!
   * \brief onRegimeJarring
   * \return
   */
  int onRegimeJarring();

  /*!
   * \brief offRegimeJarring
   * \return
   */
  int offRegimeJarring();

  /*!
   * \brief Метод записи основного режима работы ЧРП
   * \return
   */
  int setMainRegimeVSD();

  /*!
   * \brief Метод задания уставки частоты двигателя
   * При установке текущей частоты проверяется диапазон уставок
   * минимальной и максимальной частоты
   * \param value - частота
   * \return
   */
  int setFrequency(float value);

  /*!
   * \brief Метод задания минимальной частоты
   * \param value частота
   * \return 0 - задали 1 - не задали
   */
  int setMinFrequency(float value);

  /*!
   * \brief Метод задания максимальной частоты
   * \param value - значение максимальной частоты
   * \return 0 - задали 1 - не задали
   */
  int setMaxFrequency(float value);

  /*!
   * \brief Метод задания типа двигателя
   * \param TypeMotor - Тип двигателя
   * \return Код результата операции
   */
  int setMotorType(float value);

  /*!
   * \brief getMotorType
   */
  void calcMotorType();

  /*!
   * \brief calcSpeedUp
   */
  void calcTempSpeedUp();

  /*!
   * \brief calcTimeSpeedUp
   */
  void calcTimeSpeedUp();

  /*!
   * \brief calcMotorControl
   */
  void calcMotorControl();

  /*!
   * \brief calcRotation
   */
  void calcRotation();

  /*!
   * \brief Функция вычисления тока звена постоянного тока
   */
  void calcCurrentDC();

  /*!
   * \brief Метод задания времени набора частоты
   * \return Код результата операции
   */
  int setTempSpeedUp(float value);

  /*!
   * \brief Метод задания времени снижения частоты
   * \param SpeedDown
   * \return Код результата операции
   */
  int setTempSpeedDown(float value);

  /*!
   * \brief Метод задания точки характеристики U/f F1
   * \param value
   * \return
   */
  int setUfF1(float value);

  /*!
   * \brief Метод задания точки характеристики U/f F2
   * \param value
   * \return
   */

  int setUfF2(float value);
  /*!
   * \brief Метод задания точки характеристики U/f F3
   * \param value
   * \return
   */
  int setUfF3(float value);

  /*!
   * \brief Метод задания точки характеристики U/f F4
   * \param value
   * \return
   */
  int setUfF4(float value);

  /*!
   * \brief Метод задания точки характеристики U/f F5
   * \param value
   * \return
   */
  int setUfF5(float value);

  /*!
   * \brief Метод задания точки характеристики U/f F6
   * \param value
   * \return
   */
  int setUfF6(float value);

  /*!
   * \brief Метод задания точки характиристики U/f U1
   * \param value
   * \return
   */
  int setUfU1(float value);

  /*!
   * \brief Метод задания точки характиристики U/f U2
   * \param value
   * \return
   */
  int setUfU2(float value);

  /*!
   * \brief Метод задания точки характиристики U/f U3
   * \param value
   * \return
   */
  int setUfU3(float value);

  /*!
   * \brief Метод задания точки характиристики U/f U4
   * \param value
   * \return
   */
  int setUfU4(float value);

  /*!
   * \brief Метод задания точки характиристики U/f U5
   * \param value
   * \return
   */
  int setUfU5(float value);

  /*!
   * \brief Метод задания точки характиристики U/f U6
   * \param value
   * \return
   */
  int setUfU6(float value);

  /*!
   * \brief Метод задания направления вращения
   * \param value - направление вращения 1 прямое(правое),
   * 2 обратное(левое)
   * \return
   */
  int setRotation(float value);

  /*!
   * \brief Проверка на "необходимость" работы с параметром
   * \param indexParam
   * \return
   */
  int checkExchangModbusParameters(int indexParam);

  /*!
   * \brief Функция вычисления значений,
   */
  void calcParameters(uint16_t id);

private:
  ModbusParameter modbusParameters_[135];
  DeviceModbus *dm_;

};

#endif /* VSD_NOVOMET_H_ */
