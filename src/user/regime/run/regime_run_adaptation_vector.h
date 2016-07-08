#ifndef REGIMERUNADAPTATIONVECTOR_H
#define REGIMERUNADAPTATIONVECTOR_H

#include "regime_run.h"

typedef enum {
  typeOff,
  typeErr,
  typeInWork,
} TypeAdaptation;

/*!
 * \brief Класс режима автоадаптации векторного режима
 */
class RegimeRunAdaptationVector: public RegimeRun
{
public:
  RegimeRunAdaptationVector();
  virtual ~RegimeRunAdaptationVector();

  void getOtherSetpoint();
  void setOtherSetpoint();
  void processingStateIdle();
  void processingStateWork();
  void automatRegime();

private:
  float type_;
  float dR_;
  float pwrTrans_;
  float timer_;
  float nowCurMtr_;
  float nomCurMtr_;
  float strR_;
  float repeat_;
};

#endif // REGIMERUNADAPTATIONVECTOR_H
