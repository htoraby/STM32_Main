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

  void readUfCharacterictic();
  void readUf_F(uint16_t numPoint);


  void writeUf_F(uint16_t numPoint, float value);
  void readUf_U(uint16_t numPoint);
  void writeUf_U(uint16_t numPoint, float value);
  int start();

private:
  ModbusParameter modbusParameters_[132];
  DeviceModbus *dm_;

};

#endif // VSDDANFOSS_H
