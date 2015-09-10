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
 * параметров имеющий свою карту параметров класс, использующийся как базовый
 * для классов конкретных ЧРП
 */
class Vsd: public Device
{

public:
  Vsd();
  virtual ~Vsd();

  /*!
   * \brief Метод инициализации внутреннего массива параметров ЧРП
   */
  void initParameters();

  /*!
   * \brief Виртуальный метод задания типа двигателя в массив параметров
   * \param value - тип двигателя: VSD_MOTOR_TYPE_ASYNC = 0, VSD_MOTOR_TYPE_VENT = 1
   * \return 0 - задание успешно, !0 - ошибки
   */
  virtual int setMotorType(float value);

  /*!
   * \brief Функция записи в регистр номинальной частоты двигателя
   * \param freq - задаваемая частота
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setMotorNominalFreq(float freq);

  /*!
   * \brief Функция записи в регистр номинального тока двигателя
   * \param current - задаваемый ток
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setMotorNominalCurrent(float current);

  /*!
   * \brief Функция записи в регистр номинального напряжения двигателя
   * \param voltage - задаваемое напряжение
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setMotorNominalVoltage(float voltage);

  /*! Задаваемые параметры ЧРП */
  /*!
   * \brief Функция записи в регистр частоты коммутации (Частота ШИМ)
   * \param value задаваемая частота коммутации (Частота ШИМ)
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setSwitchingFrequency(float value);

  /*!
   * \brief Метод задания предела тока
   * \return Код результата операции
   */
  virtual int setCurrentLim(float value);

  /*!
   * \brief Функция записи в регистр суммарной индуктивности
   * \param induct - задаваемая суммарная индуктивности
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setSumInduct(float induct);

  /*! Задаваемые параметры работы */
  /*!
   * \brief Функция записи в регистр уставки частоты
   * \param value - задаваемая частота
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setFrequency(float value);

  /*!
   * \brief Функция записи в регистр минимальной частоты
   * \param value - минимальная частота
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setMinFrequency(float value);

  /*!
   * \brief Функция записи в регистр максимальной частоты
   * \param value - максимальная частота
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setMaxFrequency(float value);

  int setLimitsFrequence(bool Min, float value);

  virtual int setUfU(uint16_t id, float value);

  virtual int setUfU1(float value);
  virtual int setUfU2(float value);
  virtual int setUfU3(float value);
  virtual int setUfU4(float value);
  virtual int setUfU5(float value);

  /*!
   * \brief Функция записи в регистр темпа набора частоты Гц/с
   * и пересчитанного значения в регистр время набора частота
   * \param value - темп
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setTimeSpeedUp(float value);
  virtual int setTimeSpeedDown(float value);
  virtual int setTempSpeedUp(float value);
  virtual int setTempSpeedDown(float value);

  /*!
   * \brief Функция записи в регистр направления вращения
   * \param value - направление вращения
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setRotation(float value);

  /*!
   * \brief Функция записи в регистр управления двигателем
   * \param value
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setMotorControl(float value);

  /*! Читаемые параметры ЧРП */
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
   * \brief getTypeStop
   * \return
   */
  float getTypeStop();

  /*! Команды и операции */

  /*!
   * \brief Метод запуска ЧРП
   * \return Код результата операции
   */
  virtual int start();

  /*!
   * \brief Метод останова ЧРП
   * \param type - тип останова
   * \return
   */
  virtual int stop(float type);

  /*!
   * \brief Метод проверки запуска ЧРП
   * \return true - запуск, false - нет
   */
  virtual bool checkStart();

  /*!
   * \brief Метод проверки останова ЧРП
   * \return true - останов, false - нет
   */
  virtual bool checkStop();

  virtual int resetBlock();

  /*!
   * \brief Метод проверки флага в регистре статуса инвертора
   * \param bit - проверяемый бит
   * \return 0 - бит не установлен 1 - бит установлен
   */
  virtual bool checkVsdStatus(uint8_t bit);

  virtual void processingRegimeRun();

  /*!
   * \brief Метод настройки толчкового режима
   * \return
   */
  virtual int onRegimePush();

  /*!
   * \brief Метод выключения толчкогового режима
   * \return
   */
  virtual int offRegimePush();

  /*!
   * \brief onRegimeSwing
   * \return
   */
  virtual int onRegimeSwing();

  /*!
   * \brief offRegimeSwing
   * \return
   */
  virtual int offRegimeSwing();

  /*!
   * \brief onRegimeJarring
   * \return
   */
  virtual int onRegimeJarring();

  /*!
   * \brief offRegimeJarring
   * \return
   */
  virtual int offRegimeJarring();

  /*!
   * \brief Метод настройки режима подхвата
   * \return
   */
  virtual int onRegimePickup();

  /*!
   * \brief offRegimePickup
   * \return
   */
  virtual int offRegimePickup();

  /*!
   * \brief onRegimeSkipFreq
   * \return
   */
  virtual int onRegimeSkipFreq();

  /*!
   * \brief offRegimeSkipFreq
   * \return
   */
  virtual int offRegimeSkipFreq();

  /*!
   * \brief resetSetpoints
   * \return
   */
  virtual int resetSetpoints();

  /*!
   * \brief Функция вычисления текущего cos ЧРП по активной и полной мощности
   * \return
   */
  int calcVsdCos();


protected:

private:
  //! Массив параметров устройства
  parameter parametersArray_[VSD_END - VSD_BEGIN];

};

#endif /* VSD_H_ */
