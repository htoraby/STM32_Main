#ifndef VSDDANFOSS_H
#define VSDDANFOSS_H

#include "vsd.h"
#include "device_modbus.h"

enum enDanfossStatus_1
{
  VSD_DANFOSS_STATUS_STATE = 11,          //!< Запуск ПЧ
};

enum enDanfossControl
{
  VSD_DANFOSS_CONTROL_DC_BREAK          = 2,
  VSD_DANFOSS_CONTROL_COASTING          = 3,
  VSD_DANFOSS_CONTROL_QUICK_STOP        = 4,
  VSD_DANFOSS_CONTROL_HOLD_FREQUENCY    = 5,
  VSD_DANFOSS_CONTROL_RAMP              = 6,
  VSD_DANFOSS_CONTROL_RESET             = 7,
  VSD_DANFOSS_CONTROL_JOG               = 8,
};

class VsdDanfoss : public Vsd
{
public:
  VsdDanfoss();
  virtual ~VsdDanfoss();

  void initModbusParameters();
  void initParameters();

  void init();
  bool isConnect();
  void getNewValue(uint16_t id);
  uint8_t setNewValue(uint16_t id, float value);

  void writeToDevice(int id, float value);
  void readInDevice(int id);

  void readUfCharacterictic();
  void readUf_F(uint16_t numPoint);


  void writeUf_F(uint16_t numPoint, float value);
  void readUf_U(uint16_t numPoint);
  void writeUf_U(uint16_t numPoint, float value);

  int start();
  int stop(float type);

  int startCoil();
  int stopCoil(float type);

private:
  ModbusParameter modbusParameters_[134];
  DeviceModbus *dm_;

};

#endif // VSDDANFOSS_H
