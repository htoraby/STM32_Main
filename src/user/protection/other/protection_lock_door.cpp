#include "protection_lock_door.h"

ProtectionLockDoor::ProtectionLockDoor()
{
  idMode_= CCS_PROT_OTHER_LOCK_DOOR_MODE;
  idPrevent_= CCS_PROT_OTHER_LOCK_DOOR_PREVENT;
  idActivDelay_ = CCS_PROT_OTHER_LOCK_DOOR_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_OTHER_LOCK_DOOR_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_OTHER_LOCK_DOOR_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_OTHER_LOCK_DOOR_RESTART_LIMIT;
  idRestartResetTime_ = CCS_PROT_OTHER_LOCK_DOOR_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_OTHER_LOCK_DOOR_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_OTHER_LOCK_DOOR_RESTART_SETPOINT;
  idParam_ = CCS_PROT_OTHER_LOCK_DOOR_PARAMETER;
  idRestartFlag_ = CCS_PROT_OTHER_LOCK_DOOR_RESTART_FLAG;
  idBlockFlag_ = CCS_PROT_OTHER_LOCK_DOOR_BLOCK_FLAG;
  idDelayFlag_ = CCS_PROT_OTHER_LOCK_DOOR_DELAY_FLAG;
  idState_ = CCS_PROT_OTHER_LOCK_DOOR_STATE;
  idTimer_ = CCS_PROT_OTHER_LOCK_DOOR_TIME;
  idRestartCount_ = CCS_PROT_OTHER_LOCK_DOOR_RESTART_COUNT;
  idRestartFirstTime_ = CCS_PROT_OTHER_LOCK_DOOR_RESTART_FIRST_TIME;

  lastReasonRun_ = LastReasonRunApvLockDoor;
  lastReasonStop_ = LastReasonStopHackSu;

  protReactEventId_ = LookDoorProtReactId;
  protBlockedEventId_ = LookDoorProtBlockedId;
}

ProtectionLockDoor::~ProtectionLockDoor()
{

}

bool ProtectionLockDoor::checkAlarm()
{
  /*
  uint8_t i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11,i12,i13,i14;
  i1 = getDigitalInput(DI1);
  i2 = getDigitalInput(DI2);
  i3 = getDigitalInput(DI3);
  i4 = getDigitalInput(DI4);
  i5 = getDigitalInput(DI5);
  i6 = getDigitalInput(DI6);
  i7 = getDigitalInput(DI7);
  i8 = getDigitalInput(DI8);
  i9 = getDigitalInput(DI9);
  i10 = getDigitalInput(DI10);
  i11 = getDigitalInput(DI11);
  i12 = getDigitalInput(DI12);
  i13 = getDigitalInput(DI13);
  i14 = getDigitalInput(DI14);
  if ((i5 == 0) || (i1 == 1) || (i2 == 1) || (i3 == 1) || (i4 == 1)
      || (i6 == 1) || (i7 == 1) || (i8 == 1) || (i9 == 1) || (i10 == 1)
      || (i11 == 1) || (i12 == 1) || (i13 == 1) || (i14 == 1)) {
  */
  if (!getDigitalInput(DI5)) {
    protReactEventId_ = LookDoorProtReactId;
    return true;
  }
  return false;
}

bool ProtectionLockDoor::checkPrevent()
{
  return alarm_;
}
