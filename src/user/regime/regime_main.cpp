#include "regime_main.h"

#define COUNT_REGIMES 6                     //!< Количество режимов, увеличивать при добавлении нового

Regime *regimes[COUNT_REGIMES];

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

    for (int i = 0; i < COUNT_REGIMES; ++i) {
      regimes[i]->processing();
    }  
    vsd->processingRegimeRun();
    checkWorkingRunMode();
  }
}

bool interceptionStartRegime()
{
  if (ksu.isProgramMode() && (parameters.get(CCS_RGM_PERIODIC_MODE) != Regime::OffAction)) {
    if ((parameters.get(CCS_RGM_PERIODIC_STATE) == Regime::WorkState) ||
        (parameters.get(CCS_RGM_PERIODIC_STATE) == Regime::PauseState) ||
        (parameters.get(CCS_RGM_PERIODIC_STATE) == Regime::PauseState + 1)) {
      return false;
    }
  }

  if (parameters.get(CCS_RGM_RUN_PICKUP_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
    case VSD_TYPE_ETALON:
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) == Regime::IdleState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_PICKUP_STATE) == Regime::IdleState) {
        return false;
      }
      break;
    }
  }

  if (parameters.get(CCS_RGM_RUN_PUSH_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
    case VSD_TYPE_ETALON:
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) == Regime::IdleState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_PUSH_STATE) == Regime::IdleState) {
        return false;
      }
      break;
    }
  }

  if (parameters.get(CCS_RGM_RUN_SWING_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
    case VSD_TYPE_ETALON:
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) == Regime::IdleState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_SWING_STATE) == Regime::IdleState) {
        return false;
      }
      break;
    }
  }

  if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
    case VSD_TYPE_ETALON:
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) == Regime::IdleState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_SKIP_RESONANT_STATE) == Regime::IdleState) {
        return false;
      }
      break;
    }
  }

  if (parameters.get(CCS_RGM_RUN_AUTO_ADAPTATION_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) == Regime::IdleState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_AUTO_ADAPTATION_STATE) == Regime::IdleState) {
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

  parameters.set(CCS_RGM_RUN_VSD_STATE, Regime::IdleState);
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
