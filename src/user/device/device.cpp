/*
 * classDevice.cpp
 *
 *  Created on: 23.06.2014
 *      Author: trofimenko
 */

#include "device.h"
#include "user_main.h"

#if USE_EXT_MEM
static float buffer[VSD_BEGIN] __attribute__((section(".extmem")));
#else
static float buffer[VSD_BEGIN];
#endif

float Device::units[PHYSIC_LAST][6][2] =
{
  {{1.0, 0.0},                            // PHYSIC_NUMERIC
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_DATE_TIME
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_TIME TIME_SS
   {0.000001, 0.0},                       // PHYSIC_TIME TIME_US
   {0.001, 0.0},                          // PHYSIC_TIME TIME_MS
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_PERCENT PERCENT_PERCENT
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_LENGHT LENGTH_M
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_DENSITY DENSITY_KG_M3
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_SPACE SPACE_M2
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_FREQUENCY FREQUENCY_HZ
   {1000.0, 0.0},                         // PHYSIC_FREQUENCY FREQUENCY_KHZ
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_VOLTAGE VOLTAGE_V
   {1000.0, 0.0},                         // PHYSIC_VOLTAGE VOLTAGE_KV
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_CURRENT CURRENT_A
   {1000.0, 0.0},                         // PHYSIC_CURRENT CURRENT_MA
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_ACTIVE_POWER ACTIV_POWER_W
   {1000.0, 0.0},                         // PHYSIC_ACTIVE_POWER ACTIV_POWER_KW
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_FULL_POWER FULL_POWER_VA
   {1000.0, 0.0},                         // PHYSIC_FULL_POWER FULL_POWER_KVA
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_REACTIVE_POWER REACTIV_POWER_VAR
   {1000.0, 0.0},                         // PHYSIC_REACTIVE_POWER REACTIV_POWER_KVAR
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_ANGLE ANGLE_DEGREES
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_RPM RPM_RPM
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_TEMP TEMP_HZ_SS
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_RESIST_ISOLATION RESISTANCE_OM
   {1000.0, 0.0},                         // PHYSIC_RESIST_ISOLATION RESISTANCE_KOM
   {1000000.0, 0.0},                      // PHYSIC_RESIST_ISOLATION RESISTANCE_MOM
   {0.001, 0.0},                          // PHYSIC_RESIST_ISOLATION RESISTANCE_MILIOM
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_RESIST_CABLE RESIST_CABLE_KOM_M
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_PRESSURE PRESSURE_MPA
   {0.1013250, 0.0},                      // PHYSIC_PRESSURE PRESSURE_ATM
   {0.0980665, 0.0},                      // PHYSIC_PRESSURE PRESSURE_AT
   {0.1000000, 0.0},                      // PHYSIC_PRESSURE PRESSURE_BAR
   {0.0068948, 0.0},                      // PHYSIC_PRESSURE PRESSURE_PSI
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_TEMPERATURE TEMPERATURE_C
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_ACCELERATION ACCELERATION_G
   {0.101972, 0.0},                       // PHYSIC_ACCELERATION ACCELERATION_MSS2
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_SPEED SPEED_M_SS
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_VISCOSITY VISCOSITY_MPA_SS
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_FLOW FLOW_M3_DD
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_POWER POWER_W
   {1000.0, 0.0},                         // POWER_KW
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_ACTIVE_ENERGY ACTIVE_ENERGY_W_HH
   {1000.0, 0.0},                         // PHYSIC_ACTIVE_ENERGY ACTIVE_ENERGY_KW_HH
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_REACTIVE_ENERGY REACTIVE_ENERGY_VAR_HH
   {1000.0, 0.0},                         // PHYSIC_REACTIVE_ENERGY REACTIVE_ENERGY_KVAR_HH
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_FULL_ENERGY FULL_ENERGY_VAHH
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_RATE RATE_NM
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}},
  {{1.0, 0.0},                            // PHYSIC_INDUNCTANCE INDUNCTANCE_MH
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0},
   {1.0, 0.0}}
};


static void deviceUpdateValueTask(void *p)
{
  (static_cast<Device*>(p))->updateValueTask();
}

Device::Device(uint32_t startAddrParams, parameter *parameters, uint16_t countParameter)
  : startAddrParams_(startAddrParams)
  , parameters_(parameters)
  , countParameter_(countParameter)
{

}

Device::~Device()
{
  osThreadTerminate(updateValueThreadId_);
}

void Device::createThread(const char *threadName)
{
  osMessageQDef(GetValueDeviceQueue, 100, uint32_t);
  getValueDeviceQId_ = osMessageCreate(osMessageQ(GetValueDeviceQueue), NULL);

  // Заполняем структуру для создания задачи
  osThreadDef_t t = {threadName,                  // Название задачи
                     deviceUpdateValueTask,       // Указатель на функцию задачи
                     osPriorityNormal,            // Приоритет задачи
                     0,                           //
                     2 * configMINIMAL_STACK_SIZE // Размер стека задачи
                    };
  updateValueThreadId_ = osThreadCreate(&t, this);// Создаём задачу
}

inline unsigned short Device::getFieldId(unsigned short index)
{
  return parameters_[index].id;
}

inline unsigned char Device::getFieldAccess(unsigned short index)
{
  return parameters_[index].access;
}

inline unsigned char Device::getFieldOperation(unsigned short index)
{
  return parameters_[index].operation;
}

inline unsigned char Device::getFieldPhysic(unsigned short index)
{
  return parameters_[index].physic;
}

inline unsigned char Device::getFieldValidity(unsigned short index)
{
  return parameters_[index].validity;
}

inline float Device::getFieldValue(unsigned short index)
{
  return parameters_[index].value.float_t;
}

inline uint32_t Device::getFieldValueUint32(uint16_t index)
{
  return parameters_[index].value.uint32_t;
}

inline float Device::getFieldMinimum(unsigned short index)
{
  return parameters_[index].min;
}

inline float Device::getFieldMaximum(unsigned short index)
{
  return parameters_[index].max;
}

inline float Device::getFieldDefault(unsigned short index)
{
  return parameters_[index].def;
}

inline parameter Device::getFieldAll(unsigned short index)
{
  return parameters_[index];
}

void Device::setFieldID(unsigned short index, unsigned short id)
{
  parameters_[index].id = id;
}

void Device::setFieldAccess(unsigned short index, unsigned char access)
{
  parameters_[index].access = access;
}

void Device::setFieldOperation(unsigned short index, unsigned char operation)
{
  parameters_[index].operation = operation;
}

void Device::setFieldPhysic(unsigned short index, unsigned char physic)
{
  parameters_[index].physic = physic;
}

void Device::setFieldValidity(unsigned short index, unsigned char validity)
{
  parameters_[index].validity = validity;
}

template<typename T> inline void Device::setFieldValue(const uint16_t &index, const T &value)
{
  parameters_[index].value.uint32_t = *(uint32_t*)&value;
}

void Device::setFieldMin(unsigned short index, float min)
{
  parameters_[index].min = min;
}

void Device::setFieldMax(unsigned short index, float max)
{
  parameters_[index].max = max;
}

void Device::setFieldDef(unsigned short index, float def)
{
  parameters_[index].def = def;
}

void Device::setFieldAll(unsigned short index, parameter param)
{
  parameters_[index] = param;
}

void Device::setFieldValueDef(unsigned short index)
{
  parameters_[index].value.float_t = parameters_[index].def;
}

float Device::applyCoef(float value, float coef)
{
  return value * coef;
}

float Device::applyUnit(float value, int physic, int units)
{
  return (value * Device::units[physic][units][0] + Device::units[physic][units][1]);
}

unsigned short Device::getIndexAtId(unsigned short id)
{
  if ((id >= startAddrParams_) && (id < (startAddrParams_ + countParameter_))) {
    if (getFieldId(id - startAddrParams_) == id)
      return (id - startAddrParams_);
  }

  // TODO: Предупреждение о не найденом параметре

  return 0;
}

float Device::getValue(unsigned short id, bool *ok)
{
  uint16_t index = getIndexAtId(id);
  *ok = (getFieldValidity(index) == VALIDITY_OK) ? true : false;
  return getFieldValue(index);
}

uint32_t Device::getValueUint32(unsigned short id, bool *ok)
{
  uint16_t index = getIndexAtId(id);
  *ok = (getFieldValidity(index) == VALIDITY_OK) ? true : false;
  return getFieldValueUint32(index);
}

uint8_t Device::setValue(uint16_t id, float value)
{
  //TODO: Добавить проверки на корректность записываемых данных

  uint16_t index = getIndexAtId(id);
  float valueOld = getFieldValue(index);

  setFieldValue(index, value);

  // Сообщить контроллеру визуализации об обновлении параметра
  if (value != valueOld)
    novobusSlave.putMessageParams(id);
  return 0;
}

uint8_t Device::setValue(uint16_t id, uint32_t value)
{
  uint16_t index = getIndexAtId(id);
  uint32_t valueOld = getFieldValueUint32(index);

  setFieldValue(index, value);

  // Сообщить контроллеру визуализации об обновлении параметра
  if (value != valueOld)
    novobusSlave.putMessageParams(id);
  return 0;
}

uint8_t Device::setValue(uint16_t id, int value)
{
  return setValue(id, (float)value);
}

uint8_t Device::getPhysic(unsigned short id)
{
  return getFieldPhysic(getIndexAtId(id));
}

void Device::updateValueTask()
{
  osEvent event;
  while (1) {
    osDelay(1);

    event = osMessageGet(getValueDeviceQId_, 0);
    if (event.status == osEventMessage)
      getNewValue(event.value.v);
  }
}

void Device::getNewValue(uint16_t)
{

}

void Device::calcParameters(uint16_t id)
{

}

uint8_t Device::setNewValue(uint16_t id, float value)
{
  return setValue(id, value);
}

uint8_t Device::setNewValue(uint16_t id, uint32_t value)
{
  return setValue(id, value);
}

uint8_t Device::setNewValue(uint16_t id, int value)
{
  return setValue(id, value);
}

StatusType Device::saveParameters()
{
  // Проверка на завершение работы DMA
  framReadData(startAddrParams_*4, (uint8_t *)buffer, 1*4);

  for (int i = 0; i < countParameter_; ++i) {
    buffer[i] = parameters_[i].value.float_t;
  }

  StatusType status = framWriteData(startAddrParams_*4,
                                    (uint8_t *)buffer,
                                    countParameter_*4);
  if (status == StatusError)
    asm("nop");
  return status;
}

StatusType Device::readParameters()
{
  StatusType status = framReadData(startAddrParams_*4,
                                   (uint8_t *)buffer,
                                   countParameter_*4);
  if (status == StatusError)
    asm("nop");

  for (int i = 0; i < countParameter_; ++i) {
    parameters_[i].value.float_t = buffer[i];
  }

  return status;
}
