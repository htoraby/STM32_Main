#ifndef VSDDANFOSS_H
#define VSDDANFOSS_H

#include "vsd.h"
#include "device_modbus.h"

class RegimeRunPush;
class RegimeRunSwing;
class RegimeRunAdaptationVector;
class RegimeRunPickup;
class RegimeRunSkipResonantFreq;
class RegimeRunSynchron;

#define QUNTITY_PROFILES_MOTOR    15
#define QUANTITY_PARAMETER_MOTOR  45

enum enVsdDanfosStatus1
{
  VSD_DANFOSS_STATUS_CONTROL_READY= 0,
  VSD_DANFOSS_STATUS_DRIVE_READY  = 1,
  VSD_DANFOSS_STATUS_COASTING_STOP= 2,
  VSD_DANFOSS_STATUS_TRIP         = 3,
  VSD_DANFOSS_STATUS_ERROR        = 4,
  VSD_DANFOSS_STATUS_TRIP_LOCK    = 6,
  VSD_DANFOSS_STATUS_WARNING      = 7,
  VSD_DANFOSS_STATUS_SPEED        = 8,
  VSD_DANFOSS_STATUS_BUS_CONTROL  = 9,
  VSD_DANFOSS_STATUS_OUT_LIMIT    = 10,
  VSD_DANFOSS_STATUS_OPERATION    = 11,
  VSD_DANFOSS_STATUS_TEMPERATURE  = 12,
  VSD_DANFOSS_STATUS_VOLTAGE      = 13,
  VSD_DANFOSS_STATUS_TORQUE       = 14,
  VSD_DANFOSS_STATUS_TIMER        = 15,
};

/*!
 * \brief Коды аварий ЧРП
 */
enum enVsdDanfossAlarm {
  // STATUS_WORD_3 ЧРП Danfoss
  VSD_DANFOSS_ALARM_A_28          = 2000,   //!< Проверка тормоза A28
  VSD_DANFOSS_ALARM_A_69          = 2001,   //!< Темп. силовой платы A69
  VSD_DANFOSS_ALARM_A_14          = 2002,   //!< Пробой на землю A14
  VSD_DANFOSS_ALARM_A_65          = 2003,   //!< Темп. платы управления А65
  VSD_DANFOSS_ALARM_A_17          = 2004,   //!< Упр. слово ТО А17
  VSD_DANFOSS_ALARM_A_13          = 2005,   //!< Превыш тока А13
  VSD_DANFOSS_ALARM_A_12          = 2006,   //!< Предел момента A12
  VSD_DANFOSS_ALARM_A_11          = 2007,   //!< Перегрев термист. двигателя А11
  VSD_DANFOSS_ALARM_A_10          = 2008,   //!< Превышение ETR двигателя А10
  VSD_DANFOSS_ALARM_A_9           = 2009,   //!< Перегрузка инвертера А9
  VSD_DANFOSS_ALARM_A_8           = 2010,   //!< Пониж. напряжение пост. тока А8
  VSD_DANFOSS_ALARM_A_7           = 2011,   //!< Повыш. напряжение пост. тока А7
  VSD_DANFOSS_ALARM_A_16          = 2012,   //!< Короткое замыкание А16
  VSD_DANFOSS_ALARM_A_33          = 2013,   //!< Отказ из-за броска тока А33
  VSD_DANFOSS_ALARM_A_4           = 2014,   //!< Обрыв фазы питания А4
  VSD_DANFOSS_ALARM_AAD           = 2015,   //!< Сбой ААД
  VSD_DANFOSS_ALARM_A_2           = 2016,   //!< Ошибка действующего нуля А2
  VSD_DANFOSS_ALARM_A_38          = 2017,   //!< Внутренний отказ А38
  VSD_DANFOSS_ALARM_A_26          = 2018,   //!< Перегрузка тормоза А26
  VSD_DANFOSS_ALARM_A_30          = 2019,   //!< Обрыв фазы U A30
  VSD_DANFOSS_ALARM_A_31          = 2020,   //!< Обрыв фазы V A31
  VSD_DANFOSS_ALARM_A_32          = 2021,   //!< Обрыв фазы W A32
  VSD_DANFOSS_ALARM_A_34          = 2022,   //!< Отказ Fieldbus А34
  VSD_DANFOSS_ALARM_A_47          = 2023,   //!< Низкое напряжение питания 24В А47
  VSD_DANFOSS_ALARM_A_36          = 2024,   //!< Отказ питания А36
  VSD_DANFOSS_ALARM_A_48          = 2025,   //!< Низкое напряжение питания 1,8В А48
  VSD_DANFOSS_ALARM_A_25          = 2026,   //!< Тормозной резистор А25
  VSD_DANFOSS_ALARM_A_27          = 2027,   //!< Тормозной IGBT А27
  VSD_DANFOSS_ALARM_A_67          = 2028,   //!< Смена доп.устройства А67
  VSD_DANFOSS_ALARM_A_80          = 2029,   //!< Провод иниц. А80
  VSD_DANFOSS_ALARM_A_68          = 2030,   //!< Безоп. останов А68
  VSD_DANFOSS_ALARM_A_63          = 2031,   //!< Мала эффективность механич. тормоза А63
  // STATUS_WORD_4 ЧРП Danfoss
  VSD_DANFOSS_ALARM_SERVICE_TRIP  = 2032,   //!< Сервисная защита
  VSD_DANFOSS_ALARM_HI_TEMP_DISCHARGE = 2041,     //!< Высокая температура разряда
  VSD_DANFOSS_ALARM_START_ERROR   = 2042,   //!< Ошибка пуска
  VSD_DANFOSS_ALARM_SPEED_LIMIT   = 2043,   //!< Предел скорости
  VSD_DANFOSS_ALARM_EXTERNAL_BLOCK = 2044,  //!< Внешняя блокировка
  VSD_DANFOSS_ALARM_EXTERNAL_DEVICE = 2045, //!< Недопустимое сочетание дополнительных устройств
  VSD_DANFOSS_ALARM_PROT_DEVICE   = 2046,   //!< Дополнительное защитное устройство отсутствует
  VSD_DANFOSS_ALARM_KTY           = 2049,   //!< Ошибка KTY
  VSD_DANFOSS_ALARM_FAN           = 2050,   //!< Ошибка вентиляторов
  VSD_DANFOSS_ALARM_ECB           = 2051,   //!< Ошибка ECB
  VSD_DANFOSS_ALARM_A_59          = 2057,   //!< Предел по току А59
  VSD_DANFOSS_ALARM_A_90          = 2061,   //!< Отказ энкодера A90
  VSD_DANFOSS_ALARM_A_74          = 2062,   //!< Термистор PTC А74
  VSD_DANFOSS_ALARM_A_72          = 2063,   //!< Опасный отказ
  VSD_DANFOSS_ALARM_A_78          = 2999,   //!< Ошибка слежения
};

enum enVsdDanfoss {
  VSD_DANFOSS_WARNING_W_28        = 2000,   //!< Проверка тормоза W28
  VSD_DANFOSS_WARNING_W           = 2001,   //!< Недиагностированное предупреждение
  VSD_DANFOSS_WARNING_W_14        = 2002,   //!< Пробой на землю W14
  VSD_DANFOSS_WARNING_W_65        = 2003,   //!< Темп. платы управления W65
  VSD_DANFOSS_WARNING_W_17        = 2004,   //!< Упр. слово ТО W17
  VSD_DANFOSS_WARNING_W_13        = 2005,   //!< Превыш тока W13
  VSD_DANFOSS_WARNING_W_12        = 2006,   //!< Предел момента W12
  VSD_DANFOSS_WARNING_W_11        = 2007,   //!< Перегрев термист. двигателя W11
  VSD_DANFOSS_WARNING_W_10        = 2008,   //!< Превышение ETR двигателя W10
  VSD_DANFOSS_WARNING_W_9         = 2009,   //!< Перегрузка инвертера W9
  VSD_DANFOSS_WARNING_W_8         = 2010,   //!< Пониж. напряжение пост. тока W8
  VSD_DANFOSS_WARNING_W_7         = 2011,   //!< Повыш. напряжение пост. тока W7
  VSD_DANFOSS_WARNING_W_6         = 2012,   //!< Пониж. напряжение пост. тока W6
  VSD_DANFOSS_WARNING_W_5         = 2013,   //!< Повыш. напряжение пост. тока W5
  VSD_DANFOSS_WARNING_W_4         = 2014,   //!< Обрыв фазы питания W4
  VSD_DANFOSS_WARNING_W_3         = 2015,   //!< Нет двигателя W3
  VSD_DANFOSS_WARNING_W_2         = 2016,   //!< Ошибка действующего нуля W2
  VSD_DANFOSS_WARNING_W_1         = 2017,   //!< Низкое напряжение источника 10В W1
  VSD_DANFOSS_WARNING_W_26        = 2018,   //!< Перегрузка тормоза W26
  VSD_DANFOSS_WARNING_W_25        = 2019,   //!< Тормозной резистор W25
  VSD_DANFOSS_WARNING_W_27        = 2020,   //!< Тормозной IGBT W27
  VSD_DANFOSS_WARNING_W_49        = 2021,   //!< Предел скорости W49
  VSD_DANFOSS_WARNING_W_34        = 2022,   //!< Отказ Fieldbus W34
  VSD_DANFOSS_WARNING_W_47        = 2023,   //!< Низкое напряжение питания 24В W47
  VSD_DANFOSS_WARNING_W_36        = 2024,   //!< Отказ питания W36
  VSD_DANFOSS_WARNING_W_59        = 2025,   //!< Предел по току W59
  VSD_DANFOSS_WARNING_W_66        = 2026,   //!< Низкая температура W66
  VSD_DANFOSS_WARNING_W_64        = 2027,   //!< Предел по напряжению W64
  VSD_DANFOSS_WARNING_W_90        = 2028,   //!< Отказ энкодера W90
  VSD_DANFOSS_WARNING_W_62        = 2029,   //!< Пр вых. част W62
  VSD_DANFOSS_WARNING_W_68        = 2030,   //!< Безоп. останов W68
  VSD_DANFOSS_WARNING_EXT         = 2031,   //!< Расширенное слово состояния
};

enum enDanfossControl
{
  VSD_DANFOSS_CONTROL_DC_BREAK          = 2,
  VSD_DANFOSS_CONTROL_COASTING          = 3,
  VSD_DANFOSS_CONTROL_QUICK_STOP        = 4,
  VSD_DANFOSS_CONTROL_HOLD_FREQUENCY    = 5,
  VSD_DANFOSS_CONTROL_RAMP              = 6,
  VSD_DANFOSS_CONTROL_RESET             = 7,
  VSD_DANFOSS_CONTROL_JOG               = 8,
};

class VsdDanfoss : public Vsd
{
public:
  VsdDanfoss();
  virtual ~VsdDanfoss();

  void initModbusParameters();
  void initParameters();

  void init();
  bool isConnect();
  void setLimitsCcsParameters();

  // ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ДВИГАТЕЛЯ
  int setMotorType(float value);
  int setMotorTypeProfile();
  int setMotorCurrent(float value, EventType eventType = NoneType);

  /*!
   * \brief setLimitsMotor функция задания пределов параметров двигателя
   */
  void setLimitsMotor();

  /*!
   * \brief setLimitsMotorCurrent
   */
  void setLimitsMotorCurrent();

  /*!
   * \brief setLimitsMotorVoltage
   */
  void setLimitsMotorVoltage();

  /*!
   * \brief setMotorVoltage функция задания номинального напряжения двигателя
   * \param value паспортное номинальное значение напряжения двигателя
   * \param coef коэффициент трансформации, т.к. ЧРП не знает о существовании
   * трансформатора, в ЧРП записываем value / coef
   * \param eventType флаг формирования сообщения
   * \return
   */
  int setMotorVoltage(float value, float coef, EventType eventType);

  int setMotorResistanceStator(float value);
  int setCurrentLim(float curLimit, float nomCurMtr, float coefTrans);
  void setMotorConfig();
  int setSumInduct(float value);

  // ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ЧРП
  int setVsdControl(float value);
  int setMinFrequency(float value);
  int setMaxFrequency(float value);
  int setFrequency(float value, EventType eventType = NoneType);
  int setTimeSpeedUp(float value); 
  int setTimeSpeedDown(float value);
  int setSwitchingFrequency(float value);
  int setSwitchingFrequencyCode(float value);
  int calcFiltTimeCurLim(float value);

  int setOutFilter(float value);

  // НАСТРОЙКА U/f
  int setUf_f1(float value);
  int setUf_f2(float value);
  int setUf_f3(float value);
  int setUf_f4(float value);
  int setUf_f5(float value);
  int setUf_f6(float value);
  int setUf_U1(float value);
  int setUf_U2(float value);
  int setUf_U3(float value);
  int setUf_U4(float value);
  int setUf_U5(float value);
  int setUf_U6(float value);
  void setUf();

  int setProtCurrentMotorTripSetpoint(float value);

  int calcUfCharacteristicU(float value);
  int calcUfCharacteristicF(float value);
  void readUfCharacterictic();

  float checkAlarmVsd();
  float checkAlarmVsdUnderVoltage();
  float checkAlarmVsdCurrentMotor();

  bool checkPreventVsd();
  float checkWarningVsd();

  void getNewValue(uint16_t id);
  uint8_t setNewValue(uint16_t id, float value, EventType eventType = AutoType);

  void writeToDevice(int id, float value);
  void readInDevice(int id);

  int start(bool init);
  int stop(bool isAlarm);

  bool checkStart();
  bool checkStop();
  bool isSetPointFreq();

  int resetSetpoints();
  void processingRegimeRun();

  void resetAdaptationVector(uint16_t type);
  void setAdaptationVector();
  bool checkSetAdaptationVector();
  bool checkResetAdaptationVector();

  void getConnect();
  void resetConnect();
  int onProtConnect();
  int offProtConnect();

  bool isControl();
private:
  ModbusParameter modbusParameters_[230];
  DeviceModbus *dm_;

  RegimeRunPush *regimeRunPush_;
  RegimeRunSwing *regimeRunSwing_;
  RegimeRunAdaptationVector *regimeRunAdaptationVector_;
  RegimeRunPickup *regimeRunPickup_;
  RegimeRunSkipResonantFreq *regimeRunSkipResonantFreq_;
  RegimeRunSynchron *regimeRunSynchron_;
};

#endif // VSDDANFOSS_H
