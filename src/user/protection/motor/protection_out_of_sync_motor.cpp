#include "protection_out_of_sync_motor.h"

ProtectionOutOfSyncMotor::ProtectionOutOfSyncMotor()
{
  idReaction_= CCS_PROT_MOTOR_OUT_OF_SYNC_REACTION;
  idActivDelay_ = CCS_PROT_MOTOR_OUT_OF_SYNC_ACTIV_DELAY;
  idTripDelay_ = CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_DELAY;
  idRestartDelay_ = CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_DELAY;
  idRestartLimit_ = CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_LIMIT;
  idRestartReset_ = CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_RESET;
  idTripSetpoint_ = CCS_PROT_MOTOR_OUT_OF_SYNC_TRIP_SETPOINT;
  idRestartSetpoint_ = CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_SETPOINT;
  idParam_ = CCS_PROT_MOTOR_OUT_OF_SYNC_PARAMETER;
  idParam2_ = CCS_PROT_MOTOR_OUT_OF_SYNC_PARAMETER_2;
  idState_ = CCS_PROT_MOTOR_OUT_OF_SYNC_MODE;
  idTimer_ = CCS_PROT_MOTOR_OUT_OF_SYNC_TIME;
  idRestartCount_ = CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_COUNT;
  idRestartResetCount_ = CCS_PROT_MOTOR_OUT_OF_SYNC_RESTART_RESET_COUNT;

  protActivatedEventId_ = OutOfSyncMotorProtActivId;
  apvEventId_ = OutOfSyncMotorApvId;
  apvDisabledEventId_ = OutOfSyncMotorApvDisabledId;
  protBlockedEventId_ = OutOfSyncMotorProtBlockedId;
}

ProtectionOutOfSyncMotor::~ProtectionOutOfSyncMotor()
{

}

void ProtectionOutOfSyncMotor::init()
{
  Protection::init("ProtOutOfSyncMotor");
}

bool ProtectionOutOfSyncMotor::checkAlarm()
{
  float nominal = parameters.getValue(VSD_MOTOR_CURRENT);
  valueParameter2_ = parameters.getValue(CCS_MOTOR_CURRENT_AVARAGE_NOW);
  valueParameter_ = parameters.getValue(CCS_MOTOR_COS_PHI_NOW);

  if ((valueParameter2_ > nominal) && (valueParameter_ < tripSetpoint_))
    return true;
  else
    return false;
}

bool ProtectionOutOfSyncMotor::checkBlock()
{
  return false;
}

void ProtectionOutOfSyncMotor::addEventReactionProt()
{
  float nominal = parameters.getValue(VSD_MOTOR_CURRENT);
  logEvent.add(ProtectCode, AutoType, protActivatedEventId_, valueParameter2_, nominal,
               parameters.getPhysic(VSD_MOTOR_CURRENT));
  logEvent.add(ProtectCode, AutoType, protActivatedEventId_, valueParameter_, tripSetpoint_,
               parameters.getPhysic(idTripSetpoint_));
}
