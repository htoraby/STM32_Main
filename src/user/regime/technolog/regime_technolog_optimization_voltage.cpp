#include "regime_technolog_optimization_voltage.h"
#include "protection_main.h"

RegimeTechnologOptimizationVoltage::RegimeTechnologOptimizationVoltage()
{

}

RegimeTechnologOptimizationVoltage::~RegimeTechnologOptimizationVoltage()
{

}



void RegimeTechnologOptimizationVoltage::processing()
{
  // Уставки режима
  action_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_MODE);                         // Действие режима
  scale_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_SCALE) / 100;                   // Максимальное отклонение U/f
  step_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_STEP);                           // Шаг изменения напряжения

  // Скрытые настройки
  recalcLimits_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_LIMITS);                 // Пересчитывать границы от текущей характеристики U/f
  beginUp_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_FIRST_STEP);                  // Первый шаг 0-вниз, 1-вверх
  delta_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_DELTA) / 100;                   // Дельта изменения тока для принятия решения
  timeCur_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_TIME_CURRENT);                // Время усреднения тока

  // Текущие параметры режима
  state_ = parameters.get(CCS_RGM_OPTIM_VOLTAGE_STATE);                         // Состояние
  // Прочие уставки станции
  maxUfPoint_ = parameters.get(CCS_BASE_VOLTAGE);

  if ((action_ == OffAction) || ksu.isBreakOrStopMotor()) {                     // Выключен режим
    if ((state_ != IdleState) &&
        (state_ != RunningState) &&
        (state_ != PauseState) &&
        (state_ != PauseState + 1) &&
        (state_ != StopState)) {                                                // Режим выключили во время работы
      #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Optim::processing() Shutdown in operation (state_=%d, action_=%d)",
                   state_, action_);
      #endif
      state_ = StopState;                                                       // Возвращаем настройки работы установки
    }
  }

  switch (state_) {                                                             // Автомат состояния режима
  case IdleState:                                                               // Состояние в котором ничего не происходит
    if ((action_ != OffAction) && ksu.isRunOrWorkMotor()) {                     // Если режим включен и двигатель запущен
      #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Optim::processing() Idle -> Running (state_=%d, action_=%d, isRunOrWorkMotor()=%d)",
                   state_, action_, ksu.isRunOrWorkMotor());
      #endif
      state_ = RunningState;
    }
    break;

  case RunningState:                                                            // Состояние ожидания после первого пуска
    if ((ksu.getSecFromCurTime(CCS_LAST_RUN_DATE_TIME) >= parameters.get(CCS_RGM_OPTIM_VOLTAGE_DELAY))                // Если время первого запуска истекло
      && (parameters.get(VSD_FREQUENCY) == parameters.get(VSD_FREQUENCY_NOW))) {// Вышли на частоту уставки
      saveUfBeforeOptim();

      // Сохраняем напряжения U/f
      optim_[0][0] = VSD_UF_CHARACTERISTIC_F_1;
      optim_[0][1] = VSD_UF_CHARACTERISTIC_U_1;
      optim_[1][0] = VSD_UF_CHARACTERISTIC_F_2;
      optim_[1][1] = VSD_UF_CHARACTERISTIC_U_2;
      optim_[2][0] = VSD_UF_CHARACTERISTIC_F_3;
      optim_[2][1] = VSD_UF_CHARACTERISTIC_U_3;
      optim_[3][0] = VSD_UF_CHARACTERISTIC_F_4;
      optim_[3][1] = VSD_UF_CHARACTERISTIC_U_4;
      optim_[4][0] = VSD_UF_CHARACTERISTIC_F_5;
      optim_[4][1] = VSD_UF_CHARACTERISTIC_U_5;
      optim_[5][0] = VSD_UF_CHARACTERISTIC_F_6;
      optim_[5][1] = VSD_UF_CHARACTERISTIC_U_6;

      for (int i = 0; i <= 5; i++) {
        optim_[i][2] = parameters.get(optim_[i][0]);
        optim_[i][3] = parameters.get(optim_[i][1]);
        optim_[i][4] = ((optim_[i][3] * (1 - scale_)) < 0) ? 0 : optim_[i][3] * (1 - scale_);
        optim_[i][5] = ((optim_[i][3] * (1 + scale_)) > maxUfPoint_) ? maxUfPoint_ : optim_[i][3] * (1 + scale_);
      }

      beginCurrent_ = ksu.getTime();                                            // Запоминаем время перехода к усреднению тока
      state_ = WorkState;                                                       // Переход началу работы режима
      #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "Optim::processing() Running -> Work (state_=%d, action_=%d, runTime=%d, delay_=%d, freq=%6.2f, freqNow=%6.2f)",
                   state_, action_, ksu.getSecFromCurTime(CCS_LAST_RUN_DATE_TIME), parameters.get(CCS_RGM_OPTIM_VOLTAGE_DELAY), parameters.get(VSD_FREQUENCY), parameters.get(VSD_FREQUENCY_NOW));
      #endif
    }
    break;

  case WorkState:
    if (ksu.getSecFromCurTime(beginCurrent_) >= timeCur_) {                     // Время усреднения тока истекло
      newCurrent_ = newCurrent_ / cntCurrent_;                                  // Вычисление нового среднего тока

      optim_[0][0] = VSD_UF_CHARACTERISTIC_F_1;
      optim_[0][1] = VSD_UF_CHARACTERISTIC_U_1;
      optim_[1][0] = VSD_UF_CHARACTERISTIC_F_2;
      optim_[1][1] = VSD_UF_CHARACTERISTIC_U_2;
      optim_[2][0] = VSD_UF_CHARACTERISTIC_F_3;
      optim_[2][1] = VSD_UF_CHARACTERISTIC_U_3;
      optim_[3][0] = VSD_UF_CHARACTERISTIC_F_4;
      optim_[3][1] = VSD_UF_CHARACTERISTIC_U_4;
      optim_[4][0] = VSD_UF_CHARACTERISTIC_F_5;
      optim_[4][1] = VSD_UF_CHARACTERISTIC_U_5;
      optim_[5][0] = VSD_UF_CHARACTERISTIC_F_6;
      optim_[5][1] = VSD_UF_CHARACTERISTIC_U_6;

      for (int i = 0; i <= 5; i++) {
        optim_[i][2] = parameters.get(optim_[i][0]);
        optim_[i][3] = parameters.get(optim_[i][1]);
      }

      if (parameters.get(CCS_RGM_OPTIM_VOLTAGE_LIMITS)) {
        for (int i = 0; i <= 5; i++) {
          optim_[i][4] = ((optim_[i][3] * (1 - scale_)) < 0) ? 0 : optim_[i][3] * (1 - scale_);
          optim_[i][5] = ((optim_[i][3] * (1 + scale_)) > maxUfPoint_) ? maxUfPoint_ : optim_[i][3] * (1 + scale_);
        }
      }

      if (beginUp_) {                                                            // Оптимизация начинается с повышения
        state_ = WorkState + 6;                                                 // Переход на первое повышение напряжения
        #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Optim::processing() Work -> Work+6 (state_=%d, action_=%d, beginUp_=%1.0f, time=%d, timeCur_=%d, newCurrent_=%10.6f)",
                     state_, action_, beginUp_, ksu.getSecFromCurTime(beginCurrent_), timeCur_, newCurrent_);
        #endif

      }
      else {                                                                    // Оптимизация начинается с повышения
        state_ = WorkState + 1;                                                 // Переход на первое понижение напряжения
        #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Optim::processing() Work -> Work+1 (state_=%d, action_=%d, beginUp_=%1.0f, time=%d, timeCur_=%d, newCurrent_=%10.6f)",
                     state_, action_, beginUp_, ksu.getSecFromCurTime(beginCurrent_), timeCur_, newCurrent_);
        #endif
      }
      oldCurrent_ = newCurrent_;
      newCurrent_ = 0;
      cntCurrent_ = 0;                                                          // Сброс счётчика выборок тока
    }
    else {                                                                      // Время
      newCurrent_ = newCurrent_ + parameters.get(CCS_MOTOR_CURRENT_AVARAGE);    // Накопление значений тока
      cntCurrent_++;
    }
//    firstStepDown_ = 1;
//    firstStepUp_ = 1;
    break;

  // Состояние первого понижения напряжения
  case WorkState + 1:
    for (int i = 1; i <= 5; i++) {                                              // Цикл по частотам для верхней точки нахождения отрезка U/f
      if (parameters.get(VSD_FREQUENCY_NOW) <= optim_[i][2]) {                  // Если текущая частоты меньше значения очередной точки U/f
        if ((optim_[i][3] <= optim_[i][4]) &&                                   // Если достигли нижних границ в обеих точках отрезка
            (optim_[i-1][3] <= optim_[i-1][4])) {                               // Нет возможности понижать напряжение
          if (beginUp_) {                                                       // Если оптимизация начиналась вверх, т.е. попробовали в обе стороны
            beginPause_ = ksu.getTime();
            state_ = PauseState;                                                // Переход на паузу, конец оптимизации
            #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "Optim::processing() Work+1 -> Pause (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f, beginUp_=%3.1f)",
                         state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5], beginUp_);
            #endif

          }
          else {
            state_ = WorkState + 6;                                             // Переход на первое повышение напряжения
            #if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "Optim::processing() Work+1 -> Work+6 (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f, beginUp_=%3.1f)",
                         state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5], beginUp_);
            #endif

          }
        }
        else {                                                                  // Можем понизить напряжение
          for (int j = i; j >= i - 1; j--) {                                    // Понижаем напряжение на шаг
            optim_[j][3] = ((optim_[j][3] - step_) > optim_[j][4]) ? (optim_[j][3] - step_) : optim_[j][4];
            parameters.set(optim_[j][1], optim_[j][3], NoneType);               // Пишем в устройство
          }
          beginCurrent_ = ksu.getTime();
          state_ = WorkState + 2;                                               // Переход на контроль тока после первого понижения
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+1 -> Work+2 (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f, beginUp_=%3.1f)",
                       state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5], beginUp_);
          #endif
        }
        break;                                                                  // Выход из цикла
      }
    }
    break;
  // Состояние контроля тока после первого понижения
  case WorkState + 2:
    if (ksu.getSecFromCurTime(beginCurrent_) >= timeCur_) {                     // Время усреднения тока истекло
      newCurrent_ = newCurrent_ / cntCurrent_;                                  // Вычисление нового среднего тока
      if ((newCurrent_ > oldCurrent_ * (1 + delta_)) ||                         // Если ток увеличился
          (newCurrent_ > parameters.get(VSD_MOTOR_CURRENT))) {
        if (beginUp_) {                                                         // Если оптимизация начиналась с повышения
          beginPause_ = ksu.getTime();
          state_ = PauseState;                                                  // Переход на откат повышения напряжения
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+2 -> Pause (state_=%d, action_=%d, time=%d, timeCur_=%d, newCurrent_=%10.6f, oldCurrent_=%10.6f, mtrCur=%10.6f, cntCurrent_=%d, beginUp_=%d)",
                       state_, action_, ksu.getSecFromCurTime(beginCurrent_), timeCur_, newCurrent_, oldCurrent_, parameters.get(VSD_MOTOR_CURRENT), cntCurrent_, beginUp_);
          #endif
        }
        else {                                                                  // Оптимизация начинается вниз
          state_ = WorkState + 6;                                               // Переход на первое повышение напряжения
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+2 -> Work+6 (state_=%d, action_=%d, time=%d, timeCur_=%d, newCurrent_=%10.6f, oldCurrent_=%10.6f, mtrCur=%10.6f, cntCurrent_=%d, beginUp_=%d)",
                       state_, action_, ksu.getSecFromCurTime(beginCurrent_), timeCur_, newCurrent_, oldCurrent_, parameters.get(VSD_MOTOR_CURRENT), cntCurrent_, beginUp_);
          #endif
        }
      }
      else {
        state_ = WorkState + 3;                                                 // Переход на второе и последующие понижения
        #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Optim::processing() Work+2 -> Work+3 (state_=%d, action_=%d, time=%d, timeCur_=%d, newCurrent_=%10.6f, oldCurrent_=%10.6f, mtrCur=%10.6f, cntCurrent_=%d)",
                     state_, action_, ksu.getSecFromCurTime(beginCurrent_), timeCur_, newCurrent_, oldCurrent_, parameters.get(VSD_MOTOR_CURRENT), cntCurrent_);
        #endif
      }
      oldCurrent_ = newCurrent_;                                                // Запоминаем старое среднее значение тока
      cntCurrent_ = 0;
      newCurrent_ = 0;
    }
    else {
      newCurrent_ = newCurrent_ + parameters.get(CCS_MOTOR_CURRENT_AVARAGE);    // Накопление значений тока
      cntCurrent_++;
    }
    break;
  // Состояние второго и последующих понижений напряжения
  case WorkState + 3:
    for (int i = 1; i <= 5; i++) {                                              // Цикл по частотам для нахождения отрезка U/f
      if (parameters.get(VSD_FREQUENCY_NOW) <= optim_[i][2]) {                  // Если текущая частоты меньше значения очередной точки U/f
        if ((optim_[i][3] <= optim_[i][4]) &&                                   // Нет возможности понижать напряжение
            (optim_[i-1][3] <= optim_[i-1][4])) {                               // Нижняя и верхняя точка отрезка в минимуме
          beginPause_ = ksu.getTime();
          state_ = PauseState;                                                  // Оптимизация закончена
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+3 -> Pause (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f)",
                       state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5]);
          #endif
        }
        else {
          for (int j = i; j >= i-1; j--) {
            optim_[j][3] = ((optim_[j][3] - step_) > optim_[j][4]) ? (optim_[j][3] - step_) : optim_[j][4];
            parameters.set(optim_[j][1], optim_[j][3], NoneType);
          }
          beginCurrent_ = ksu.getTime();
          state_ = WorkState + 4;                                               // Переход на контроль тока после второго и последующих понижений
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+3 -> Work+4 (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f)",
                       state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5]);
          #endif
        }
        break;                                                                  // Выход из цикла
      }
    }
    break;
  // Состояние контроля тока после второго и последующих понижений напряжения
  case WorkState + 4:
    if (ksu.getSecFromCurTime(beginCurrent_) >= timeCur_) {                      // Время усреднения тока истекло
      newCurrent_ = newCurrent_ / cntCurrent_;                                  // Вычисление нового среднего тока
      if ((newCurrent_ > oldCurrent_ * (1 + delta_)) ||
          (newCurrent_ > parameters.get(VSD_MOTOR_CURRENT))) {
        state_ = WorkState + 10;                                                // Переход на состяние отката напряжения вверх
        #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Optim::processing() Work+4 -> Work+10 (state_=%d, action_=%d, time=%d, timeCur_=%d, newCurrent_=%10.6f, oldCurrent_=%10.6f, mtrCur=%10.6f, cntCurrent_=%d)",
                     state_, action_, ksu.getSecFromCurTime(beginCurrent_), timeCur_, newCurrent_, oldCurrent_, parameters.get(VSD_MOTOR_CURRENT), cntCurrent_);
        #endif
      }
      else {
        state_ = WorkState + 3;                                                 // Переход на второе и последующие понижения
        #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Optim::processing() Work+4 -> Work+3 (state_=%d, action_=%d, time=%d, timeCur_=%d, newCurrent_=%10.6f, oldCurrent_=%10.6f, mtrCur=%10.6f, cntCurrent_=%d)",
                     state_, action_, ksu.getSecFromCurTime(beginCurrent_), timeCur_, newCurrent_, oldCurrent_, parameters.get(VSD_MOTOR_CURRENT), cntCurrent_);
        #endif

      }
      oldCurrent_ = newCurrent_;
      cntCurrent_ = 0;
      newCurrent_ = 0;
    }
    else {
      newCurrent_ = newCurrent_ + parameters.get(CCS_MOTOR_CURRENT_AVARAGE);    // Накопление значений тока
      cntCurrent_++;
    }
    break;
  // Состяние отката напряжения вниз
  case WorkState + 5:
    for (int i = 1; i <= 5; i++) {                                              // Цикл по частотам для нахождения отрезка U/f
      if (parameters.get(VSD_FREQUENCY_NOW) <= optim_[i][2]) {                  // Если текущая частоты меньше значения очередной точки U/f
        if ((optim_[i][3] <= optim_[i][4]) &&                                   // Нет возможности понижать напряжение
            (optim_[i-1][3] <= optim_[i-1][4])) {                               // Нижняя и верхняя точка отрезка в минимуме
          beginPause_ = ksu.getTime();
          state_ = PauseState;
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+5 -> Pause (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f)",
                       state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5]);
          #endif
        }
        else {
          for (int j = i; j >= i - 1; j--) {
            optim_[j][3] = ((optim_[j][3] - step_) > optim_[j][4]) ? (optim_[j][3] - step_) : optim_[j][4];
            parameters.set(optim_[j][1], optim_[j][3], NoneType);               // Пишем в устройство
          }
          beginPause_ = ksu.getTime();
          state_ = PauseState;                                                  // Оптимизация закончена переход на паузу
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+5 -> Back (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f)",
                       state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5]);
          #endif
        }
        break;                                                                  // Выход из цикла
      }
    }
    break;
  // Состояние первого повышения напряжения
  case WorkState + 6:
    for (int i = 1; i <= 5; i++) {                                              // Цикл по частотам для нахождения отрезка U/f
      if (parameters.get(VSD_FREQUENCY_NOW) <= optim_[i][2]) {                  // Если текущая частоты меньше значения очередной точки U/f
        if ((optim_[i][3] >= optim_[i][5]) &&                                   // Нет возможности повышать напряжение
            (optim_[i-1][3] >= optim_[i-1][5])) {                               // Нижняя и верхняя точка отрезка в максимуме
            if (beginUp_) {                                                     // Если оптимизация начинается с повышения напряжения
              state_ = WorkState + 1;
              #if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "Optim::processing() Work+6 -> Work+1 (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f, beginUp_=%3.1f)",
                           state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5], beginUp_);
              #endif

            }
            else {
              beginPause_ = ksu.getTime();
              state_ = PauseState;
              #if (USE_LOG_DEBUG == 1)
              logDebug.add(DebugMsg, "Optim::processing() Work+6 -> Pause (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f, beginUp_=%3.1f)",
                           state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5], beginUp_);
              #endif
            }
        }
        else {
          for (int j = i; j >= i - 1; j--) {
            optim_[j][3] = ((optim_[j][3] + step_) < optim_[j][5]) ? (optim_[j][3] + step_) : optim_[j][5];
            parameters.set(optim_[j][1], optim_[j][3], NoneType);               // Пишем в устройство
          }
          beginCurrent_ = ksu.getTime();
          state_ = WorkState + 7;                                               // Переход на контроль тока после первого повышения
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+6 -> Work+7 (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f)",
                       state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5]);
          #endif
        }
        break;                                                                  // Выход из цикла
      }
    }
    break;
  // Состояние контроля тока после первого повышения
  case WorkState + 7:
    if (ksu.getSecFromCurTime(beginCurrent_) >= timeCur_) {                      // Время усреднения тока истекло
      newCurrent_ = newCurrent_ / cntCurrent_;
      if ((newCurrent_ > oldCurrent_ * (1 + delta_)) ||
          (newCurrent_ > parameters.get(VSD_MOTOR_CURRENT))) {
        if (beginUp_) {
          state_ = WorkState + 1;                                               // Переход на первое понижение напряжения
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+7 -> Work+1 (state_=%d, action_=%d, time=%d, timeCur_=%d, newCurrent_=%10.6f, oldCurrent_=%10.6f, mtrCur=%10.6f, cntCurrent_=%d, beginUp_=%d)",
                       state_, action_, ksu.getSecFromCurTime(beginCurrent_), timeCur_, newCurrent_, oldCurrent_, parameters.get(VSD_MOTOR_CURRENT), cntCurrent_, beginUp_);
          #endif
        }
        else {
          state_ = PauseState;
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+7 -> Pause (state_=%d, action_=%d, time=%d, timeCur_=%d, newCurrent_=%10.6f, oldCurrent_=%10.6f, mtrCur=%10.6f, cntCurrent_=%d, beginUp_=%d)",
                       state_, action_, ksu.getSecFromCurTime(beginCurrent_), timeCur_, newCurrent_, oldCurrent_, parameters.get(VSD_MOTOR_CURRENT), cntCurrent_, beginUp_);
          #endif
        }
      }
      else {
        beginPause_ = ksu.getTime();
        state_ = WorkState + 8;                                                 // Переход на второе и последующие повышение
        #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Optim::processing() Work+7 -> Work+8 (state_=%d, action_=%d, time=%d, timeCur_=%d, newCurrent_=%10.6f, oldCurrent_=%10.6f, mtrCur=%10.6f, cntCurrent_=%d)",
                     state_, action_, ksu.getSecFromCurTime(beginCurrent_), timeCur_, newCurrent_, oldCurrent_, parameters.get(VSD_MOTOR_CURRENT), cntCurrent_);
        #endif

      }
      oldCurrent_ = newCurrent_;
      cntCurrent_ = 0;
      newCurrent_ = 0;
    }
    else {
      newCurrent_ = newCurrent_ + parameters.get(CCS_MOTOR_CURRENT_AVARAGE);    // Накопление значений тока
      cntCurrent_++;
    }
    break;
  // Состояние второго и последующих повышений напряжения
  case WorkState + 8:
    for (int i = 1; i <= 5; i++) {                                              // Цикл по частотам для нахождения отрезка U/f
      if (parameters.get(VSD_FREQUENCY_NOW) <= optim_[i][2]) {                  // Если текущая частоты меньше значения очередной точки U/f
        if ((optim_[i][3] >= optim_[i][5]) &&                                   // Нет возможности повышать напряжение
            (optim_[i-1][3] >= optim_[i-1][5])) {                               // Нижняя и верхняя точка отрезка в максимуме
          beginPause_ = ksu.getTime();
          state_ = PauseState;                                                // Переход на первое понижение напряжения
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+8 -> Pause (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f)",
                       state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5]);
          #endif
        }
        else {
          for (int j = i; j >= i - 1; j--) {
            optim_[j][3] = ((optim_[j][3] + step_) < optim_[j][5]) ? (optim_[j][3] + step_) : optim_[j][5];
            parameters.set(optim_[j][1], optim_[j][3], NoneType);               // Пишем в устройство
          }
          beginCurrent_ = ksu.getTime();
          state_ = WorkState + 9;                                               // Переход на контроль тока после первого повышения
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+8 -> Work+9 (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f)",
                       state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5]);
          #endif
        }
        break;                                                                  // Выход из цикла
      }
    }
    break;

  case WorkState + 9:                                                           // Состояние контроля тока после второго и последующих повышений напряжения
    if (ksu.getSecFromCurTime(beginCurrent_) >= timeCur_) {                      // Время усреднения тока истекло
      newCurrent_ = newCurrent_ / cntCurrent_;
      if ((newCurrent_ > oldCurrent_ * (1 + delta_)) ||
          (newCurrent_ > parameters.get(VSD_MOTOR_CURRENT))) {
        state_ = WorkState + 5;                                                 // Переход на состяние отката напряжения вниз
        #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Optim::processing() Work+9 -> Work+5 (state_=%d, action_=%d, time=%d, timeCur_=%d, newCurrent_=%10.6f, oldCurrent_=%10.6f, mtrCur=%10.6f, cntCurrent_=%d)",
                     state_, action_, ksu.getSecFromCurTime(beginCurrent_), timeCur_, newCurrent_, oldCurrent_, parameters.get(VSD_MOTOR_CURRENT), cntCurrent_);
        #endif
      }
      else {
        state_ = WorkState + 8;                                                 // Переход на второе и последующие повышения
        #if (USE_LOG_DEBUG == 1)
        logDebug.add(DebugMsg, "Optim::processing() Work+9 -> Work+8 (state_=%d, action_=%d, time=%d, timeCur_=%d, newCurrent_=%10.6f, oldCurrent_=%10.6f, mtrCur=%10.6f, cntCurrent_=%d)",
                     state_, action_, ksu.getSecFromCurTime(beginCurrent_), timeCur_, newCurrent_, oldCurrent_, parameters.get(VSD_MOTOR_CURRENT), cntCurrent_);
        #endif
      }
      oldCurrent_ = newCurrent_;
      cntCurrent_ = 0;
      newCurrent_ = 0;
    }
    else {
      newCurrent_ = newCurrent_ + parameters.get(CCS_MOTOR_CURRENT_AVARAGE);    // Накопление значений тока
      cntCurrent_++;
    }
    break;

  case WorkState + 10:                                                          // Состояние отката напряжения вверх, переход возможен из понижения
    for (int i = 1; i <= 5; i++) {                                              // Цикл по частотам для нахождения отрезка U/f
      if (parameters.get(VSD_FREQUENCY_NOW) <= optim_[i][2]) {                  // Если текущая частоты меньше значения очередной точки U/f
        if ((optim_[i][3] >= optim_[i][5]) &&                                   // Нет возможности повышать напряжение
            (optim_[i-1][3] >= optim_[i-1][5])) {                               // Нижняя и верхняя точка отрезка в максимуме
          beginPause_ = ksu.getTime();
          state_ = PauseState;                                                  // Оптимизация закончена
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+10 -> Pause (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f)",
                       state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5]);
          #endif
        }
        else {
          for (int j = i; j >= i - 1; j--) {
            optim_[j][3] = ((optim_[j][3] + step_) < optim_[j][5]) ? (optim_[j][3] + step_) : optim_[j][5];
            parameters.set(optim_[j][1], optim_[j][3], NoneType);               // Пишем в устройство
          }
          beginPause_ = ksu.getTime();
          state_ = PauseState;                                                  // Оптимизация закончена
          #if (USE_LOG_DEBUG == 1)
          logDebug.add(DebugMsg, "Optim::processing() Work+10 -> Back (state_=%d, action_=%d, freqNow=%6.2f, i=%d, f1=%5.1f, U1=%5.1f, U1min=%5.1f, U1max=%5.1f, f2=%5.1f, U2=%5.1f, U2min=%5.1f, U2max=%5.1f)",
                       state_, action_, parameters.get(VSD_FREQUENCY_NOW), i, optim_[i-1][2], optim_[i-1][3], optim_[i-1][4], optim_[i-1][5], optim_[i][2], optim_[i][3], optim_[i][4], optim_[i][5]);
          #endif
        }
        break;                                                                  // Выход из цикла
      }
    }
    break;

  case PauseState:
    if (ksu.getSecFromCurTime(beginPause_) >= parameters.get(CCS_RGM_OPTIM_VOLTAGE_PERIOD)) {                        // Время периодичности истекло
      if (parameters.get(CCS_RGM_OPTIM_VOLTAGE_RESTORE_U)) {                    // Сбрасываем характеристику U/f
        loadUfAfterOptim();
      }
      #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "optim::processing() Pause -> Pause+1 (state_=%d, action_=%d, begin_=%d, period_=%2.0f, restore=%1.0f)",
                   state_, action_, ksu.getSecFromCurTime(beginPause_), parameters.get(CCS_RGM_OPTIM_VOLTAGE_PERIOD), parameters.get(CCS_RGM_OPTIM_VOLTAGE_RESTORE_U));
      #endif
      beginCurrent_ = ksu.getTime();
      state_ = PauseState + 1;
    }
    break;
  case PauseState + 1:
    if (ksu.getSecFromCurTime(beginCurrent_) >= parameters.get(CCS_RGM_OPTIM_VOLTAGE_DELAY_RESTORE_U)) {
      #if (USE_LOG_DEBUG == 1)
      logDebug.add(DebugMsg, "optim::processing() Pause+1 -> Work (state_=%d, action_=%d, beginCurrent_=%d, time_=%2.0f)",
                   state_, action_, ksu.getSecFromCurTime(beginCurrent_), parameters.get(CCS_RGM_OPTIM_VOLTAGE_DELAY_RESTORE_U));
      #endif
      beginCurrent_ = ksu.getTime();
      state_ = WorkState;
    }
    break;
  case StopState:
    loadUfAfterOptim();
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "optim::processing() Stop -> Idle (state_=%d, action_=%d)",
                 state_, action_);
    #endif
    state_ = IdleState;
    break;

  default:
    #if (USE_LOG_DEBUG == 1)
    logDebug.add(DebugMsg, "optim::processing() Default -> Stop (state_=%d, action_=%d)",
                 state_, action_);
    #endif
    state_ = StopState;
    break;
  }
  parameters.set(CCS_RGM_OPTIM_VOLTAGE_STATE, state_);
}


void RegimeTechnologOptimizationVoltage::saveUfBeforeOptim()
{
  parameters.set(CCS_RGM_OPTIM_VOLTAGE_U1, parameters.get(VSD_UF_CHARACTERISTIC_U_1));
  parameters.set(CCS_RGM_OPTIM_VOLTAGE_U2, parameters.get(VSD_UF_CHARACTERISTIC_U_2));
  parameters.set(CCS_RGM_OPTIM_VOLTAGE_U3, parameters.get(VSD_UF_CHARACTERISTIC_U_3));
  parameters.set(CCS_RGM_OPTIM_VOLTAGE_U4, parameters.get(VSD_UF_CHARACTERISTIC_U_4));
  parameters.set(CCS_RGM_OPTIM_VOLTAGE_U5, parameters.get(VSD_UF_CHARACTERISTIC_U_5));
  parameters.set(CCS_RGM_OPTIM_VOLTAGE_U6, parameters.get(VSD_UF_CHARACTERISTIC_U_6));
}

void RegimeTechnologOptimizationVoltage::loadUfAfterOptim()
{
  parameters.set(VSD_UF_CHARACTERISTIC_U_1, parameters.get(CCS_RGM_OPTIM_VOLTAGE_U1));
  parameters.set(VSD_UF_CHARACTERISTIC_U_2, parameters.get(CCS_RGM_OPTIM_VOLTAGE_U2));
  parameters.set(VSD_UF_CHARACTERISTIC_U_3, parameters.get(CCS_RGM_OPTIM_VOLTAGE_U3));
  parameters.set(VSD_UF_CHARACTERISTIC_U_4, parameters.get(CCS_RGM_OPTIM_VOLTAGE_U4));
  parameters.set(VSD_UF_CHARACTERISTIC_U_5, parameters.get(CCS_RGM_OPTIM_VOLTAGE_U5));
  parameters.set(VSD_UF_CHARACTERISTIC_U_6, parameters.get(CCS_RGM_OPTIM_VOLTAGE_U6));
}
