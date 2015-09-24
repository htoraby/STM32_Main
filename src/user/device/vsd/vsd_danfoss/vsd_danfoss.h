#ifndef VSDDANFOSS_H
#define VSDDANFOSS_H

#include "vsd.h"
#include "device_modbus.h"

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

private:
  ModbusParameter modbusParameters_[132];
  DeviceModbus *dm_;

};

#endif // VSDDANFOSS_H
