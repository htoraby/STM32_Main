/*
 * classTms.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef TMS_H_
#define TMS_H_

#include "device.h"

/*!
 * \brief Базовый класс ТМС
 * Наследник класса Device использующий его структуру и методы хранения
 * банка параметров имеющий свою карту параметров класс не имеющий объектов,
 * а использующийся как базовый для классов конкретных ТМС
 */
class Tms: public Device
{
public:
  Tms();
  virtual ~Tms();

  /*!
   * \brief Функция заполнения массива параметров ТМС.
   * Заполняем массив id параметрами и значенями по умолчанию (ошибочными)
   */
  void initParameters();

    /*!
   * \brief Функция задания единиц измерения давления
   * \param unit - код единиц измерения
   * \return
   */
  virtual int setUnitPressure(float unit);

  /*!
   * \brief Метод задания единиц измерения температуры
   * \param
   * \return
   */
  virtual int setUnitTemperature(float unit);

  /*!
   * \brief Метод задания единиц измерения виброускорения
   * \param
   * \return
   */
  int setUnitAcceleration(float Unit);

  /*!
   * \brief Метод задания единиц измерения виброскорости
   * \param
   * \return
   */
  int setUnitSpeed(float Unit);

  /*!
   * \brief Метод включения чтения статических параметров
   * \return
   */
  int setEnableReadingStaticParam();

  /*!
   * \brief Метод выключения чтения статических параметров
   * \return
   */
  int setDisableReadingStaticParam();

  /*!
   * \brief Метод включения режима КВД
   * \return
   */
  int setEnablePidLoop();

  /*!
   * \brief Метод выключения режима КВД
   * \return
   */
  int setDisablePidLoop();

  /*!
   * \brief getConnect
   */
  virtual void getConnect();

  /*!
   * \brief setConnect
   * \param counters
   */
  void setConnect(stConnectQuality *counters);

};


#endif /* TMS_H_ */
