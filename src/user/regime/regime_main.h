#ifndef REGIMEMAIN_H
#define REGIMEMAIN_H

#include "regime_technolog_periodic.h"
#include "regime_technolog_soft_change_freq.h"
#include "regime_technolog_maintenance_param.h"
#include "regime_technolog_alternation_freq.h"
#include "regime_technolog_optimization_voltage.h"
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
 * \brief Метод перехвата останова режимом
 * \return false - стоп запрещён, true - стоп разрещён
 */
bool interceptionStopRegime();

/*!
 * \brief Метод проверки работы пускового режима
 * функция проверяет состояние автоматов пусковых режимов, и если хотя бы один из
 * них не в состоянии Idle возвращает это состояниие. При добавлении новых пусковых
 * режимов нужно добавить в эту функцию.
 * \return
 */
void checkWorkingRunMode();

/*!
 * \brief Функция выключает (переводит MODE в Off) все пусковые режимы кроме
 * id указанного в параметре
 */
void offRunModeExcept(uint16_t id);

/*!
 * \brief isWorkingRunMode
 * \param id
 * \return
 */
bool isWorkingRunMode(uint16_t id);


#endif // REGIMEMAIN_H
