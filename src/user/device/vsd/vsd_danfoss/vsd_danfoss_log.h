#ifndef VSDDANFOSSLOG_H
#define VSDDANFOSSLOG_H

#include "vsd_log.h"


class VsdDanfossLog : public VsdLog
{
public:
  VsdDanfossLog();
  ~VsdDanfossLog();

  bool checkAlarm();

  bool checkReady();

  void resetReady();

  /*!
   * \brief Чтение аварийного архива
   * \param ia
   * \param ib
   * \param ic
   * \param ud
   */
  void readAlarmLog(uint16_t *ia, uint16_t *ib, uint16_t *ic, uint16_t *ud);

  /*!
   * \brief Чтение пускового архива
   * \param ia
   * \param ib
   * \param ic
   * \param ud
   * \param cos
   */
  void readRunningLog(uint16_t *ia, uint16_t *ib, uint16_t *ic, uint16_t *ud, uint16_t *cos);
};

#endif // VSDDANFOSSLOG_H
