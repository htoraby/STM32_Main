#ifndef __TEST_SRAM_H__
/*----------------------------------------------------------------------------*/

#define __TEST_SRAM_H__

#ifdef __cplusplus
 extern "C" {
#endif

#define SRAM_EXT_BEGIN ( ( uint8_t * ) 0x60000000 )
#define SRAM_EXT_END   ( ( uint8_t * ) 0x600FFFFF )
#define SRAM_EXT_SIZE  ( ( uint32_t ) SRAM_EXT_END - ( uint32_t ) SRAM_EXT_BEGIN + 1 )


extern uint8_t sram_test_step;
extern uint32_t sram_test_code;

uint8_t memTestDataBus( volatile uint8_t *address );
uint8_t *memTestAddressBus( uint8_t *baseAddress, uint32_t nBytes );
uint8_t *memTestDevice( uint8_t *baseAddress, uint32_t nBytes );
uint8_t xmemTestAll( void );

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------*/
#endif

