#ifndef VSDETALONLOG_H
#define VSDETALONLOG_H

#include "vsd_log.h"

class VsdEtalonLog : public VsdLog
{
public:
  VsdEtalonLog();
  ~VsdEtalonLog();

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

#endif // VSDETALONLOG_H
