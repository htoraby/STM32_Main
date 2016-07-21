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
  void processingStateRunning();
  void processingState();
  void processingStateWork();
  void processingStatePause();
  void processingStateStop();
  void processingStateRestart();
  void automatRegime();

  void resetAdaptationVector(uint16_t type);

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
