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
 * \brief Флаги статусных регистров инвертора
 */
enum enStatus {
  // STATUS_WORD_1
  VSD_STATUS_STARTED              = 0,      //!< Запуск ПЧ
  VSD_STATUS_WAIT_RECT_START      = 1,      //!< Ожидаем запуска выпрямителя
  VSD_STATUS_STOPPED_REGISTER     = 2,      //!< Инвертор остановлен по изменению важного параметра
  VSD_STATUS_STOPPED_EXTERNAL     = 3,      //!< Инвертор остановлен по команде извне
  VSD_STATUS_WAIT_RECT_STOP       = 4,      //!< Ожидание остнова выпрямителя
  VSD_STATUS_FAULT_STOPPED        = 5,      //!< Остановлен по причине FAULT
  VSD_STATUS_RIGHT_DIRECTION      = 6,      //!< Правое направление вращения
  VSD_STATUS_I_LIMIT              = 7,      //!< Токоограничение
  VSD_STATUS_ULOW                 = 8,      //!< Недостаточно напряжения
  VSD_STATUS_STOPPED_ALARM        = 9,      //!< Остановлен аварийно
  VSD_STATUS_UD_LOW_FAULT         = 10,     //!< Остановлен по снижению напряжения на шин
  VSD_STATUS_UD_HIGH_FAULT        = 11,     //!< Остановлен по превышению напряжения на шине
  VSD_STATUS_TO_STOP_MODE         = 12,     //!< Режим плавной остановки двигателя
  VSD_STATUS_UIN_ASYM             = 13,     //!< Остановлен по несимметрии входного напряжения
  VSD_STATUS_URECT_SHORTCIRCUIT   = 14,     //!< Остановлен по КЗ от выпрямителя
  VSD_STATUS_RESERVED             = 15,     //!< Резерв
  // STATUS_WORD_2
  VSD_STATUS_FC_I2T_ERR           = 16,     //!< Сработала токовая тепловая защита инвертора
  VSD_STATUS_AST_ERR              = 17,     //!< Система автонастройки не смогла определить параметры линии
  VSD_STATUS_I_LIMIT_FAST         = 18,     //!< Превышение порога мгновенного токоограничения
  VSD_STATUS_CURRENT_OPT          = 19,     //!< Включена оптимизация по току
  VSD_STATUS_POWER_OPT            = 20,     //!< Включена оптимизация по выходной мощности
  VSD_STATUS_OPT_DONE             = 21,     //!< Оптимизация состоялась
  VSD_STATUS_M_TYPE0              = 22,     //!< Бит типа двигателя 0
  VSD_STATUS_M_TYPE1              = 23,     //!< Бит типа двигателя 1
  VSD_STATUS_DISCHARGE_ON         = 24,     //!< Задействован режим разряда шины
  VSD_STATUS_DISCAHRGE            = 25,     //!< Режим разряда шины работает
  VSD_STATUS_DISCHARGE_ERR        = 26,     //!< Ошибка режима разряда шины
  VSD_STATUS_VC_ERR               = 27,     //!< Ошибка векторного режима
  VSD_STATUS_I_FAST_ERR           = 28,     //!< Быстрая токовая защита двигателя
  VSD_STATUS_M_I2T_ERR            = 29,     //!< Токовая защита двигателя (перегруз)
  VSD_STATUS_OWERPWM1             = 30,     //!< OverPWM тип 1
  VSD_STATUS_OWERPWM2             = 31,     //!< OverPWM тип 2
  // STATUS_WORD_3
  VSD_STATUS_3                    = 32,
  // STATUS_WORD_7
  VSD_STATUS_IMAX                 = 48,     //!< Превышен максимальный ток инвертора
  VSD_STATUS_IZ                   = 49,     //!< Детектирован разностный ток выходных фаз (утечка)
  VSD_STATUS_AN_MON               = 50,     //!< Ошибка монитора питания  аналоговой цепи контроллера
  VSD_STATUS_CTR_MON              = 51,     //!< Ошибка монитора питания цифровой цепи контроллера
  VSD_STATUS_CLK_MON              = 52,     //!< Ошибка монитора питания часов контроллера
  VSD_STATUS_MB_MON               = 53,     //!< Ошибка монитора платы измерений
  VSD_STATUS_DRV0                 = 54,     //!< Ошибка драйвера 0
  VSD_STATUS_DRV1                 = 55,     //!< Ошибка драйвера 1
  VSD_STATUS_DRV2                 = 56,     //!< Ошибка драйвера 2
  VSD_STATUS_TEST                 = 57,     //!< Включен тестовый режим контроллера ПЧ
  // STATUS_WORD_5
  VSD_STATUS_ABC_STATE            = 64,     //!< Направление чередования фаз
  VSD_STATUS_1                    = 65,
  VSD_STATUS_ERR_STATE            = 66,     //!< Ошибка синхронизации с сетью
  VSD_STATUS_CHARGE_STATE         = 67,     //!< Производится заряд ёмкости инвертора
  VSD_STATUS_STARTED_STATE        = 68,     //!< Ёмкость заряжена
  VSD_STATUS_SYNC_STATE           = 69,     //!< Первая попытка синхронизации
  VSD_STATUS_BOUNCE_WAIT          = 70,     //!< Ожидание времени антидребезга
  VSD_STATUS_ERR_SHORTCIRQUIT     = 71,     //!< Ошибка по КЗ на инверторе
  // STATUS_WORD_4
  VSD_STATUS_READY                = 80,     //!< Готов
  VSD_STATUS_UNDERLOAD            = 81,     //!< Недогруз
  VSD_STATUS_RESISTANCE           = 82,     //!< Низкое Rиз
  VSD_STATUS_UNDERVOLTAGE         = 83,     //!< Низкое напряжение
  VSD_STATUS_OVERVOLTAGE          = 84,     //!< Высокое напряжение
  VSD_STATUS_RUN_COUNT            = 85,     //!< Прев. кол-во пусков
  VSD_STATUS_OVERHEAT_FILTER      = 86,     //!< Перегрев фильтра
  VSD_STATUS_MONOMETR             = 87,     //!< Конт манометр
  VSD_STATUS_OVERHEAT_MOTOR       = 88,     //!< Выс. Температура
  VSD_STATUS_OVERVIBRATION        = 89,     //!< Выс. Вибрация
  VSD_STATUS_PRESSURE             = 90,     //!< Низ. Давление
  VSD_STATUS_PRESSURE_Z           = 91,     //!< Низкое Pзатр
  VSD_STATUS_AI_0                 = 92,     //!< Доп. аналог вход 0
  VSD_STATUS_TURBINE              = 93,     //!< Турбинное вращение
  VSD_STATUS_ERR_19               = 94,     //!< Ошибка 19
  VSD_STATUS_ERR_31               = 95,     //!< Ошибка 31

  VSD_STATUS_NO_CONNECT           = 250,     //!< Нет связи с ЧРП
};

/*!
 * \brief The enControl enum
 * Флаги регистра управления ПЧ
 */
enum enControl
{
  VSD_CONTROL_START           = 1,       //!< Запустить инвертор ("Старт")
  VSD_CONTROL_STOP            = 2,       //!< Остановить инвертор ("Стоп")
  VSD_CONTROL_LEFT_DIRECTION  = 4,       //!< Задать левое вращение (обратное)
  VSD_CONTROL_RIGHT_DIRECTION = 8,       //!< Задать правое вращение (прямое)
  VSD_CONTROL_ALARM           = 16,      //!< Аварийный останов
  VSD_CONTROL_CURRENT_OPT     = 32,      //!< Задание оптимизации по току
  VSD_CONTROL_POWER_OPT       = 64,      //!< задание оптимизации по мощности
  VSD_CONTROL_CLEAR_OPT       = 128,     //!< Отключить оптимизацию
  VSD_CONTROL_ASYN_MOTOR      = 256,     //!< Асинхронный
  VSD_CONTROL_VENT_MOTOR      = 512,     //!< Вентильный
  VSD_CONTROL_OVERPWM1_ON     = 1024,    //!< Включить режим OverPWM1
  VSD_CONTROL_OVERPWM2_ON     = 2048,    //!< Включить режим OverPWM2
  VSD_CONTROL_OVERPWM_OFF     = 4096,    //!< Выключить режим OverPWM
  VSD_CONTROL_DISCHARGE_ON    = 8192,    //!< Включить предразряд шины инвертора
  VSD_CONTROL_DISCHARGE_OFF   = 16384,   //!< Выключить предразряд шины инвертора
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
   * \param value - задаваемая частота
   * \return 0 - задание успешно, № - ошибки
   */
  int setFrequency(float value);

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
  int setSwitchingFrequency(float value);

  /*!
   * \brief setSwitchingFrequencyCode
   * \param value
   * \return
   */
  int setSwitchingFrequencyCode(float value);

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
   * \brief Метод задания предела тока
   * \return Код результата операции
   */
  int setCurrentLim(float value);

  /*!
   * \brief Функция записи в регистр суммарной индуктивности
   * \param induct - задаваемая суммарная индуктивности
   * \return 0 - задание успешно, № - ошибки
   */
  int setSumInduct(float induct);


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
  virtual bool checkStatusVsd(uint8_t bit);

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
   * \brief resetSetpoints
   * \return
   */
  virtual int resetSetpoints();

  /*!
   * \brief Функция вычисления текущего cos ЧРП по активной и полной мощности
   * \return
   */
  int calcVsdCos();

  /*!
   * \brief writeToDevice
   */
  virtual void writeToDevice(int id, float value);

  VsdLog *log() const { return log_; }

protected:
  VsdLog *log_;

private:
  //! Массив параметров устройства
  parameter parametersArray_[VSD_END - VSD_BEGIN];

};

#endif /* VSD_H_ */
