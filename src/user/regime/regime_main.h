#ifndef REGIMEMAIN_H
#define REGIMEMAIN_H

#include "regime_technolog_periodic.h"
#include "regime_technolog_soft_change_freq.h"
#include "regime_technolog_maintenance_param.h"
#include "regime_technolog_alternation_freq.h"

#include "regime_run_push.h"
#include "regime_run_push_novomet.h"
#include "regime_run_swing.h"
#include "regime_run_swing_novomet.h"
#include "regime_run_novomet.h"

/*!
 * \brief Функция инициализации задачи обработки всех режимов
 */
void regimeInit();

/*!
 * \brief Метод перехвата запуска режимом
 * \return false - пуск запрещён, true - пуск разрещён
 */
bool interceptionStartRegime();

#endif // REGIMEMAIN_H
