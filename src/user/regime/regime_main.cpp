#include "regime_main.h"

#define COUNT_RUN_REGIMES  7                  //!< Количество пусковых режимов, увеличивать при добавлении нового
#define COUNT_REGIMES 7                       //!< Количество технологических режимов, увеличивать при добавлении нового


const uint16_t runRgmMode[COUNT_RUN_REGIMES] = {     //!<
  CCS_RGM_RUN_PUSH_MODE,
  CCS_RGM_RUN_SWING_MODE,
  CCS_RGM_RUN_PICKUP_MODE,
  CCS_RGM_RUN_AUTO_ADAPTATION_MODE,
  CCS_RGM_RUN_SKIP_RESONANT_MODE,
  CCS_RGM_RUN_SYNCHRON_MODE,
  CCS_RGM_RUN_DIRECT_MODE
};

const uint16_t runRgmState[COUNT_RUN_REGIMES] = {     //!<
  CCS_RGM_RUN_PUSH_STATE,
  CCS_RGM_RUN_SWING_STATE,
  CCS_RGM_RUN_PICKUP_STATE,
  CCS_RGM_RUN_AUTO_ADAPTATION_STATE,
  CCS_RGM_RUN_SKIP_RESONANT_STATE,
  CCS_RGM_RUN_SYNCHRON_STATE,
  CCS_RGM_RUN_DIRECT_STATE
};

const uint16_t workRgmMode[COUNT_REGIMES][COUNT_REGIMES+1] = {
  {CCS_RGM_PERIODIC_MODE,           1,  0,  1,  0,  0,  0, 0},
  {CCS_RGM_CHANGE_FREQ_MODE,        0,  1,  0,  0,  0,  0, 0},
  {CCS_RGM_MAINTENANCE_PARAM_MODE,  1,  0,  1,  0,  0,  0, 0},
  {CCS_RGM_ALTERNATION_FREQ_MODE,   0,  0,  0,  1,  0,  0, 0},
  {CCS_RGM_OPTIM_VOLTAGE_MODE,      0,  0,  0,  0,  1,  0, 0},
  {CCS_RGM_JARRING_MODE,            0,  0,  0,  0,  0,  1, 0},
  {CCS_RGM_PUMP_GAS_MODE,           0,  0,  0,  0,  0,  0, 1}
};

Regime *regimes[COUNT_REGIMES];               //!< Количество режимов, увеличивать при добавлении нового

RegimeTechnologPeriodic regimeTechnologPeriodic;
RegimeTechnologSoftChangeFreq regimeTechnologSoftChangeFreq;
RegimeTechnologMaintenanceParam regimeTechnologMaintenanceParam;
RegimeTechnologAlternationFreq regimeTechnologAlternationFreq;
RegimeTechnologOptimizationVoltage regimeTechnologOptimizationVoltage;
RegimeTechnologJarring regimeTechnologJarring;
RegimeTechnologPumpingGas regimeTechnologPumpingGas;


static void regimeTask(void *argument);

void regimeInit()
{
  regimes[0] = &regimeTechnologPeriodic;
  regimes[1] = &regimeTechnologSoftChangeFreq;
  regimes[2] = &regimeTechnologMaintenanceParam;
  regimes[3] = &regimeTechnologAlternationFreq;
  regimes[4] = &regimeTechnologOptimizationVoltage;
  regimes[5] = &regimeTechnologJarring;
  regimes[6] = &regimeTechnologPumpingGas;

  osThreadDef(Regimes, regimeTask, osPriorityNormal, 0, 4 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Regimes), NULL);
}

void regimeTask(void *argument)
{
  (void)argument;

  while (1) {
    osDelay(100);

    vsd->processingRegimeRun();
    setGeneralStateRunMode();

    for (int i = 0; i < COUNT_REGIMES; ++i) {
      regimes[i]->processing();
    }  

  }
}

bool interceptionStartRegime()
{
  if (ksu.isProgramMode() && (parameters.get(CCS_RGM_PERIODIC_MODE) != Regime::OffAction)) {
    if ((parameters.get(CCS_RGM_PERIODIC_STATE) == Regime::WorkState) ||
        (parameters.get(CCS_RGM_PERIODIC_STATE) == Regime::PauseState)) {
      return false;
    }
  }

  // Перехватываем запуск режимом прямого пуска
  if (parameters.get(CCS_RGM_RUN_DIRECT_MODE) != Regime::OffAction) {
    if (parameters.get(CCS_RGM_RUN_DIRECT_STATE) < Regime::WorkState) {
      return false;
    }
  }

  if (parameters.get(CCS_RGM_RUN_PICKUP_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
    case VSD_TYPE_ETALON:
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) < Regime::WorkState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_PICKUP_STATE) < Regime::WorkState) {
        return false;
      }
      break;
    }
  }

  if (parameters.get(CCS_RGM_RUN_PUSH_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
    case VSD_TYPE_ETALON:
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) < Regime::WorkState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_PUSH_STATE) < Regime::WorkState) {
        return false;
      }
      break;
    }
  }

  if (parameters.get(CCS_RGM_RUN_SWING_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
    case VSD_TYPE_ETALON:
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) < Regime::WorkState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_SWING_STATE) < Regime::WorkState) {
        return false;
      }
      break;
    }
  }

  if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
    case VSD_TYPE_ETALON:
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) < Regime::WorkState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_STATE) < Regime::WorkState) {
        return false;
      }
      break;
    }
  }

  if (parameters.get(CCS_RGM_RUN_AUTO_ADAPTATION_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) < Regime::WorkState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_AUTO_ADAPTATION_STATE) < Regime::WorkState) {
        return false;
      }
      break;
    }
  }

  if (parameters.get(CCS_RGM_RUN_SYNCHRON_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
    case VSD_TYPE_ETALON:
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) < Regime::WorkState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_SYNCHRON_STATE) < Regime::WorkState) {
        return false;
      }
      break;
    }
  }

  return true;
}

bool interceptionStopRegime()
{
  if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
    case VSD_TYPE_ETALON:
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) != Regime::IdleState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_STATE) != Regime::IdleState) {
        return false;
      }
      break;
    }
  }
  return true;
}

void setGeneralStateRunMode()
{
  for (int i = 0; i < COUNT_RUN_REGIMES; i++) {
    if (parameters.get(runRgmState[i]) != Regime::IdleState) {
      parameters.set(CCS_RGM_RUN_VSD_STATE, parameters.get(runRgmState[i]));
    }
  }

  if (parameters.get(CCS_TYPE_VSD) == VSD_TYPE_DANFOSS) {
    parameters.set(CCS_RGM_RUN_VSD_STATE, Regime::IdleState);
  }
}

bool offRunModeExcept(uint16_t id)
{
  bool err = true;
  if (ksu.isRunOrWorkMotor()) {                                                      // Если установка в работе
    return err;                                                                 // Возвращаем ошибку
  }
  else {                                                                        // Установка в останове
    for (int i = 0; i < COUNT_RUN_REGIMES; i++) {                               // Массив пусковых режимов
      if (id != runRgmMode[i]) {                                                // Не включаемый режим
        err = parameters.set(runRgmMode[i], Regime::OffAction);                 // Выключаем режим
        if (err) {                                                              // Не смогли выключить режим
          return err;                                                           // Возвращаем ошибку
        }
      }
    }
    err = false;
    return err;
  }
}

bool offWorkRgmExcept(uint16_t id)
{
  bool err = true;
  for (int i = 0; i < COUNT_REGIMES; i++) {                                     // Массив пусковых режимов
    if (id == workRgmMode[i][0]) {                                              // Нашли строку с включаемым режимом
      for (int j = 0; j < COUNT_REGIMES; j++) {                                 // Цикл по столбцам
        if (!workRgmMode[i][j+1]) {                                             //
          err = parameters.set(workRgmMode[j][0], Regime::OffAction);           // Выключаем режим
          if (err) {
            return err;                                                         // Возвращаем ошибку
          }
        }
      }
    }
  }
  err = false;
  return err;
}
