#include "regime_technolog_jarring.h"

RegimeTechnologJarring::RegimeTechnologJarring()
{

}

RegimeTechnologJarring::~RegimeTechnologJarring()
{

}

void RegimeTechnologJarring::processing()
{
  // Получение уставок режима
  action_ = parameters.get(CCS_RGM_JARRING_MODE);
  firstFreq_ = parameters.get(CCS_RGM_JARRING_UPDFREQ);

}

