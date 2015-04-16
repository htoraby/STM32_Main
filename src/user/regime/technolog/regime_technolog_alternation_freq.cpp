#include "regime_technolog_alternation_freq.h"

RegimeTechnologAlternationFreq::RegimeTechnologAlternationFreq()
{

}

RegimeTechnologAlternationFreq::~RegimeTechnologAlternationFreq()
{

}

void RegimeTechnologAlternationFreq::processing()
{
  action_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_MODE);
  firstFreq_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_FREQ_1);
  secondFreq_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_FREQ_2);
  timeFirstFreq_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_TIMER_1);
  timeSecondFreq_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_TIMER_2);


  state_ = parameters.get(CCS_RGM_ALTERNATION_FREQ_STATE);
}

