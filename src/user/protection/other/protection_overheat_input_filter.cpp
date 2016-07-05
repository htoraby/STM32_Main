#include "protection_overheat_input_filter.h"

ProtectionOverheatInputFilter::ProtectionOverheatInputFilter()
{
  idMode_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_MODE;
  idPrevent_ = CCS_PROT_OTHER_OVERHEAT_INPUT_FILTER_PREVENT;

  lastReasonRun_ = LastReasonRunApvOverheatInputFilter;
  lastReasonStop_ = LastReasonStopOverheatInputFilter;

  protReactEventId_ = OverheatInputFilterProtReactId;
  protBlockedEventId_ = OverheatInputFilterProtBlockedId;
}

ProtectionOverheatInputFilter::~ProtectionOverheatInputFilter()
{

}

bool ProtectionOverheatInputFilter::checkAlarm()
{
  if (parameters.get(CCS_FILTER_INPUT)) {   // Включен входной фильтр
    if (0) {                                // TODO: Условие аварии пока не известно
      protReactEventId_ = OverheatInputFilterProtReactId;
      return true;
    }
  }
  return false;
}

bool ProtectionOverheatInputFilter::checkPrevent()
{
  return alarm_;
}

