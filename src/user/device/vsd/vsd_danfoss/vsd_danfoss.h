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

  // ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ДВИГАТЕЛЯ
  int setMotorType(float value);
  int setMotorSpeed(float value);

  // РЕЖИМЫ ПУСКА

  // ЗАДАВАЕМЫЕ ПАРАМЕТРЫ ЧРП
  int setVsdControl(float value);
  int setRotation(float value);  
  int setMinFrequency(float value);
  int setMaxFrequency(float value);
  int setFrequency(float value);
  int setTimeSpeedUp(float value); 
  int setTimeSpeedDown(float value);
  int setSwitchingFrequencyCode(float value);

  int setOutFilter(float value);

  // НАСТРОЙКА U/f
  int setUf_f1(float value);
  int setUf_f2(float value);
  int setUf_f3(float value);
  int setUf_f4(float value);
  int setUf_f5(float value);
  int setUf_f6(float value);
  int setUf_U1(float value);
  int setUf_U2(float value);
  int setUf_U3(float value);
  int setUf_U4(float value);
  int setUf_U5(float value);
  int setUf_U6(float value);

  void readUf_F(uint16_t numPoint);
  void writeUf_F(uint16_t numPoint, float value);
  void readUf_U(uint16_t numPoint);
  void writeUf_U(uint16_t numPoint, float value);
  void readUfCharacterictic();

  uint16_t configVsd();
  uint16_t configVsdAsync();
  uint16_t configVsdVentUf500();
  uint16_t configVsdVentUf1000();
  uint16_t configVsdVentUf3000();
  uint16_t configVsdVentUf6000();
  uint16_t configVsdVentVect500();
  uint16_t configVsdVentVect1000();
  uint16_t configVsdVentVect3000();
  uint16_t configVsdVentVect6000();

  void getNewValue(uint16_t id);
  uint8_t setNewValue(uint16_t id, float value);

  void writeToDevice(int id, float value);
  void readInDevice(int id);

  int start();
  int stop(float type);

  int startCoil();
  int stopCoil(float type);

  int resetSetpoints();

private:
  ModbusParameter modbusParameters_[135];
  DeviceModbus *dm_;

};

#endif // VSDDANFOSS_H
