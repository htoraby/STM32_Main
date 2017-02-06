#ifndef _MB_SLAVE_SNG_H
#define _MB_SLAVE_SNG_H

#include "port.h"

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

#include "mb.h"
#include "mbport.h"
#include "mbproto.h"

/*! \defgroup modbus SNG Modbus SNG
 * \code #include "mb_slave_sng.h" \endcode
 * Унифирцированый протокол обмена Сургутнефтегаз.
 * За основу был взят код FreeModbus
 */

#define MB_SNG_FUNC_MAX 5

#define MB_SNG_SIZE_MIN  6       //!< Минимальный размер пакета
#define MB_SNG_SIZE_MAX  256     //!< Максимальный размер пакета
#define MB_SNG_SIZE_CRC  2       //!< Size of CRC field in PDU
#define MB_SNG_START_OFF 0
#define MB_SNG_ADDR_OFF  1
#define MB_SNG_FUNC_OFF  2
#define MB_SNG_DATA_FUNC_OFF  0

typedef enum
{
  STATE_ENABLED,
  STATE_DISABLED,
  STATE_NOT_INITIALIZED
} eMbSngState;

typedef enum
{
  MB_SNG_EX_NONE = 0x00,
  MB_SNG_EX_ERROR_CRC = 0x01,
  MB_SNG_EX_ILLEGAL_FUNCTION = 0x02,
  MB_SNG_EX_ILLEGAL_DATA = 0x03,
  MB_SNG_EX_CMD_BLOCK = 0x04,
  MB_SNG_EX_ERROR_WRITE = 0x05,
  MB_SNG_EX_ERROR_LINK = 0x06,
  MB_SNG_EX_ILLEGAL_LEN = 0x07,
  MB_SNG_EX_LOCAL_MODE = 0x08,
} eMbSngException;

typedef enum
{
  STATE_RX_INIT, //!< Приемник находится в исходном состоянии
  STATE_RX_IDLE, //!< Приемник находится в состоянии ожидания
  STATE_RX_RCV,  //!< Приём пакета
  STATE_RX_ERROR //!< Ошибка приёма
} eMbSngRcvState;

typedef enum
{
  STATE_TX_IDLE, //!< Передатчик находится в состоянии ожидания
  STATE_TX_XMIT  //!< Передатчик находится в состоянии передачи
} eMbSngSndState;

/*! \ingroup modbus SNG
 * \brief Инициализация протокола
 * \param ucSlaveAddress
 * \param ucPort
 * \param ulBaudRate
 * \param eParity
 * \param stopBits
 *
 * \return Ошибка типа eMBErrorCode
 */
eMBErrorCode eMbSngInit(UCHAR ucSlaveAddress, UCHAR ucPort,
                        ULONG ulBaudRate, eMBParity eParity, UCHAR stopBits);

/*! \ingroup modbus SNG
 * \brief Release resources used by the protocol stack.
 *
 * This function disables the Modbus protocol stack and release all
 * hardware resources. It must only be called when the protocol stack 
 * is disabled. 
 *
 * \note Note all ports implement this function. A port which wants to 
 *   get an callback must define the macro MB_PORT_HAS_CLOSE to 1.
 *
 * \return Ошибка типа eMBErrorCode
 */
eMBErrorCode eMbSngClose();

/*! \ingroup modbus SNG
 * \brief Enable the Modbus protocol stack.
 *
 * This function enables processing of Modbus frames. Enabling the protocol
 * stack is only possible if it is in the disabled state.
 *
 * \return Ошибка типа eMBErrorCode
 */
eMBErrorCode eMbSngEnable();

/*! \ingroup modbus SNG
 * \brief Disable the Modbus protocol stack.
 *
 * This function disables processing of Modbus frames.
 *
 * \return Ошибка типа eMBErrorCode
 */
eMBErrorCode eMbSngDisable();

/*! \ingroup modbus SNG
 * \brief The main pooling loop of the Modbus protocol stack.
 *
 * \return Ошибка типа eMBErrorCode
 */
eMBErrorCode eMbSngPoll();

eMbSngException eMbSngFuncReadCurrentState(uint8_t * pucFrame, uint16_t * usLen);
eMbSngException eMbSngFuncRandomSample(uint8_t * pucFrame, uint16_t * usLen);
eMbSngException eMbSngFuncSampleArchive(uint8_t * pucFrame, uint16_t * usLen);
eMbSngException eMbSngFuncWriteRegister(uint8_t * pucFrame, uint16_t * usLen);
eMbSngException eMbSngFuncRestartInterfaceUnit(uint8_t * pucFrame, uint16_t * usLen);

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
