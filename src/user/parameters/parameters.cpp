#include "parameters.h"
#include "user_main.h"
#include "gpio.h"
#include "usb_host.h"
#include "update.h"

#include <string.h>

#define MAX_QUEUE_SIZE_PARAMS 100
#define BUFFER_SIZE 512*8
#define CRC_PARAMS_SIZE 8*4

#define CONFIG_DIR USB_DISK ":ksu_data"
#define FILE_MASK "%02d%02d%02d_%02d%02d%02d.cfg"

#if USE_EXT_MEM
static uint8_t buffer[BUFFER_SIZE] __attribute__((section(".extmem")));
#else
static uint8_t buffer[BUFFER_SIZE];
#endif

Parameters::Parameters()
  : isBanSaveConfig_(false)
{

}

Parameters::~Parameters()
{

}

static void parametersTask(void *p)
{
  (static_cast<Parameters*>(p))->task();
}

static void parametersSetDelayTask(void *p)
{
  (static_cast<Parameters*>(p))->setDelayTask();
}

void Parameters::init()
{
  semaphoreId_ = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(semaphoreId_, 0);

  osThreadDef(SaveParameters, parametersTask, osPriorityNormal, 0, 2*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(SaveParameters), this);

  osThreadDef(SetDelayTask, parametersSetDelayTask, osPriorityNormal, 0, 3*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(SetDelayTask), this);

  // Создаём очередь сообщений id параметров
  osMessageQDef(MessageIdParams, MAX_QUEUE_SIZE_PARAMS, uint32_t);
  messageIdParams_ = osMessageCreate(osMessageQ(MessageIdParams), NULL);
  // Создаём очередь сообщений значений параметров
  osMessageQDef(MessageValueParams, MAX_QUEUE_SIZE_PARAMS, uint32_t);
  messageValueParams_ = osMessageCreate(osMessageQ(MessageValueParams), NULL);
}

void Parameters::task()
{
  static int time = 0;
  while (1) {
    osDelay(1);

    if (isBanSaveConfig_)
      continue;

    if ((osSemaphoreWait(semaphoreId_, 0) != osEventTimeout) || ksu.isPowerOff()) {
      time = 0;
      save();
    }

    if (++time >= PARAMS_SAVE_TIME) {
      time = 0;
      save();
    }

    while (ksu.isPowerOff()) {
      osDelay(1);
    }
  }
}

void Parameters::startSave()
{
  osSemaphoreRelease(semaphoreId_);
}

void Parameters::save()
{  
  ksu.saveParameters();
  vsd->saveParameters();
  tms->saveParameters();
  em->saveParameters();
}

void Parameters::read()
{
  ksu.readParameters();
  vsd->readParameters();
  tms->readParameters();
  em->readParameters();
}

float Parameters::get(unsigned short id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getValue(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getValue(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getValue(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getValue(id);

  return 0;
}



uint32_t Parameters::getU32(unsigned short id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getValueUint32(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getValueUint32(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getValueUint32(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getValueUint32(id);

  return 0;
}

uint32_t Parameters::getDiscret(unsigned short id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getDiscret(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getDiscret(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getDiscret(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getDiscret(id);

  return 0;
}

int Parameters::set(unsigned short id, float value, EventType eventType)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.setNewValue(id, value, eventType);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->setNewValue(id, value, eventType);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->setNewValue(id, value, eventType);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->setNewValue(id, value, eventType);

  return 0;
}

int Parameters::set(unsigned short id, uint32_t value, EventType eventType)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.setNewValue(id, value, eventType);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->setNewValue(id, value, eventType);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->setNewValue(id, value, eventType);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->setNewValue(id, value, eventType);

  return 0;
}

int Parameters::set(unsigned short id, int value, EventType eventType)
{
  return set(id, (float)value, eventType);
}

uint8_t Parameters::getPhysic(unsigned short id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getPhysic(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getPhysic(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getPhysic(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getPhysic(id);

  return 0;
}

uint8_t Parameters::getValidity(unsigned short id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getValidity(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getValidity(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getValidity(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getValidity(id);

  return err_r;
}

bool Parameters::isValidity(uint16_t id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return !(ksu.getValidity(id) == VALIDITY_ERROR);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return !(vsd->getValidity(id) == VALIDITY_ERROR);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return !(tms->getValidity(id) == VALIDITY_ERROR);
  if ((id > EM_BEGIN) && (id < EM_END))
    return !(em->getValidity(id) == VALIDITY_ERROR);

  return false;
}

void Parameters::setValidity(uint16_t id, uint8_t validity)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    ksu.setValidity(id, validity);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    vsd->setValidity(id, validity);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    tms->setValidity(id, validity);
  if ((id > EM_BEGIN) && (id < EM_END))
    em->setValidity(id, validity);
}

float Parameters::getMin(uint16_t id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getMin(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getMin(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getMin(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getMin(id);

  return err_r;
}

uint8_t Parameters::setMin(uint16_t id, float value)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.setMin(id, value);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->setMin(id, value);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->setMin(id, value);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->setMin(id, value);

  return err_r;
}

float Parameters::getMax(uint16_t id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getMax(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getMax(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getMax(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getMax(id);

  return err_r;
}

float Parameters::getValueDef(uint16_t id)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.getValueDef(id);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->getValueDef(id);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->getValueDef(id);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->getValueDef(id);

  return err_r;
}

uint8_t Parameters::setMax(uint16_t id, float value)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    return ksu.setMax(id, value);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    return vsd->setMax(id, value);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    return tms->setMax(id, value);
  if ((id > EM_BEGIN) && (id < EM_END))
    return em->setMax(id, value);

  return err_r;
}

void Parameters::setAccess(uint16_t id, uint8_t access)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    ksu.setAccess(id, access);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    vsd->setAccess(id, access);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    tms->setAccess(id, access);
  if ((id > EM_BEGIN) && (id < EM_END))
    em->setAccess(id, access);
}

void Parameters::setOperation(uint16_t id, uint8_t operation)
{
  if ((id > CCS_BEGIN) && (id < CCS_END))
    ksu.setOperation(id, operation);
  if ((id > VSD_BEGIN) && (id < VSD_END))
    vsd->setOperation(id, operation);
  if ((id > TMS_BEGIN) && (id < TMS_END))
    tms->setOperation(id, operation);
  if ((id > EM_BEGIN) && (id < EM_END))
    em->setOperation(id, operation);
}

float Parameters::checkZero(unsigned short id, bool reset, float value)
{
  if (parameters.get(id) == 0) {
    if (reset) {
      if (value == 0) {
        if (parameters.getValueDef(id) == 0) {
          if (parameters.getMin(id) == 0) {
#if (USE_LOG_DEBUG == 1)
            logDebug.add(DebugMsg, "Parameters::checkZero() (id = %d)");
#endif
            return 1;
          }
          else {
            parameters.set(id, parameters.getMin(id));
          }
        }
        else {
          parameters.set(id, parameters.getValueDef(id));
        }
      }
      else {
        parameters.set(id, value);
      }
    }
    else {
      if (value == 0) {
        return 1;
      }
      else {
        return value;
      }
    }
  }
  return parameters.get(id);
}

void Parameters::setDelayTask()
{
  osEvent event;
  while (1) {
    osDelay(1);

    event = osMessageGet(messageIdParams_, 0);
    if (event.status == osEventMessage) {
      EventType eventType = EventType(event.value.v >> 16);
      uint16_t id = event.value.v;
      event = osMessageGet(messageValueParams_, 0);
      if (event.status == osEventMessage) {
        unTypeData value;
        value.uint32_t = event.value.v;
        if (getPhysic(id) == PHYSIC_DATE_TIME)
          set(id, (uint32_t)value.uint32_t, eventType);
        else
          set(id, value.float_t, eventType);
      }
    }
  }
}

void Parameters::setDelay(uint16_t id, uint32_t value, EventType eventType)
{
  uint32_t message = (eventType << 16) + id;
  osMessagePut(messageIdParams_, message, 0);
  osMessagePut(messageValueParams_, value, 0);
}

void Parameters::saveConfig()
{
  int profile = parameters.get(CCS_CMD_SAVE_SETPOINT);
  int saveUsb = parameters.get(CCS_CMD_SAVE_CONFIG_USB);

  if ((profile >= 1) && (profile <= 5)) {
    saveConfigProfile(profile);
    parameters.set(CCS_CMD_SAVE_SETPOINT, 0.0);
  }
  else if (saveUsb) {
    saveConfigUsb();
    parameters.set(CCS_CMD_SAVE_CONFIG_USB, 0.0);
  }
}

void Parameters::saveConfigProfile(int profile)
{
  int time = HAL_GetTick();
  logEvent.add(OtherCode, OperatorType, SaveConfigId, 0, profile);

  uint32_t save = parameters.getU32(CCS_SAVE_SETPOINT);
  save |= (1 << (profile-1));
  parameters.set(CCS_SAVE_SETPOINT, save);

  uint32_t address;
  switch (profile) {
  case 1: address = AddrSaveConfig1; break;
  case 2: address = AddrSaveConfig2; break;
  case 3: address = AddrSaveConfig3; break;
  case 4: address = AddrSaveConfig4; break;
  case 5: address = AddrSaveConfig5; break;
  default: address = AddrSaveConfig1; break;
  }
  ksu.saveConfig(address);
  vsd->saveConfig(address);
  tms->saveConfig(address);
  em->saveConfig(address);

  logDebug.add(WarningMsg, "Parameters::saveConfigProfile() Save setpoints (%d) - completed %d ms",
               profile, HAL_GetTick() - time);
}

void Parameters::getFilePath(char *path)
{
  char buf[_MAX_LFN + 1];
  char *fileName = buf;

  time_t time = parameters.getU32(CCS_DATE_TIME);
  tm dateTime = *localtime(&time);
  if (dateTime.tm_year > 100)
    dateTime.tm_year = dateTime.tm_year - 100;
  else
    dateTime.tm_year = 0;

  sprintf(fileName, FILE_MASK, dateTime.tm_year, dateTime.tm_mon + 1, dateTime.tm_mday, dateTime.tm_hour, dateTime.tm_min, dateTime.tm_sec);
  convert_utf8_to_windows1251(fileName, fileName, _MAX_LFN + 1);

  strcat(path, "\\");
  strcat(path, fileName);
}

bool Parameters::saveConfigUsb()
{
  int time = HAL_GetTick();
  logEvent.add(OtherCode, OperatorType, SaveConfigUsbId);

  startSave();
  osDelay(100);

  uint32_t timeReady = 0;
  while(!usbIsReady()) {
    osDelay(10);
    timeReady += 10;
    if (timeReady > 5000) {
      ksu.setError(NoConnectionUsbErr);
      return false;
    }
  }

  FRESULT result;
  FIL file;
  UINT bytesWritten;
  char buf[_MAX_LFN + 1];
  char *fileName = buf;

  result = f_mkdir(CONFIG_DIR);
  if ((result == FR_OK) || (result == FR_EXIST)) {
    strcpy(fileName, CONFIG_DIR);
    getFilePath(fileName);

    if (f_open(&file, fileName, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
      uint16_t calcCrc = 0xFFFF;
      uint32_t addr = 0;
      uint32_t size = BUFFER_SIZE;

      CFG_FILE_HEADER header;
      header.codeProduction = CODE_PRODUCTION;
      header.codeEquip = CODE_EQUIP;
      header.subCodeEquip = parameters.get(CCS_TYPE_VSD);
      header.version = FIRMWARE_VERSION;
      time_t time = parameters.getU32(CCS_DATE_TIME);
      tm dateTime = *localtime(&time);
      if (dateTime.tm_year > 100)
        dateTime.tm_year = dateTime.tm_year - 100;
      else
        dateTime.tm_year = 0;
      header.date = (toBcd(dateTime.tm_year) << 24) |
          (toBcd(dateTime.tm_mon + 1) << 16) |
          (toBcd(dateTime.tm_mday) << 8) |
          (toBcd(dateTime.tm_hour) & 0xFF);
      // Размер заголовка + размер всех параметров + размер CRC параметров и их количества + общая CRC
      header.size = sizeof(header) + PARAMETERS_SIZE + CRC_PARAMS_SIZE + 2;

      result = f_write(&file, (uint8_t*)&header, sizeof(header), &bytesWritten);
      if ((result != FR_OK) || (sizeof(header) != bytesWritten)) {
        f_close(&file);
        ksu.setError(WriteFileUsbErr);
        return false;
      }
      calcCrc = crc16_ibm((uint8_t*)&header, bytesWritten, calcCrc);

      isBanSaveConfig_ = true;
      while (usbIsReady()) {
        StatusType status = framReadData(addr, buffer, size);
        if (status == StatusError)
          asm("nop");
        addr = addr + size;
        calcCrc = crc16_ibm(buffer, size, calcCrc);

        result = f_write(&file, buffer, size, &bytesWritten);
        if ((result != FR_OK) || (bytesWritten != size)) {
          f_close(&file);
          ksu.setError(WriteFileUsbErr);
          return false;
        }

        if (addr >= PARAMETERS_SIZE)
          break;
      }

      addr = CcsParamsCountAddrFram;
      size = CRC_PARAMS_SIZE;
      StatusType status = framReadData(addr, buffer, size);
      if (status == StatusError)
        asm("nop");
      calcCrc = crc16_ibm(buffer, size, calcCrc);

      result = f_write(&file, buffer, size, &bytesWritten);
      if ((result != FR_OK) || (bytesWritten != size)) {
        f_close(&file);
        ksu.setError(WriteFileUsbErr);
        return false;
      }

      isBanSaveConfig_ = false;

      result = f_write(&file, (uint8_t*)&calcCrc, sizeof(calcCrc), &bytesWritten);
      if ((result != FR_OK) || (sizeof(calcCrc) != bytesWritten)) {
        f_close(&file);
        ksu.setError(WriteFileUsbErr);
        return false;
      }

      result = f_close(&file);
      if (result != FR_OK) {
        ksu.setError(CloseFileUsbErr);
        return false;
      }
    } else {
      ksu.setError(OpenFileUsbErr);
      return false;
    }
  } else {
    ksu.setError(MkDirUsbErr);
    return false;
  }

  logDebug.add(WarningMsg, "Parameters::saveConfigUsb() Save config USB - completed %d ms",
               HAL_GetTick() - time);
  return true;
}

void Parameters::loadConfig()
{
  int profile = parameters.get(CCS_CMD_LOAD_SETPOINT);
  int loadUsb = parameters.get(CCS_CMD_LOAD_CONFIG_USB);

  if ((profile >= 1) && (profile <= 5)) {
    loadConfigProfile(profile);
    parameters.set(CCS_CMD_LOAD_SETPOINT, 0.0);
  }
  else if (loadUsb) {
    loadConfigUsb();
    parameters.set(CCS_CMD_LOAD_CONFIG_USB, 0.0);
  }
}

void Parameters::loadConfigProfile(int profile)
{
  int time = HAL_GetTick();
  logEvent.add(OtherCode, OperatorType, LoadConfigId, 0, profile);

  uint32_t address;
  switch (profile) {
  case 1: address = AddrSaveConfig1; break;
  case 2: address = AddrSaveConfig2; break;
  case 3: address = AddrSaveConfig3; break;
  case 4: address = AddrSaveConfig4; break;
  case 5: address = AddrSaveConfig5; break;
  default: address = AddrSaveConfig1; break;
  }
  ksu.loadConfig(address);
  vsd->loadConfig(address);

  logDebug.add(WarningMsg, "Parameters::loadConfig() Load setpoints (%d) - completed %d ms",
               profile, HAL_GetTick() - time);
}

void Parameters::getConfigFile(char *fileName)
{
  FRESULT result;
  DIR dir;
  FILINFO fileInfo;
  char *fn;
#if _USE_LFN
  static char lfn[_MAX_LFN + 1];
  fileInfo.lfname = lfn;
  fileInfo.lfsize = sizeof(lfn);
#endif

  result = f_opendir(&dir, CONFIG_DIR);
  if (result == FR_OK) {
    while (1) {
      result = f_readdir(&dir, &fileInfo);
      if ((result != FR_OK) || (fileInfo.fname[0] == 0))
        break;
#if _USE_LFN
      fn = *fileInfo.lfname ? fileInfo.lfname : fileInfo.fname;
#else
      fn = fileInfo.fname;
#endif
      if (strstr(fn, ".cfg")) {
        if (strlen(fileName)) {
          logDebug.add(WarningMsg, "Parameters::getFile() Multiple configuration files");
          ksu.setError(MultipleConfigFilesErr);
          fileName[0] = 0;
          f_closedir(&dir);
          return;
        }
        strcpy(fileName, CONFIG_DIR);
        strcat(fileName, "\\");
        strcat(fileName, fn);
      }
    }
    if (strlen(fileName)) {
      f_closedir(&dir);
      return; // файл конфигурации найден
    }
    logDebug.add(WarningMsg, "Parameters::getFile() Configuration file not found");
    ksu.setError(NotFoundConfigFileErr);
  }
  else {
    logDebug.add(WarningMsg, "Parameters::getFile() Failed to open directory");
    ksu.setError(OpenDirUsbErr);
  }
  f_closedir(&dir);
}

bool Parameters::loadConfigUsb()
{
  int time = HAL_GetTick();
  logEvent.add(OtherCode, OperatorType, LoadConfigUsbId);

  uint32_t timeReady = 0;
  while(!usbIsReady()) {
    osDelay(10);
    timeReady += 10;
    if (timeReady > 5000) {
      ksu.setError(NoConnectionUsbErr);
      return false;
    }
  }

  FRESULT result;
  FIL file;
  static char buf[_MAX_LFN + 1];
  buf[0] = 0;
  char *fileName = buf;
  UINT readSize = 0x00;
  uint16_t calcCrc = 0xFFFF;
  uint32_t allSize = 0;
  uint32_t addr = 0;

  getConfigFile(fileName);
  if (!strlen(fileName)) {
    return false;
  }

  // Открытие файла конфигурации
  result = f_open(&file, fileName, FA_READ);
  if (result == FR_OK) {
    // Проверка контрольной суммы
    while (usbIsReady()) {
      osDelay(1);
      result = f_read(&file, buffer, BUFFER_SIZE, &readSize);
      calcCrc = crc16_ibm(buffer, readSize, calcCrc);
      if (readSize < BUFFER_SIZE)
        break;
    }

    if (!calcCrc) {
      f_lseek(&file, 0);
      CFG_FILE_HEADER header;
      f_read(&file, &header, sizeof(header), &readSize);
      allSize += readSize;
      // Проверка заголовка
      if ((readSize == sizeof(header)) &&
          (header.size == file.fsize) &&
          (header.codeProduction == CODE_PRODUCTION) &&
          (header.codeEquip == CODE_EQUIP) &&
          (header.subCodeEquip == parameters.get(CCS_TYPE_VSD))) {

        // Чтение и сохранение конфигурации
        isBanSaveConfig_ = true;
        while (usbIsReady()) {
          osDelay(1);

          f_read(&file, buffer, BUFFER_SIZE, &readSize);
          StatusType status = framWriteData(addr, buffer, readSize);
          if (status == StatusError)
            logDebug.add(CriticalMsg, "Parameters::loadConfigUsb() Error save configuration (addr = %d)", addr);
          addr = addr + readSize;

          allSize += readSize;
          if (allSize >= (file.fsize - CRC_PARAMS_SIZE - 2))
            break;
        }

        f_read(&file, buffer, CRC_PARAMS_SIZE, &readSize);
        addr = CcsParamsCountAddrFram;
        StatusType status = framWriteData(addr, buffer, readSize);
        if (status == StatusError)
          logDebug.add(CriticalMsg, "Parameters::loadConfigUsb() Error save CRC (addr = %d)", addr);

        f_close(&file);
        logDebug.add(WarningMsg, "Parameters::loadConfigUsb() Load config USB (%d) - completed %d ms",
                     HAL_GetTick() - time);
        ksu.startReboot(false);
        return true;
      }
      else {
        logDebug.add(WarningMsg,
                     "Parameters::loadConfigUsb() Error in anything of configuration file (readSize = %d; size = %d; Production = %d; Equip = %d; %d)",
                     readSize, header.size, header.codeProduction,
                     header.codeEquip, header.subCodeEquip);
        ksu.setError(HeaderConfigFileErr);
      }
    }
    else {
      logDebug.add(WarningMsg,
                   "Parameters::loadConfigUsb() CRC error in configuration file");
      ksu.setError(CrcConfigFileErr);
    }
  }
  else {
    logDebug.add(WarningMsg, "Parameters::loadConfigUsb() Error opening configuration file");
    ksu.setError(OpenFileUsbErr);
  }

  f_close(&file);
  return false;
}

void Parameters::setProfileDefaultSetpoint()
{
  int profile = parameters.get(CCS_PROFILE_DEFAULT_SETPOINT);
  if ((profile < 1) || (profile > 5))
    return;

  int time = HAL_GetTick();
  uint32_t address;

  switch (profile) {
  case 1: address = AddrSaveConfig1; break;
  case 2: address = AddrSaveConfig2; break;
  case 3: address = AddrSaveConfig3; break;
  case 4: address = AddrSaveConfig4; break;
  case 5: address = AddrSaveConfig5; break;
  default: address = AddrSaveConfig1; break;
  }
  ksu.loadConfigInProfileDefault(address, profileDefaultParams_);
  vsd->loadConfigInProfileDefault(address, profileDefaultParams_);

  logDebug.add(WarningMsg, "Parameters::setProfileDefaultSetpoint() Сompleted %d ms", HAL_GetTick() - time);
}

void Parameters::setAllDefault()
{
  logEvent.add(SetpointResetCode, OperatorType, DefaultSetpointResetId);

  int profile = parameters.get(CCS_PROFILE_DEFAULT_SETPOINT);
  float mtrType = parameters.get(CCS_MOTOR_TYPE);

  switch (profile) {
  case NovometDefaultSetpoint:
    for (int i = 0; i < COUNT_PARAMETERS_DEFAULT; ++i) {
      set(defaultParams[i][0], getValueDef(defaultParams[i][0]), NoneType);
    }
    break;
  case RosneftDefaultSetpoint:
    for (int i = 0; i < COUNT_PARAMETERS_DEFAULT; ++i) {
      if (mtrType == VSD_MOTOR_TYPE_ASYNC)
        set(defaultParams[i][0], defaultParams[i][1], NoneType);
      else
        set(defaultParams[i][0], defaultParams[i][2], NoneType);
    }
    break;
  case Profile1DefaultSetpoint: case Profile2DefaultSetpoint:
  case Profile3DefaultSetpoint: case Profile4DefaultSetpoint:
  case Profile5DefaultSetpoint:
    for (int i = 0; i < COUNT_PARAMETERS_DEFAULT; ++i) {
      set(defaultParams[i][0], profileDefaultParams_[i], NoneType);
    }
    break;
  }
}

void Parameters::setDefault(uint16_t id)
{
  int profile = parameters.get(CCS_PROFILE_DEFAULT_SETPOINT);
  float mtrType = parameters.get(CCS_MOTOR_TYPE);

  switch (profile) {
  case NovometDefaultSetpoint:
    set(id, getValueDef(id), NoneType);
    break;
  case RosneftDefaultSetpoint:
    for (int i = 0; i < COUNT_PARAMETERS_DEFAULT; ++i) {
      if (defaultParams[i][0] == id) {
        if (mtrType == VSD_MOTOR_TYPE_ASYNC)
          set(defaultParams[i][0], defaultParams[i][1], NoneType);
        else
          set(defaultParams[i][0], defaultParams[i][2], NoneType);
        return;
      }
    }
    set(id, getValueDef(id), NoneType);
    break;
  case Profile1DefaultSetpoint: case Profile2DefaultSetpoint:
  case Profile3DefaultSetpoint: case Profile4DefaultSetpoint:
  case Profile5DefaultSetpoint:
    for (int i = 0; i < COUNT_PARAMETERS_DEFAULT; ++i) {
      if (defaultParams[i][0] == id) {
        set(defaultParams[i][0], profileDefaultParams_[i], NoneType);
        return;
      }
    }
    set(id, getValueDef(id), NoneType);
    break;
  }
}
