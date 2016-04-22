/*
 * vsd.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef VSD_H_
#define VSD_H_

#include "device.h"
#include "vsd_log.h"

#define VSD_CMD_NUMBER_REPEATS 10 //!< Количество повторов
#define VSD_CMD_TIMEOUT 1000      //!< Время ожидания подтверждения команды

/*!
 * \brief Коды аварий ЧРП
 */
enum enAlarmVsd {
  VSD_ALARM_NONE                  = 0,      //!< Нет аварии
  VSD_ALARM_NO_CONNECT            = 250     //!< Нет связи
};

/*!
 * \brief Коды предупреждений ЧРП
 */
enum enWarningVsd {
  VSD_WARNING_NONE                = 0,      //!< Нет предупреждения
};

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

  // ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ДВИГАТЕЛЯ
  /*!
   * \brief Метод задания типа двигателя в массив параметров
   * \param value - тип двигателя: VSD_MOTOR_TYPE_ASYNC = 0, VSD_MOTOR_TYPE_VENT = 1
   * \return 0 - задание успешно, !0 - ошибки
   */
  int setMotorType(float value);

  /*!
   * \brief setMotorFrequency
   * \param value
   * \return
   */
  virtual int setMotorFrequency(float value);

  /*!
   * \brief setMotorCurrent
   * \param value
   * \return
   */
  virtual int setMotorCurrent(float value);

  /*!
   * \brief setMotorVoltage
   * \param value
   * \return
   */
  virtual int setMotorVoltage(float value);

  /*!
   * \brief setMotorSpeed
   * \param value
   * \return
   */
  virtual int setMotorSpeed(float value);

  /*!
   * \brief setMotorPower
   * \param value
   * \return
   */
  virtual int setMotorPower(float value);

  /*!
   * \brief setLimitsMotor
   * \return
   */
  virtual void setLimitsMotor();

  // РЕЖИМЫ ПУСКА
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
   * \brief onRegimeAutoAdaptation
   * \return
   */
  virtual int onRegimeAutoAdaptation();

  /*!
   * \brief offRegimeAutoAdaptation
   * \return
   */
  virtual int offRegimeAutoAdaptation();

  /*!
   * \brief onRegimeCurrentLimit
   * \return
   */
  virtual int onRegimeCurrentLimitation();

  /*!
   * \brief offRegimeCurrentLimit
   * Виртуальная функция задания
   * \return
   */
  virtual int offRegimeCurrentLimitation();

  /*!
   * \brief configRegimeCurrentLimitation
   * Виртуальная функция задания настроек "Режима ограничения тока"
   * \return
   */
  virtual int configRegimeCurrentLimitation();

  // ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ЧРП
  /*!
   * \brief Функция записи в регистр управления двигателем
   * \param value
   * \return 0 - задание успешно, № - ошибки
   */
  int setVsdControl(float value);
  
  /*!
   * \brief Функция записи в регистр направления вращения
   * \param value - направление вращения
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setRotation(float value);  
  
  /*!
   * \brief Функция записи в регистр минимальной частоты
   * \param value - минимальная частота
   * \return 0 - задание успешно, № - ошибки
   */
  int setMinFrequency(float value);

  /*!
   * \brief setLimitsMinFrequence функция присвоения минимальной границы
   * Максимальной частоты и уставки частоты минимальной частоте
   * \param value
   * \return
   */
  int setLimitsMinFrequence(float value);
  
  /*!
   * \brief Функция записи в регистр максимальной частоты
   * \param value - максимальная частота
   * \return 0 - задание успешно, № - ошибки
   */
  int setMaxFrequency(float value);

  /*!
   * \brief setLimitsMaxFrequence функция присвоения максимальной границы
   * минимальной частоты и уставки частоты максимальной частоте частоте
   * \param value
   * \return
   */
  int setLimitsMaxFrequence(float value);

  /*!
   * \brief Функция записи в регистр уставки частоты
   * \param value присваемое значение
   * \param eventType - тип события для фиксирования изменений в журнале
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setFrequency(float value, EventType eventType = NoneType);

  /*!
   * \brief setTimeSpeedUp
   * \param value
   * \return 
   */
  virtual int setTimeSpeedUp(float value);
  
  /*!
   * \brief setTimeSpeedDown
   * \param value
   * \return 
   */
  virtual int setTimeSpeedDown(float value);
  
  /*!
   * \brief Функция записи в регистр частоты коммутации (Частота ШИМ)
   * \param value задаваемая частота коммутации (Частота ШИМ)
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setSwitchingFrequency(float value);

  /*!
   * \brief setSwitchingFrequencyCode
   * \param value
   * \return
   */
  virtual int setSwitchingFrequencyCode(float value);

  /*!
   * \brief setSwitchingFrequencyMode
   * \param value
   * \return
   */
  virtual int setSwitchingFrequencyMode(float value);

  /*!
   * \brief setResonanceRemoveSource
   * \param value
   * \return
   */
  virtual int setResonanceRemoveSource(float value);

  /*!
   * \brief setOutFilter
   * \param value
   * \return
   */
  virtual int setOutFilter(float value);

  // НАСТРОЙКА U/f
  /*!
   * \brief setUf_f1
   * \param value
   * \return
   */
  int setUf_f1(float value);

  /*!
   * \brief setUf_f2
   * \param value
   * \return
   */
  int setUf_f2(float value);

  /*!
   * \brief setUf_f3
   * \param value
   * \return
   */
  int setUf_f3(float value);

  /*!
   * \brief setUf_f4
   * \param value
   * \return
   */
  int setUf_f4(float value);

  /*!
   * \brief setUf_f5
   * \param value
   * \return
   */
  int setUf_f5(float value);

  /*!
   * \brief setUf_f6
   * \param value
   * \return
   */
  int setUf_f6(float value);

  /*!
   * \brief setUf_U1
   * \param value
   * \return
   */
  int setUf_U1(float value);

  /*!
   * \brief setUf_U2
   * \param value
   * \return
   */
  int setUf_U2(float value);

  /*!
   * \brief setUf_U3
   * \param value
   * \return
   */
  int setUf_U3(float value);

  /*!
   * \brief setUf_U4
   * \param value
   * \return
   */
  int setUf_U4(float value);

  /*!
   * \brief setUf_U5
   * \param value
   * \return
   */
  int setUf_U5(float value);

  /*!
   * \brief setUf_U6
   * \param value
   * \return
   */
  int setUf_U6(float value);

  /*!
   * \brief setDischarge
   * \param value
   * \return
   */
  int setDischarge(float value);

  /*!
   * \brief calcUfCharacteristic
   * \return
   */
  virtual int calcUfCharacteristicU(float value);

  /*!
   * \brief calcUfCharacteristicF
   * \param value
   * \return
   */
  virtual int calcUfCharacteristicF(float value);

  /*!
   * \brief Метод задания предела тока
   * \return Код результата операции
   */
  int setCurrentLim(float value);

  /*!
   * \brief Функция записи в регистр суммарной индуктивности
   * \param induct - задаваемая суммарная индуктивности
   * \return 0 - задание успешно, № - ошибки
   */
  virtual int setSumInduct(float value);


  int setUfU(uint16_t id, float value);



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
  virtual int start(bool init);

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

  /*!
   * \brief Метод проверки что выходная частота равна частоте уставки
   * \return
   */
  virtual bool isSetPointFreq();

  virtual int resetBlock();

  virtual float checkAlarmVsd();
  virtual bool checkPreventVsd();

  virtual void processingRegimeRun();



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
   * \brief onRegimeAutoOptimCurrent
   * \return
   */
  virtual int onRegimeAutoOptimCurrent();

  /*!
   * \brief offRegimeAutoOptimCurrent
   * \return
   */
  virtual int offRegimeAutoOptimCurrent();

  /*!
   * \brief resetSetpoints
   * \return
   */
  virtual int resetSetpoints();

  /*!
   * \brief Функция вычисления текущего cos ЧРП по активной и полной мощности
   * \return
   */
   int calcMotorCos();

  /*!
   * \brief calcVsdPowerFull Функция вычисления текущей полной мощности
   * \return
   */
  float calcVsdPowerFull();

  /*!
   * \brief Функция вычисления тока звена постоянного тока
   */
  float calcCurrentDC();

  /*!
   * \brief calcTempSpeedUp
   */
  void calcTempSpeedUp();

  /*!
   * \brief calcTempSpeedDown
   */
  void calcTempSpeedDown();

  /*!
   * \brief calcTimeSpeedUp
   */
  void calcTimeSpeedUp();

  /*!
   * \brief calcTimeSpeedDown
   */
  void calcTimeSpeedDown();

  /*!
   * \brief setLimitsCcsParameters
   * Функция задания границ для параметров КСУ связанных с параметрами ЧРП
   */
  virtual void setLimitsCcsParameters();

  /*!
   * \brief getConnect
   */
  virtual void getConnect();

  /*!
   * \brief setConnect
   * \param counters
   */
  void setConnect(stConnectQuality *counters);

  /*!
   * \brief resetConnect
   */
  virtual void resetConnect();

  /*!
   * \brief writeToDevice
   */
  virtual void writeToDevice(int id, float value);

  VsdLog *log() const { return log_; }

protected:
  VsdLog *log_;

  int startTimeMs_;
  int startCountRepeats_;

};

#endif /* VSD_H_ */
