
/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbconfig.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_FUNC_READ_ADDR_OFF           ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_READ_REGCNT_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE               ( 3 )
#define MB_PDU_FUNC_READ_REGCNT_MAX         ( 0x007D )

/* ----------------------- Static functions ---------------------------------*/
eMBException    prveMBError2Exception( eMBErrorCode eErrorCode );

/* ----------------------- Start implementation -----------------------------*/
#if MB_FUNC_READ_ID_INFO_ENABLED > 0

eMBException
eMBFuncReadIdInfo( UCHAR * pucFrame, USHORT * usLen )
{
    USHORT          usRegAddress;
    UCHAR          *pucFrameCur;

    eMBException    eStatus = MB_EX_NONE;
    eMBErrorCode    eRegStatus;

    if( *usLen == ( MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN ) )
    {
        usRegAddress = ( USHORT )( pucFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8 );
        usRegAddress |= ( USHORT )( pucFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1] );
        usRegAddress++;

        /* Set the current PDU data pointer to the beginning. */
        pucFrameCur = &pucFrame[MB_PDU_FUNC_OFF];
        *usLen = MB_PDU_FUNC_OFF;

        /* First byte contains the function code. */
        *pucFrameCur++ = MB_FUNC_READ_ID_INFO;
        *usLen += 1;

        eRegStatus = eMBRegReadInfo( pucFrameCur, usRegAddress, usLen );

        /* If an error occured convert it into a Modbus exception. */
        if( eRegStatus != MB_ENOERR )
        {
          eStatus = prveMBError2Exception( eRegStatus );
        }
    }
    else
    {
        /* Can't be a valid read input register request because the length
         * is incorrect. */
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}

#endif
