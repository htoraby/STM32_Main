#ifndef UPDATE_H
#define UPDATE_H

#include <stdint.h>

#include "flash_ext.h"

#define SW_FILE_NAME "505.013.1000-SW.31"

#define CODE_PRODUCTION 1
#define CODE_EQUIP      1
#define SUBCODE_EQUIP   1

#pragma pack(1)

typedef struct {
  unsigned int size;
  unsigned short codeProduction;
  unsigned char codeEquip;
  unsigned char subCodeEquip;
  unsigned short version;
  unsigned int date;
  unsigned int swMainSize;
  unsigned int swGuiSize;
  unsigned short swMainCrc16;
  unsigned short swGuiCrc16;
} IMAGE_FILE_HEADER;

#pragma pack()

typedef struct {
  uint8_t flag;
  uint8_t type;
  uint8_t numRegion;
} UPDATE_HEADER;

typedef enum {
  TypeNoUpdate,
  TypeFirstUpdate,
  TypeNewUpdate,
  TypeOldUpdate,
} TypeUpdate;

/*!
  * \brief Адреса на FLASH
 */
typedef enum {
  //! FlashSpi1
  AddrUpdateHeader = 0x00800000,
  AddrUpdateFile1  = 0x00800100,
  AddrUpdateFile2  = 0x00C00000,
} AddrFlashSpi;

/*!
 * \brief Метод получения прошивки с Usb flash и сохранения на внешнюю flash
 * \return
 */
bool updateFromUsb();

/*!
 * \brief Чтение прошивки с внешней flash для Master
 * \param address
 * \param data
 * \param size
 * \return
 */
StatusType readSwFromFlashExt(uint32_t address, uint8_t *data, uint32_t size);

/*!
 * \brief Запись прошивки на внешнию flash от Master
 * \param address
 * \param data
 * \param size
 * \return
 */
StatusType writeSwInFlashExt(uint32_t address, uint8_t *data, uint32_t size);

#endif  /* UPDATE_H */
