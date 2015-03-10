/*
 * vsd_novomet.h
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#ifndef VSD_NOVOMET_H_
#define VSD_NOVOMET_H_

#include "vsd.h"
#include "device_modbus.h"

/*!
 * \brief The enInvertorControl enum
 * Флаги регистра управления ПЧ
 */
enum enInvertorControl
{
  /// Запустить инвертор ("Старт")
  INV_CONTROL_START           = 1,
  /// Остановить инвертор ("Стоп")
  INV_CONTROL_STOP            = 2,
  /// Задать левое вращение
  INV_CONTROL_LEFT_DIRECTION  = 4,
  /// Задать правое вращение
  INV_CONTROL_RIGHT_DIRECTION = 8,
  /// Аварийный останов
  INV_CONTROL_ALARM           = 16,
  /// Задание оптимизации по току
  INV_CONTROL_CURRENT_OPT     = 32,
  /// задание оптимизации по мощности
  INV_CONTROL_POWER_OPT       = 64,
  /// Отключить оптимизацию
  INV_CONTROL_CLEAR_OPT       = 128
};

/*!
 * \brief The enInvertorStatus enum
 * Флаги регистра статуса инвертора
 */
enum enInvertorStatus
{
  /// Запуск ПЧ
  INV_STATUS_STARTED          = 1,
  /// Ожидаем запуска выпрямителя
  INV_STATUS_WAIT_RECT_START  = 2,
  /// Инвертор остановлен по изменению важного параметра
  INV_STATUS_STOPPED_REGISTER = 4,
  /// Инвертор остановлен по команде извне
  INV_STATUS_STOPPED_EXTERNAL = 8,
  /// Ожидание остнова выпрямителя
  INV_STATUS_WAIT_RECT_STOP   = 16,
  /// Остановлен по причине FAULT
  INV_STATUS_FAULT_STOPPED    = 32,
  /// Правое направление вращения
  INV_STATUS_RIGHT_DIRECTION  = 64,
  /// Токоограничение
  INV_STATUS_I_LIMIT          = 128,
  /// Недостаточно напряжения
  INV_STATUS_ULOW             = 256,
  /// Остановлен аварийно
  INV_STATUS_STOPPED_ALARM    = 512,
  /// Остановлен по снижению напряжения на шине
  INV_STATUS_UD_LOW_FAULT     = 1024,
  /// Остановлен по превышению напряжения на шине
  INV_STATUS_UD_HIGH_FAULT    = 2048,
  /// Режим плавной остановки двигателя
  INV_STATUS_TO_STOP_MODE     = 4096,
  /// Остановлен по несимметрии входного напряжения
  INV_STATUS_UIN_ASYM         = 8192,
  /// Остановлен по КЗ от выпрямителя
  INV_STATUS_URECT_SHORTCIRCUIT = 16384,
  /// Резерв
  INV_STATUS_RESERVED         = 32768
};

/*!
 * \brief The enInvertorExtStatus enum
 * Флаги расширенного регистра статуса инвертора
 */
enum enInvertorExtStatus
{
  /// Сработала тепловая защита
  INV_EXT_STATUS_I_RMS        = 1,
  /// Система автонастройки не смогла определить параметры линии
  INV_EXT_STATUS_AST_ERR      = 2,
  /// Превышение порога мгновенного токоограничения
  INV_EXT_STATUS_I_LIMIT_FAST = 4,
  /// Включена оптимизация по току
  INV_EXT_STATUS_CURRENT_OPT  = 8,
  /// Включена оптимизация по выходной мощности
  INV_EXT_STATUS_POWER_OPT    = 16
};

class VsdNovomet: public Vsd
{
public:
  VsdNovomet();
  virtual ~VsdNovomet();

  void initModbusParameters();
  void initParameters();
  void getNewValue(uint16_t id);

  uint8_t setNewValue(uint16_t id, float value);

  /*!
   * \brief Метод проверки флага в регистре статуса инвертора
   * \param flag проверяемый флаг
   * \return 0 - флаг установлен 1 - не установлен
   */
  int checkInvertorStatus(uint16_t flag);

  /*!
   * \brief Метод запуска ЧРП Новомет
   * Для управления запусками и остановами, и для контроля состояния ПЧ
   * используется регистр управления IREG_INVERTOR_CONTROL и два регистра
   * состояния IREG_INVERTOR_STATUS и IREG_INVERTOR_EXT_STATUS.
   * Пуск осуществляется установкой в "1" бита 0 ("старт") в регистре управления
   * ПЧ IREG_INVERTOR_CONTROL через внешний интерфейс MODBUS, либо по
   * внутренней CAN шине с помощью пульта управления ПЧ.
   * После получения команды "старт" в случае, если включен режим остановки
   * турбинного вращения для ВД, перед процедурой запуска двигателя
   * производится разряд шины инвертора.
   * Далее контроллер ПЧ устанавливает биты INV_STATUS_STARTED и
   * INV_STATUS_WAIT_RECT_START регистра управления и ждет подтверждения
   * заряда конденсаторов DC фильтра от платы управления выпрямителем
   * (выделенный сигнал оптоволоконной линии).
   * После получения подтверждения бит INV_STATUS_WAIT_RECT_START
   * снимается, контроллер ПЧ включает инвертор и запускает очередь
   * отработки регуляторов двигателя (торможение турбинного вращения,
   * режим пуска - с раскачкой или толчковый, основной режим управления).
   * \return Код выполнения операции
   */
  int start();

  /*!
   * \brief Метод останова ЧРП Новомет
   * При установке бита 1 ("Стоп") в регистре управления ПЧ контроллер ПЧ
   * устанавливает биты INV_STATUS_STOPPED_EXTERNAL и INV_STATUS_TO_STOP_MODE,
   * и ожидает окончания процедуры плавного останова двигателя.
   * После останова двигателя контроллер выключает инвертор, снимает флаг
   * INV_STATUS_STARTED, устанавливает флаг INV_STATUS_WAIT_RECT_STOP и
   * ждет сигнала подтверждения выключения от платы управления выпрямителем.
   * После получения подтверждения выключения флаг INV_STATUS_WAIT_RECT_STOP
   * снимается, система готова для очередного запуска.
   * \return Код выполнения операции
   */
  int stop();

  bool checkStart();
  bool checkStop();

  /*!
   * \brief Метод записи основного режима работы ЧРП
   * Основных методов 2:
   * U/f регулирование АД и ВД код: 1
   * Векторное управление ВД код: 3
   * Поскольку у ЧРП только один основной алгоритм и он обязан быть,
   * записываем его последним
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
   * \param value - направление вращения 0 прямое (левое),
   * 1 обратное (правое)
   * \return
   */
  int setRotation(uint8_t value);

  /*!
   * \brief Метод задания прямого направления вращения
   * \return
   */
  int setDirectRotation();

  /*!
   * \brief Метод задания обратного направления вращения
   * \return
   */
  int setReverseRotation();

  /*!
   * \brief Метод записи параметра в устройство
   * \param id
   * \param value
   * \return
   */
  int writeParameter(uint16_t id, float value);

  /*!
   * \brief Проверка на "необходимость" работы с параметром
   * \param indexParam
   * \return
   */
  int checkExchangModbusParameters(int indexParam);

private:
  ModbusParameter ModbusParameters[135];
  DeviceModbus *dm_;

};

#endif /* VSD_NOVOMET_H_ */
