#include "scada_karakuduk.h"
#include "user_main.h"

ScadaKarakuduk::ScadaKarakuduk()
{
  countParameters_ = 293;
  initParameters();
}

ScadaKarakuduk::~ScadaKarakuduk()
{

}

void ScadaKarakuduk::calcParamsTask()
{
  while (1) {
    osDelay(100);

    for (int i = 0; i < countParameters_; ++i) {
      if (scadaParameters_[i].id > 0) {
        scadaParameters_[i].value.float_t = parameters.get(scadaParameters_[i].id);
      }
    }

    // 256 Состояние СУ и причина останова
    uint16_t value = 0;

    scadaParameters_[0].value.float_t = value;

    // 277 Режим работы
    value = 0;

    scadaParameters_[15].value.float_t = value;

    // 289 Состояние СУ
    value = 0;

    scadaParameters_[24].value.float_t = value;

    // 290 Причина останова
    value = 0;

    scadaParameters_[25].value.float_t = value;

    // 291 Значение аварийного параметра
    value = 0;

    scadaParameters_[26].value.float_t = value;

    // 295-296
    unTypeData data;
    data.uint32_t = parameters.get(CCS_GENERAL_RUN_DATE_TIME);
    scadaParameters_[30].value.float_t = data.uint16_t[0];
    scadaParameters_[31].value.float_t = data.uint16_t[1];

    // 298-299
    data.uint32_t = parameters.get(EM_ACTIVE_ENERGY)*3600;
    scadaParameters_[33].value.float_t = data.uint16_t[0];
    scadaParameters_[34].value.float_t = data.uint16_t[1];
  }
}

int ScadaKarakuduk::setNewValue(ScadaParameter *param)
{
  if (param->id > 0) {
    return parameters.set(param->id, param->value.float_t, RemoteType);
  }
  return err_r;
}
