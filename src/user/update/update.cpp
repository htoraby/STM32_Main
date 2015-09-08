/**
  ******************************************************************************
  * @file    src/command.c
  * @author  MCD Application Team
  * @author  Shilyaev E.V.
  * @version V2.0.0
  * @date    11.09.2014
  * @brief   This file provides all the IAP command functions.
  ******************************************************************************
  */

#include "update.h"
#include "flash_ext.h"
#include "usb_host.h"
#include "ff.h"
#include "log_main.h"

// 4 Kbytes
#define BUFFER_SIZE ((uint16_t)512*8)

#if USE_EXT_MEM
static uint8_t buffer[BUFFER_SIZE] __attribute__((section(".extmem")));
#else
static uint8_t buffer[BUFFER_SIZE];
#endif

static UPDATE_HEADER updateHeader;

static void getFile(char *fileName)
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

  result = f_opendir(&dir, "/");
  if (result == FR_OK) {
    while (1) {
      result = f_readdir(&dir, &fileInfo);
      if (result != FR_OK || fileInfo.fname[0] == 0)
        break;
#if _USE_LFN
      fn = *fileInfo.lfname ? fileInfo.lfname : fileInfo.fname;
#else
      fn = fileInfo.fname;
#endif
      if (strstr(fn, SW_FILE_NAME)) {
        if (strstr(fn, ".bin")) {
          strcpy(fileName, fn); // файл прошивки найден
          return;
        }
      }
    }
    logDebug.add(WarningMsg, "Update. Файл прошивки не найден");
  }
  else {
    logDebug.add(WarningMsg, "Update. Не удалось открыть каталог");
  }
}

static bool saveSwInFlashExt(char *fileName)
{
  static UINT readSize = 0x00;
  static uint8_t readflag = 1;
  static FIL file;
  static IMAGE_FILE_HEADER imageHeader;
  bool isSaveSw = false;
  uint16_t calcCrc = 0xFFFF;
  static uint32_t lastAddress;
  if (updateHeader.numRegion == 0) {
    updateHeader.numRegion = 1;
    lastAddress = AddrUpdateFile2;
  }
  else {
    updateHeader.numRegion = 0;
    lastAddress = AddrUpdateFile1;
  }

  // Открытие файла прошивки и сохранение на внешнию flash
  if (f_open(&file, fileName, FA_READ) == FR_OK) {
    f_read(&file, &imageHeader, sizeof(imageHeader), &readSize);

    if ((readSize == sizeof(imageHeader)) &&
        (imageHeader.size == file.fsize) &&
        (imageHeader.codeProduction == CODE_PRODUCTION) &&
        (imageHeader.codeEquip == CODE_EQUIP) &&
        (imageHeader.subCodeEquip == SUBCODE_EQUIP)) {
      calcCrc = crc16_ibm((uint8_t*)&imageHeader, readSize, calcCrc);
      flashExtWriteEx(FlashSpi1, lastAddress, (uint8_t*)&imageHeader, readSize);
      lastAddress = lastAddress + readSize;

      while ((readflag == 1) && (usbState == USB_READY)) {
        f_read(&file, buffer, BUFFER_SIZE, &readSize);
        if (readSize < BUFFER_SIZE)
          readflag = 0;

        if (readflag)
          calcCrc = crc16_ibm(buffer, readSize, calcCrc);
        else
          calcCrc = crc16_ibm(buffer, readSize-6, calcCrc);
        flashExtWriteEx(FlashSpi1, lastAddress, buffer, readSize);
        lastAddress = lastAddress + readSize;
      }

      uint16_t crc = (buffer[readSize - 1 - 4] << 8) + buffer[readSize - 2 - 4];
      uint32_t finish = (buffer[readSize - 1] << 24) + (buffer[readSize - 2] << 16) +
          (buffer[readSize - 3] << 8) + (buffer[readSize - 4]);
      if ((calcCrc == crc) && (finish == 0xFFFFFFF1))
        isSaveSw = true;
      else
        logDebug.add(WarningMsg, "Update. Ошибка CRС - %h %h", calcCrc, finish);
    }
    else {
      logDebug.add(WarningMsg, "Update. Ошибка в загаловке файла прошивки - %d %d %d %d %d",
                   readSize, imageHeader.size, imageHeader.codeProduction,
                   imageHeader.codeEquip, imageHeader.subCodeEquip);
    }

    f_close(&file);
  }
  else {
    logDebug.add(WarningMsg, "Update. Ошибка открытия файла прошивки");
  }

  return isSaveSw;
}

bool updateFromUsb()
{
  static char fileName[_MAX_LFN] = {0};

  // TODO: Падение
  return false;

  if (usbState != USB_READY) {
    logDebug.add(WarningMsg, "Update. Не подключен USB накопитель");
    return false;
  }

  getFile(fileName);
  if (!strlen(fileName)) {
    return false;
  }

  flashExtRead(FlashSpi1, AddrUpdateHeader, (uint8_t*)&updateHeader, sizeof(updateHeader));

  if (saveSwInFlashExt(fileName)) {
//    updateHeader.flag = 0x5A;
//    updateHeader.type = TypeNewUpdate;
//    flashExtWriteEx(FlashSpi1, AddrUpdateHeader, (uint8_t*)&updateHeader, sizeof(updateHeader));
//    return true;
  }

  return false;
}
