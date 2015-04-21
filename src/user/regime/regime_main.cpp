#include "regime_main.h"

#define COUNT_REGIMES 6                     //!< Количество режимов, увеличивать при добавлении нового

Regime *regimes[COUNT_REGIMES];

RegimeTechnologPeriodic regimeTechnologPeriodic;
RegimeTechnologSoftChangeFreq regimeTechnologSoftChangeFreq;
RegimeTechnologMaintenanceParam regimeTechnologMaintenanceParam;
RegimeTechnologAlternationFreq regimeTechnologAlternationFreq;

RegimeRunPush *regimeRunPush;
RegimeRunSwing *regimeRunSwing;

static void regimeTask(void *argument);

void regimeInit()
{
  regimes[0] = &regimeTechnologPeriodic;
  regimes[1] = &regimeTechnologSoftChangeFreq;

  parameters.set(CCS_TYPE_VSD, 1);
  switch ((uint16_t)parameters.get(CCS_TYPE_VSD)) {
  case VSD_TYPE_NOVOMET:
    regimes[2] = new RegimeRunPushNovomet;
    regimes[3] = new RegimeRunSwingNovomet;
    break;
  default:
    regimes[2] = new RegimeRunPush;
    regimes[3] = new RegimeRunSwing;
    break;
  }

  regimes[4] = &regimeTechnologMaintenanceParam;
  regimes[5] = &regimeTechnologAlternationFreq;

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

  parameters.set(VSD_REGULATOR_QUEUE_1, 5.0);
  parameters.set(VSD_REGULATOR_QUEUE_2, -1.0);
  parameters.set(VSD_REGULATOR_QUEUE_3, -1.0);
  parameters.set(VSD_REGULATOR_QUEUE_4, -1.0);
  parameters.set(VSD_MOTOR_CONTROL, parameters.get(VSD_MOTOR_CONTROL));
  if (parameters.get(CCS_RGM_RUN_PUSH_MODE) != Regime::OffAction) {
    if (parameters.get(CCS_RGM_RUN_PUSH_STATE) == Regime::IdleState) {
      return false;
    }
  }
  else {
    vsd->offRegimePush();
  }

  return true;
}
