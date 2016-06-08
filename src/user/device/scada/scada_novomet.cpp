#include "scada_novomet.h"
#include "user_main.h"

ScadaNovomet::ScadaNovomet()
{
  countParameters_ = 172;
  initParameters();
}

ScadaNovomet::~ScadaNovomet()
{

}

void ScadaNovomet::calcParamsTask()
{
  while (1) {
    osDelay(100);

    for (int i = 0; i < countParameters_; ++i) {
      if (scadaParameters_[i].id > 0) {
        scadaParameters_[i].value.float_t = parameters.get(scadaParameters_[i].id);
      }
    }

    // 3
    uint16_t value = 0;
    if (ksu.isWorkMotor())
      value = 1;
    if (ksu.isDelay())
      value = 2;
    if (!ksu.isWorkMotor())
      value = 3;
    if (ksu.isRestart())
      value = 8;
    if (ksu.isBlock())
      value = 16;
    scadaParameters_[3].value.float_t = value;

    // 4
//    value = 0;
//    if (ksu.isStopMotor()) {
//      int reason = parameters.get(CCS_LAST_STOP_REASON);
//      int reasonVsd = parameters.get(CCS_VSD_ALARM_CODE);
//      switch (reason) {
//      case LastReasonStopOperator:
//        value = 0; break;
//      case LastReasonStopProgram:
//        value = 1; break;
//      case LastReasonStopRemote:
//        value = 2; break;
//      case LastReasonStopOverVoltIn:
//        value = 3; break;
//      case LastReasonStopUnderVoltIn:
//        value = 4; break;
//      case LastReasonStopImbalanceVoltIn:
//        value = 5; break;


//      case LastReasonStopResistIsolation:
//        value = 7; break;
//      case LastReasonStopUnderloadMotor:
//        value = 5; break;
//      case LastReasonStopOverloadMotor:
//        value = 4; break;


//      case LastReasonStopHackSu:
//        value = 8; break;
//      case LastReasonStopManometr:
//        value = 9; break;
//      case LastReasonStopImbalanceCurMotor:
//        value = 6; break;


//      case LastReasonStopMinAnalog1:
//        value = 23; break;
//      case LastReasonStopMinAnalog2:
//        value = 24; break;
//      case LastReasonStopMinAnalog3:
//        value = 25; break;
//      case LastReasonStopMinAnalog4:
//        value = 26; break;
//      case LastReasonStopMaxAnalog1:
//        value = 34; break;
//      case LastReasonStopMaxAnalog2:
//        value = 35; break;
//      case LastReasonStopMaxAnalog3:
//        value = 36; break;
//      case LastReasonStopMaxAnalog4:
//        value = 37; break;

//      case LastReasonStopCurrentMotor:
//        value = 10; break;

//      case LastReasonStopHardwareVsd:
//        switch (reasonVsd) {
//        case VSD_NOVOMET_ALARM_I_LIMIT_FAST:
//        case VSD_ETALON_ALARM_CURRENT_LIMIT:
//          value = 6; break;
//        default:
//          value = 19; break;
//        }
//        break;
//      default:
//        if (reason)
//          value = 46;
//        break;
//      }
//    }
//    scadaParameters_[4].value.float_t = value;

//    7
//    Перегруз
//    8
//    Недогруз
//    9
//    Дисбаланс токов ПЭД
//    10
//    Невыход на заданную частоту
//    11
//    Снижение сопротивления изоляции
//    12
//    Снижение давления на приёме насоса
//    13
//    Снижение расхода
//    14
//    Превышение давления на выкиде насоса
//    15
//    Перегрев ПЭД
//    16
//    Превышение максимального уровня вибрации
//    17
//    Превышение числа допустимых пусков за период
//    18
//    Открытие двери силового отсека
//    20
//    Сигнал на дискретном входе 1 клеммника внешних подключений
//    21
//    Сигнал на дискретном входе 2 клеммника внешних подключений
//    22
//    Сигнал на дискретном входе 3 клеммника внешних подключений
//    23
//    Сигнал на дискретном входе 4 клеммника внешних подключений
//    24
//    Дисбаланс входных токов
//    25
//    Выход из синхронизма вентильного ПЭД
//    26
//    Сигнал на аналоговом входе 1 клеммника внешних подключений
//    27
//    Сигнал на аналоговом входе 2 клеммника внешних подключений
//    28
//    Сигнал на аналоговом входе 3 клеммника внешних подключений
//    29
//    Сигнал на аналоговом входе 4 клеммника внешних подключений
//    30
//    Превышение предела тока ПЭД
//    31
//    Снижение уровня на приёме насоса
//    32
//    Перегрев входного фильтра
//    33
//    Потеря связи между контроллером и ПЧ
  }
}

int ScadaNovomet::setNewValue(ScadaParameter *param)
{
  if (param->id > 0) {
    return parameters.set(param->id, param->value.float_t, RemoteType);
  }

  //346-348, 634-636
  if (((param->address >= 346) && (param->address <= 348)) ||
      ((param->address >= 634) && (param->address <= 636))) {
    unTypeData value;
    value.uint32_t = param->value.float_t;
    time_t time = parameters.getU32(CCS_DATE_TIME);
    tm dateTime = *localtime(&time);

    switch (param->address) {
    case 634: case 346:
      dateTime.tm_year = value.char_t[1] + 100;
      if (value.uint16_t[0] > 0)
        dateTime.tm_mon = value.char_t[0] - 1;
      break;
    case 635: case 347:
      dateTime.tm_mday = value.char_t[1];
      if(dateTime.tm_mday == 0)
        dateTime.tm_mday = 1;
      dateTime.tm_hour = value.char_t[0];
      break;
    case 636: case 348:
      dateTime.tm_min = value.char_t[1];
      dateTime.tm_sec = value.char_t[0];
      break;
    }

    time = mktime(&dateTime);
    parameters.set(CCS_DATE_TIME, (uint32_t)time, RemoteType);
    return ok_r;
  }

  return err_r;
}
