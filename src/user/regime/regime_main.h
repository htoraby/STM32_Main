#ifndef REGIMEMAIN_H
#define REGIMEMAIN_H

#include "regime_technolog_periodic.h"
#include "regime_technolog_soft_change_freq.h"
#include "regime_technolog_maintenance_param.h"
#include "regime_technolog_alternation_freq.h"
#include "regime_technolog_jarring.h"

/*!
 * \brief Функция инициализации задачи обработки всех режимов
 */
void regimeInit();

/*!
 * \brief Метод перехвата запуска режимом
 * \return false - пуск запрещён, true - пуск разрещён
 */
bool interceptionStartRegime();

/*!
 * \brief Метод проверки работы пускового режима
 * \return
 */
void checkWorkingRunMode();

/*!
 * \brief isWorkingRunMode
 * \param id
 * \return
 */
bool isWorkingRunMode(uint16_t id);


#endif // REGIMEMAIN_H
