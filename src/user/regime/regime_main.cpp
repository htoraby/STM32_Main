#include "regime_main.h"

#define COUNT_RUN_REGIMES  7                  //!< Количество пусковых режимов, увеличивать при добавлении нового
#define COUNT_REGIMES 6

const uint16_t run[COUNT_RUN_REGIMES] = {     //!<
  CCS_RGM_RUN_PUSH_MODE,
  CCS_RGM_RUN_SWING_MODE,
  CCS_RGM_RUN_PICKUP_MODE,
  CCS_RGM_RUN_AUTO_ADAPTATION_MODE,
  CCS_RGM_RUN_SKIP_RESONANT_MODE,
  CCS_RGM_RUN_SYNCHRON_MODE,
  CCS_RGM_RUN_DIRECT_MODE
};

Regime *regimes[COUNT_REGIMES];               //!< Количество режимов, увеличивать при добавлении нового

RegimeTechnologPeriodic regimeTechnologPeriodic;
RegimeTechnologSoftChangeFreq regimeTechnologSoftChangeFreq;
RegimeTechnologMaintenanceParam regimeTechnologMaintenanceParam;
RegimeTechnologAlternationFreq regimeTechnologAlternationFreq;
RegimeTechnologOptimizationVoltage regimeTechnologOptimizationVoltage;
RegimeTechnologJarring regimeTechnologJarring;


static void regimeTask(void *argument);

void regimeInit()
{
  regimes[0] = &regimeTechnologPeriodic;
  regimes[1] = &regimeTechnologSoftChangeFreq;
  regimes[2] = &regimeTechnologMaintenanceParam;
  regimes[3] = &regimeTechnologAlternationFreq;
  regimes[4] = &regimeTechnologOptimizationVoltage;
  regimes[5] = &regimeTechnologJarring;

  osThreadDef(Regimes, regimeTask, osPriorityNormal, 0, 4 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Regimes), NULL);
}

void regimeTask(void *argument)
{
  (void)argument;

  while (1) {
    osDelay(100);

    vsd->processingRegimeRun();
    checkWorkingRunMode();

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


void checkWorkingRunMode()
{
  if (isWorkingRunMode(CCS_RGM_RUN_PUSH_STATE))
    return;
  if (isWorkingRunMode(CCS_RGM_RUN_SWING_STATE))
    return;
  if (isWorkingRunMode(CCS_RGM_RUN_AUTO_ADAPTATION_STATE))
    return;
  if (isWorkingRunMode(CCS_RGM_RUN_PICKUP_STATE))
    return;
  if (isWorkingRunMode(CCS_RGM_RUN_SKIP_RESONANT_STATE))
    return;
  if (isWorkingRunMode(CCS_RGM_RUN_SYNCHRON_STATE))
    return;
  if (isWorkingRunMode(CCS_RGM_RUN_DIRECT_STATE))
    return;

  // TODO: Надо подумать как это перенести в сами режимы
  if (parameters.get(CCS_TYPE_VSD) == VSD_TYPE_DANFOSS) {
    parameters.set(CCS_RGM_RUN_VSD_STATE, Regime::IdleState);
  }
}

bool isWorkingRunMode(uint16_t id)
{
  float state = parameters.get(id);
  if (state != Regime::IdleState) {
    parameters.set(CCS_RGM_RUN_VSD_STATE, state);
    return true;
  }
  return false;
}


void offRunModeExcept(uint16_t id)
{
  for (int i = 0; i < COUNT_RUN_REGIMES; i++) {
    if (id != run[i]) {
      parameters.set(run[i], Regime::OffAction);
    }
  }
}
