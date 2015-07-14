#include "regime_main.h"

#define COUNT_REGIMES 4                     //!< Количество режимов, увеличивать при добавлении нового

Regime *regimes[COUNT_REGIMES];

RegimeTechnologPeriodic regimeTechnologPeriodic;
RegimeTechnologSoftChangeFreq regimeTechnologSoftChangeFreq;
RegimeTechnologMaintenanceParam regimeTechnologMaintenanceParam;
RegimeTechnologAlternationFreq regimeTechnologAlternationFreq;

static void regimeTask(void *argument);

void regimeInit()
{
  regimes[0] = &regimeTechnologPeriodic;
  regimes[1] = &regimeTechnologSoftChangeFreq;
  regimes[2] = &regimeTechnologMaintenanceParam;
  regimes[3] = &regimeTechnologAlternationFreq;

  osThreadDef(RegimeTask, regimeTask, osPriorityNormal, 0, 4 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(RegimeTask), NULL);
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

  if (parameters.get(CCS_RGM_RUN_PICKUP_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) == Regime::IdleState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_PICKUP_MODE) == Regime::IdleState) {
        return false;
      }
      break;
    }

  }

  if (parameters.get(CCS_RGM_RUN_PUSH_MODE) != Regime::OffAction) {
    switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
    case VSD_TYPE_NOVOMET:
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
      if (parameters.get(CCS_RGM_RUN_VSD_STATE) == Regime::IdleState) {
        return false;
      }
      break;
    default:
      if (parameters.get(CCS_RGM_RUN_SWING_MODE) == Regime::IdleState) {
        return false;
      }
      break;
    }
  }

  return true;
}
