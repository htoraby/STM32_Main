#ifndef PROTECTION_H
#define PROTECTION_H

#include "user_main.h"

#define PROTECTION_ACTION_OFF         0
#define PROTECTION_ACTION_RESTART     1
#define PROTECTION_ACTION_BLOCK       2


#define PROTECTION_STATE_OFF            0
#define PROTECTION_STATE_ACTIV_BEGIN    10
#define PROTECTION_STATE_ACTIV_WAIT     20
#define PROTECTION_STATE_ACTIV          30
#define PROTECTION_STATE_WORK           40
#define PROTECTION_STATE_WORK_WAIT      50
#define PROTECTION_STATE_REACTION_BEGIN 60
#define PROTECTION_STATE_REACTION_WAIT  70
#define PROTECTION_STATE_REACTION       80
#define PROTECTION_STATE_FAILURE_BEGIN  90
#define PROTECTION_STATE_FAILURE_WAIT   100
#define PROTECTION_STATE_FAILURE        110
#define PROTECTION_STATE_RESTART_BEGIN  120
#define PROTECTION_STATE_RESTART_WAIT   130
#define PROTECTION_STATE_RESTART        140
#define PROTECTION_STATE_STOP           150
#define PROTECTION_STATE_BLOCK          160

class Protection
{
public:
  Protection();
  ~Protection();

  /// Состояние защиты
  float mode_;
  /// Действие защиты
  float action_;
   /// Состояние автомата защиты
  float state_;
  /// Уставка: задержка активации защиты
  float delayActiv_;
  /// Уставка: задержка срабатывания защиты
  float delayReaction_;
  /// Уставка: задержка АПВ защиты
  float delayRestart_;
  /// Уставка: граница срабатывания защиты
  float limitReaction_;
  /// Уставка: граница АПВ
  float limitRestart_;
  /// Уставка: количество АПВ
  float numberRestart_;
  /// Текущее значение контролируемого параметра
  float valueParameter_;
  /// Текущее значение таймера
  float timer_;
  /// Текущее количество АПВ по защите
  float countRestart_;
  float delayFailure_;


  /// Флаг выполнения условия срабатывания защиты
  unsigned char flagReaction_;
  /// Флаг запрещающего параметра
  unsigned char flagBlocking_;

  bool checkWorkKSU();

  /*!
   * \brief getCurrentParam
   * Метод получения текущих значений параметров защиты
   * \param первый параметр
   */
  void getCurrentParam(int count);


  /*!
   * \brief calcControlParameter
   * Метод вычисления контролируемого параметра
   * \return значение контролируемого параметра
   */
  float calcControlParameter(void);

  /*!
   * \brief checkControlParameter
   * Метод проверки контролируемого параметра
   * \return 0 - параметр в норме, 1 - параметр не в норме
   */
  unsigned char checkControlParameter();

  /*!
   * \brief protection
   * Автомат работы защиты
   * \return состояние защиты
   */
  unsigned char protection();



};

#endif // PROTECTION_H
