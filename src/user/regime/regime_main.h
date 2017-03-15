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
 * \brief Метод аккумуляции и установки в общий регистр состояния пусковых режимов
 * если один из режимов работает устанавливается его состояние, иначе состояние Idle
 */
void setGeneralStateRunMode();

/*!
 * \brief Функция выключает (переводит MODE в Off) все пусковые режимы кроме
 * id указанного в параметре
 */
bool offRunModeExcept(uint16_t id);

/*!
 * \brief offWorkRgmExcept
 * \param id
 * \return
 */
bool offWorkRgmExcept(uint16_t id);




#endif // REGIMEMAIN_H
