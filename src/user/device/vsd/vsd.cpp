/*
 * Vsd.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "vsd.h"

Vsd::Vsd()
{
  // Инициализируем банк параметров ЧРП
  Parameter Parameters[/*VSD_END -  VSD_BEGIN*/] = {};
  /*
  {
    // Идентификатор              Доступ                Операция                Физический            Исп                   Знач         Мин      Макс
    {VSD_INDEX,                 ACCESS_OPERATOR,    OPERATION_LIMITED,    PHYSIC_NUMERIC,        VALIDITY_ERROR,        0.0,        0.0,    5.0},
    {VSD_UNIT_SPEED,            ACCESS_OPERATOR,     OPERATION_LIMITED,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     2.0},         // Единицы измерения скорости
    {VSD_CONFIG_MODE,           ACCESS_OPERATOR,     OPERATION_LIMITED,     PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     3.0},         // Режим конфигурирования
    {VSD_CONTROL_MOTOR,         ACCESS_OPERATOR,     OPERATION_LIMITED,     PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     3.0},         // Принцип управления двигателем
    {VSD_TORQUE_CHARACTERISTIC, ACCESS_OPERATOR,     OPERATION_LIMITED,     PHYSIC_NUMERIC,       VALIDITY_ERROR,        0.0,         0.0,     2.0},         // Характеристика момента нагрузки
    {VSD_OVERLOAD_MODE,         ACCESS_OPERATOR,     OPERATION_LIMITED,     PHYSIC_NUMERIC,       VALIDITY_ERROR,        0.0,         0.0,     1.0},         // Режим перегрузки
    {VSD_TYPE_MOTOR,            ACCESS_OPERATOR,     OPERATION_LIMITED,     PHYSIC_NUMERIC,       VALIDITY_ERROR,        0.0,         0.0,     1.0},         // Конструкция двигателя
    {VSD_DAMPING_GANE,          ACCESS_OPERATOR,     OPERATION_LIMITED,     PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     250.0},        // Усиление демпфирования
    {VSD_LOW_SPEED_FILTER_TIME, ACCESS_OPERATOR,     OPERATION_LIMITED,     PHYSIC_TIME,          VALIDITY_ERROR,     0.0,         0.0,     20.0},         // Время фильтрации при низкой скорости
    {VSD_HIGH_SPEED_FILTER_TIME,ACCESS_OPERATOR,     OPERATION_LIMITED,     PHYSIC_TIME,          VALIDITY_ERROR,     0.0,         0.0,     20.0},         // Время фильтрации при высокой скорости
    {VSD_POWER_MOTOR,           ACCESS_OPERATOR,     OPERATION_LIMITED,     PHYSIC_POWER,        VALIDITY_ERROR,       0.0,         0.0,     1200.0},    // Мощность двигателя
    {VSD_VOLTAGE_MOTOR,         ACCESS_OPERATOR,     OPERATION_LIMITED,     PHYSIC_VOLTAGE,      VALIDITY_ERROR,     0.0,         0.0,     1000.0},    // Напряжение двигателя
    {VSD_FREQUENCY_MOTOR,       ACCESS_OPERATOR,     OPERATION_LIMITED,    PHYSIC_FREQUENCY,      VALIDITY_ERROR,     0.0,         20.0,     1000.0},     // Частота двигателя
    {VSD_CURRENT_MOTOR,         ACCESS_OPERATOR,     OPERATION_LIMITED,    PHYSIC_CURRENT,      VALIDITY_ERROR,     0.0,         0.0,     2000.0},    // Ток двигателя
    {VSD_SPEED_MOTOR,           ACCESS_OPERATOR,     OPERATION_LIMITED,    PHYSIC_RPM,         VALIDITY_ERROR,     0.0,         10.0,     6000.0},     // Скорость двигателя
    {VSD_RATE_TORQUE_MOTOR,     ACCESS_OPERATOR,     OPERATION_LIMITED,    PHYSIC_RATE,           VALIDITY_ERROR,     0.0,         1.0,     10000.0},    // Длительный номинальный момент двигателя
    {VSD_RESISTANCE_STATOR,     ACCESS_OPERATOR,     OPERATION_LIMITED,    PHYSIC_RESISTANCE,  VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Сопротивление статора
    {VSD_D_AXIS_INDUNSTANCE,    ACCESS_OPERATOR,    OPERATION_LIMITED,    PHYSIC_INDUNCTANCE, VALIDITY_ERROR,     0.0,         0.0,     1000.0},     // Индуктивности по оси D
    {VSD_POLES_MOTOR,           ACCESS_OPERATOR,     OPERATION_LIMITED,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         2.0,     10.0},         // Число полюсов двигателя
    {VSD_BACK_EMF,              ACCESS_OPERATOR,     OPERATION_LIMITED,    PHYSIC_VOLTAGE,       VALIDITY_ERROR,     0.0,         10.0,     9000.0},     // Danfoss 1-40 Используется в проекте
    {VSD_UF_CHARACTERISTIC_U,   ACCESS_OPERATOR,     OPERATION_WRITE,    PHYSIC_VOLTAGE,       VALIDITY_ERROR,     0.0,         0.0,     5.0},         // Характеристика Uf - U
    {VSD_UF_CHARACTERISTIC_F,   ACCESS_OPERATOR,     OPERATION_WRITE,    PHYSIC_FREQUENCY,   VALIDITY_ERROR,     0.0,         0.0,     5.0},         // Характеристика Uf - f
    {VSD_RESONANCE_REMOVE,      ACCESS_OPERATOR,    OPERATION_LIMITED,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1.0},         // Подавление резонанса %
    {VSD_RESONANCE_TIME,        ACCESS_OPERATOR,    OPERATION_LIMITED,    PHYSIC_TIME,           VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Постаянная времени подавления
    {VSD_MIN_CURRENT_LOW_SPEED, ACCESS_OPERATOR,    OPERATION_LIMITED,    PHYSIC_CURRENT,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Минимальный ток при низкой скорости
    {VSD_PM_START_MODE,         ACCESS_OPERATOR,    OPERATION_LIMITED,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Danfoss 1-70 Не используется в проекте, только при конфигурировании
    {VSD_START_DELAY,           ACCESS_OPERATOR,    OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Задержка запуска
    {VSD_START_FUNCTION,        ACCESS_OPERATOR,    OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Функция запуска
    // Доделать
    {VSD_STOP_FUNCTION,         ACCESS_OPERATOR,    OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Функция при остановке
    {VSD_STOP_SPEED,            ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Минимальная скорость для функции при останове
    {VSD_CONTROL_TERMISTOR_MTR, ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Контроль термистора двигателя
    {VSD_THERMISTOR_RESOURCE,   ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Вход термистора
    {VSD_PARKING_CURRENT,       ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Начальный ток
    {VSD_PARKING_TIME,          ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Длительность действия начального тока
    {VSD_OVERVOLTAGE_CONTROL,   ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Контроль перенапряжения
    {VSD_MIN_REFERENCE,         ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Минимальное задание
    {VSD_MAX_REFERENCE,         ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Максимальное задание
    {VSD_FREQUENCY,             ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Фиксированная скорость
    {VSD_RESOURCE_TASK_1,       ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Ресурс задания 1
    {VSD_RESOURCE_TASK_2,       ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Ресурс задания 2
    {VSD_RESOURCE_TASK_3,       ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Ресурс задания 3
    {VSD_TYPE_SPEED_CHANGE,     ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Тип изменения скорости
    {VSD_TIMER_DISPERSAL,       ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Время разгона
    {VSD_TIMER_DELAY,           ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Время замедления
    {VSD_TIMER_DISP_FIX_SPEED,  ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Время разгона фиксированной скорости
    {VSD_TIMER_DELAY_FIX_SPEED, ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Время замедления фиксированной скорости
    {VSD_ROTATION,              ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Направления вращения
    {VSD_LOW_LIM_SPEED_MOTOR,   ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Нижний предел скорости
    {VSD_HIGH_LIM_SPEED_MOTOR,  ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Верхний передел скорости
    {VSD_TORQUE_LIMIT,          ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Ограничение момента
    {VSD_TORQUE_LIMIT_GEN,      ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Ограничение момента в режиме генератора
    {VSD_CURRENT_LIMIT,         ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Предел тока
    {VSD_MAX_OUTPUT_FREQUENCY,  ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Максимальная выходная частота
    {VSD_MTR_FEEDBACK_LOSS_FUNC,ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Реакция на ошибку обратной связи
    {VSD_TRACK_ERROR_FUNCTION,  ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Реакция на ошибку задания частоты
    {VSD_TRACK_ERROR,           ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Величина несоответствия частота
    {VSD_TRACK_ERROR_TIMEOUT,   ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Величина несоответствия частоты
    {VSD_TRACK_ERROR_RAMPING,   ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Величина несоответствия частоты при разгоне
    {VSD_TRACK_ERROR_RAMP_TIME, ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Задержка срабатывания при несоответствии частоты при разгоне
    {VSD_TRACK_ERROR_AFTER_RAMP,ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Tracking Error After Ramping Timeout
    {VSD_WARNING_CURRENT_LOW,   ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Danfoss 4-50 Не используется в проекте, только при конфигурировании
    {VSD_WARNING_CURRENT_HIGH,  ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Danfoss 4-51 Не используется в проекте, только при конфигурировании
    {VSD_WARNING_SPEED_LOW,     ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Минимальная скорость
    {VSD_TERMINAL_27_MODE,      ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Режим клеммы 27
    {VSD_TERMINAL_29_MODE,      ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Режим клеммы 29
    {VSD_DI_18,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Цифровой вход 18
    {VSD_DI_19,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Цифровой вход 19
    {VSD_DI_27,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Цифровой вход 27
    {VSD_DI_32,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Цифровой вход 32
    {VSD_DI_33,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Цифровой вход 33
    {VSD_TERMINAL_27_DI,        ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Клемма 27 Цифровой вход
    {VSD_TERMINAL_29_DI,        ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Клемма 29 Цифровой вход
    {VSD_FUNCTION_RELE,         ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Реле функций
    {VSD_ON_DELAY_RELAY,        ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Нипонятно
    {VSD_42_AO,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Клемма 42 аналоговый вход
    {VSD_SL_CONTROLLER_MODE,    ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Режим контроллера SL
    {VSD_SL_START_EVENT,        ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Событие запуска
    {VSD_SL_STOP_EVENT,         ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Событие останова
    {VSD_SL_RESET,              ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Сброс настроек компараторов
    {VSD_SL_10,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,    1000000.0}, // Операнд сравнения компаратора
    {VSD_SL_11,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Оператор сравнения компаратора
    {VSD_SL_12,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Значение компаратора
    {VSD_SL_15,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // RS-FF Operand S
    {VSD_SL_16,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // RS-FF Operand R
    {VSD_SL_20,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Временная задержка
    {VSD_SL_40,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Булева переменная логического соотношения 1
    {VSD_SL_41,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Оператор логического соотношения 1
    {VSD_SL_42,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Булева переменная логического соотношения 2
    {VSD_SL_43,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Logic Rule Operator 2
    {VSD_SL_44,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Logic Rule Boolean 3
    {VSD_SL_51,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Событие контроллера SL
    {VSD_SL_52,                 ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Действие контроллера SL
    {VSD_SWITCHING_FREQUENCY,   ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Частота коммутации
    {VSD_OVERMODULATION,        ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Сверхмодуляция
    {VSD_DEAD_TIME_COMPENSATION,ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Компенсация задержки
    {VSD_MAINS_FAILURE,         ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Отказ питания
    {VSD_MAINS_VOLTAGE_FAILURE, ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,       VALIDITY_ERROR,     0.0,         0.0,     1000000.0}, // Напряжение сети при отказе питания
    {VSD_RESET_MODE,            ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Режим сброса
    {VSD_AUTOSTART_TIME,        ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время автоматического перезапуск
    {VSD_DELAY_CURRENT_LIMIM,   ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Задержка перед выключение при пределе тока
    {VSD_DELAY_TORQUE_LIMIT,    ACCESS_OPERATOR,      OPERATION_WRITE,    PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Задержка перед выключение при пределе моменте
    {VSD_FIL_TIME_CURRENT_LIMIT,ACCESS_OPERATOR, OPERATION_WRITE,    PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время фильтра регулятора тока
    {VSD_COS_PHI_MOTOR,         ACCESS_OPERATOR,      OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Косинус Фи
    {VSD_DC_COMPENSATION,       ACCESS_OPERATOR,      OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Компенсация напряжения постоянного тока
    {VSD_FAN_CONTROL,           ACCESS_OPERATOR,      OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Управление вентилятором
    {VSD_OUT_FILTER,               ACCESS_OPERATOR,      OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Выходной фильтр
    {VSD_FAIL_RESET,               ACCESS_OPERATOR,      OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Сброс аварий
    {VSD_SOFT_VERSION,             ACCESS_OPERATOR,      OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Версия ПО ЧРП
    {VSD_COMMAND_WORD,             ACCESS_OPERATOR,      OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Командное слово
    {VSD_STATUS_WORD,              ACCESS_OPERATOR,      OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Слово состояния
    {VSD_ACTIVE_POWER,          ACCESS_OPERATOR,      OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Активная мощность
    {VSD_OUT_VOLTAGE_MOTOR,     ACCESS_OPERATOR,      OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Напряжение двигателя
    {VSD_CURRENT_FREQUENCY,     ACCESS_OPERATOR,      OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Текущая частота двигателя
    {VSD_OUT_CURRENT_MOTOR,     ACCESS_OPERATOR,      OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Выходной ток средний
    {VSD_CURRENT_SPEED_RPM,     ACCESS_OPERATOR,      OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Текущая скорость вращения двигателя
    {VSD_DC_VOLTAGE,            ACCESS_OPERATOR,      OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Напряжение цепи пост.тока
    {VSD_RADIATOR_TEMPERATURE,  ACCESS_OPERATOR,      OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Температура радиатора
    {VSD_CONTROL_TEMPERATURE,    ACCESS_OPERATOR,      OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Температура платы управления
    {VSD_OUT_CURRENT_PHASE_1,   ACCESS_OPERATOR,      OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Ток двигателя 1
    {VSD_OUT_CURRENT_PHASE_2,   ACCESS_OPERATOR,      OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Ток двигателя 2
    {VSD_OUT_CURRENT_PHASE_3,   ACCESS_OPERATOR,      OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Ток двигателя 3
    {VSD_DI_VSD,                   ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Цифровоый вход ПЧ
    {VSD_LAST_ALARM,               ACCESS_OPERATOR,  OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Номер последней аварии и предупреждения
    {VSD_ALARM_WORD_1,          ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Слово Аварии 1
    {VSD_ALARM_WORD_2,          ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Слово Аварии 2
    {VSD_WARNING_WORD_1,        ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Слово предупреждения 1
    {VSD_WARNING_WORD_2,        ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Слово предупреждения 2
    {VSD_HIGH_START_TORQUE_TIME,ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время применения высокого пускового момента
    {VSD_HIGH_START_TORQUE_CURRENT,ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Пусковой ток в % от номинального
    {VSD_LOCK_ROTOR_PROTECTION, ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Защита блокировки ротора
    {VSD_LOCK_ROTOR_TIME,       ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время задержки аварии блокировки ротора
    {VSD_THYR_ANGLE_REFERENCE,  ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Угол управления тиристорами; служебный регистр для отладки работы выпрямителя
    {VSD_THYR_STATUS,              ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Статус платы выпрямителя. Регистр для внутреннего использования в ПЧ
    {VSD_THYR_CONTROL,             ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Регистр управления тиристорами; используется только контроллером ПЧ
    {VSD_THYR_VOLT_SHORT_CUILT, ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Напряжение защиты от КЗ инвертора.
    {VSD_DOUTPUTS,                ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Маска состояния цифровых выходов.
    {VSD_DINPUTS,                 ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Маска состояния цифровых входов.
    {VSD_DOUTPUTS_ENABLE,          ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Маска включения цифровых выходов.
    {VSD_DOUTPUTS_DISABLE,         ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Маска выключения цифровых выходов
    {VSD_VENT_ON_TEMPERATURE,   ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Температура включения главного вентилятора.
    {VSD_VENT_OFF_TEMPERATURE,  ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Температура выключения главного вентилятора.
    {VSD_VENT_PERIOD,              ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Период проверки температуры радиаторов
    {VSD_VENT_TEMPERATURE_FAULT,ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Температура срабатывания тепловой защиты радиаторов
    {VSD_INVERTOR_CONTROL,      ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Флаговый регистр управления ПЧ
    {VSD_INVERTOR_STATUS,       ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Регистр состояния инвертора
    {VSD_FREQUENCY_ERROR,       ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Уставка частоты "тревожного" режима
    {VSD_T_SPEEDUP,               ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Период нарастания частоты.
    {VSD_T_SPEEDDOWN,           ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Период Период снижения частоты
    {VSD_T_ILIMIT_SPEEDDOWN,    ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Период снижения частоты при токоограничении.
    {VSD_T_ULOW_SPEEDDOWN,        ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Период снижения частоты при нехватке напряжения на инверторе
    {VSD_CURRENT_REGULATOR,     ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Тип текущего алгоритма управления (регулятора)
    {VSD_UF_U_FORCE,             ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Напряжение форсировки
    {VSD_UF_F_FORCE,             ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Частота форсировки
    {VSD_UF_UHH,                  ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Расчетное напряжение холостого хода.
    {VSD_IFB_COMP,                 ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Постоянная времени компенсации индуктивности
    {VSD_VUOUT_PROP,            ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Пропорциональный коэффициент ОС частотного контура регулирования
    {VSD_ILIMIT,                 ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Максимальный порог ограничения тока на выходе ПЧ, абс. вел
    {VSD_VUOUT_INTEG,           ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Интегральный коэффициент ОС частотного контура регулирования
    {VSD_VTOUF_INTEG,              ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Коэффициент постоянной времени обратного перехода с вектора на U/f
    {VSD_UOUT_RATED,            ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Текущее выходное напряжение ПЧ
    {VSD_LOUT,                    ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Суммарная индуктивность на выходе ПЧ, фазное значение
    {VSD_SW_STARTUP_FREQUENCY,  ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Режим раскачки: частота
    {VSD_SW_STARTUP_ANGLE_OSC,  ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Режим раскачки: угол качания
    {VSD_SW_STARTUP_ANGLE_SHIFT,ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Режим раскачки: угол сдвига
    {VSD_SW_STARTUP_OSC_COUNT,  ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Режим раскачки: количество качаний на угол
    {VSD_SW_STARTUP_ROTATIONS,  ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Режим раскачки: количество оборотов двигателя
    {VSD_SW_STARTUP_U_PULSE,    ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Режим раскачки: кратность напряжения импульса расклинки
    {VSD_SW_STARTUP_I_LIM,      ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Режим раскачки: токоограничение
    {VSD_VFREQ_INTEG,              ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Интегральный коэффициент ОС фазового контура регулирования
    {VSD_REGULATOR_QUEUE_1,     ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Регистры рабочей очереди алгоритмов управления
    {VSD_REGULATOR_QUEUE_2,     ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Регистры рабочей очереди алгоритмов управления
    {VSD_REGULATOR_QUEUE_3,     ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Регистры рабочей очереди алгоритмов управления
    {VSD_REGULATOR_QUEUE_4,     ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Регистры рабочей очереди алгоритмов управления
    {VSD_REGULATOR_QUEUE_5,     ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Регистры рабочей очереди алгоритмов управления
    {VSD_UD_LOW_FAULT,          ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Порог защиты ПЧ по снижению напряжения на инверторе
    {VSD_UD_HIGH_FAULT,         ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Порог защиты ПЧ по превыщению напряжения на инверторе
    {VSD_UIN_ASYM_LEVEL,        ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Порог защиты ПЧ по несимметрии входного напряжения на инверторе
    {VSD_UIN_ASYM_DELAY,           ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время срабатывания защиты ПЧ по несимметрии входного напряжения на инверторе
    {VSD_F_VECT,                 ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Частота перехода на векторный алгоритм
    {VSD_VFREQ_GAIN,               ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Пропорциональный коэффициент ОС фазового контура регулирования
    {VSD_VANGLE_GAIN,           ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Фазовый коэффициент усиления системы подавления резонанса
    {VSD_RES_TIMECONST,         ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Постоянная времени системы подавления резонанса
    {VSD_RES_IAMP_VALID,        ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Служебный регистр для отладки алгоритмов управления
    {VSD_IB,                      ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Выходной ток ПЧ по фазе В. (амплитудное значение)
    {VSD_PHB,                      ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Фаза выходного тока В
    {VSD_IC,                      ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Выходной ток ПЧ по фазе В. (амплитудное значение)
    {VSD_PHC,                      ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Фаза выходного тока В
    {VSD_SW_STARTUP_I_LIM_PULSE,ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Порог токоограничения импульса расклинки в режиме с раскачкой
    {VSD_DECEL_SPEEDDOWN,       ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время снижения уставки в режиме остановки турбинного вращения
    {VSD_DECEL_VOLTAGE,         ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Напряжение остаточного напряжения на шине (окончания разряда) в режиме остановки турбинного вращения (для ВД)
    {VSD_DECEL_F_MAX,              ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Начальная частота режима остановки турбинного вращения (для АД)
    {VSD_TIME_SECOND,           ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время секунды
    {VSD_TIME_MINUTE,           ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время минуты
    {VSD_TIME_HOUR,                ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время часы
    {VSD_TIME_DAY,                 ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время дни
    {VSD_TIME_MONTH,               ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время месяцы
    {VSD_TIME_YEAR,               ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время годы
    {VSD_AST_IMEAS,             ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Величина измерительного тока автонастройки
    {VSD_AST_UMEAS,             ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Величина измерительного напряжения автонастройки
    {VSD_AST_LOUT_1_0,          ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Измеренная величина выходной индуктивности автонастройки
    {VSD_AST_STANDING,             ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Дополнительный регистр автонастройки, зарезервирован
    {VSD_INVERTOR_EXT_STATUS,   ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Расширенный регистр состояния инвертора
    {VSD_JARRING_PERIOD,        ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Период экскурсий частоты режима встряхивания.
    {VSD_JARRING_UPDFREQ,       ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Величина прибавки частоты вращения выше уставки в режиме встряхивания
    {VSD_JARRING_UPTIME,        ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время прибавки частоты вращения выше уставки в режиме встряхивания
    {VSD_JARRING_DOWNDFREQ,     ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Величина уменьшения частоты вращения ниже уставки в режиме встряхивания
    {VSD_JARRING_DOWNTIME,      ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Время уменьшения частоты вращения ниже уставки в режиме встряхивания
    {VSD_JARRING_MODE,          ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Регистры обслуживания внутренних нужд режима встряхивания
    {VSD_JARRING_TICK_CNT,      ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Регистры обслуживания внутренних нужд режима встряхивания
    {VSD_JARRING_SECOND_CNT,    ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Регистры обслуживания внутренних нужд режима встряхивания
    {VSD_START_FREQ,               ACCESS_OPERATOR,  OPERATION_WRITE,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Стартовая частота вращения двигателя в основном режиме регулирования
    {VSD_IA,                      ACCESS_OPERATOR,  OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Выходной ток ПЧ по фазе A. (амплитудное значение)
    {VSD_PHA,                      ACCESS_OPERATOR,  OPERATION_READ,PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}, /// Фаза выходного тока A
    {VSD_PHOUT,                   ACCESS_OPERATOR,  OPERATION_READ,    PHYSIC_NUMERIC,   VALIDITY_ERROR, 0.0, 0.0, 100000.0}  /// Усредненная фаза выходного тока ПЧ
   };
   */
}

Vsd::~Vsd()
{
  // TODO Auto-generated destructor stub
}

