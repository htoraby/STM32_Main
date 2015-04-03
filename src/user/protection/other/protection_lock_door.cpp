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
