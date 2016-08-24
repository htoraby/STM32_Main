#include "scada_surgutneftegas.h"
#include "user_main.h"

ScadaSurgutneftegas::ScadaSurgutneftegas()
{
  countParameters_ = 256;
  initParameters();
}

ScadaSurgutneftegas::~ScadaSurgutneftegas()
{

}

void ScadaSurgutneftegas::calcParamsTask()
{
  while (1) {
    osDelay(100);

    for (int i = 0; i < countParameters_; ++i) {
      if (scadaParameters_[i].id > 0) {
        scadaParameters_[i].value.float_t = parameters.get(scadaParameters_[i].id);
      }
    }

    // 20 Режим работы СУ (СНГ)
    uint16_t value = 0;

    scadaParameters_[20].value.float_t = value;

  }
}

int ScadaSurgutneftegas::setNewValue(ScadaParameter *param)
{
  if (param->id > 0) {
    return parameters.set(param->id, param->value.float_t, RemoteType);
  }

  return err_r;
}
