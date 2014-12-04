/*===================================================================
 *
 *   Функции для тестирования памяти, шины данных и шины адреса
 *
 *===================================================================
 * Дата последней модификации: 19.11.2014
 *===================================================================*/

/*
 * Базируется на исходных текстах, опубликованных статье:
 *  Barr, Michael. "Software-Based Memory Testing."  
 *  Online at http://www.netrino.com/Articles/MemoryTesting/paper.html, 
 *  February 1997
 */
#include <stdint.h>
#include "test_sram.h"

#pragma GCC optimize ("O0")
/********************************************************************
 *
 * FUNCTION:    memTestDataBus()
 *
 * DESCRIPTION: Test the data bus wiring in a memory region by
 *              performing a walking 1's test at a fixed address
 *              within that region. The address (and hence the
 *              region) is selected by the caller.
 *
 * NOTES:       This implementation of the test assumes a data bus
 *              width of 8 bits (unsigned char).
 *
 * RETURNS:     0 if the test succeeds. 
 *
 *              A non-zero result is the first pattern that failed.
 ********************************************************************/
uint8_t memTestDataBus( volatile uint8_t *address )
{
    uint8_t pattern;

    /*
     * Perform a walking 1's test at the given address.
     */
    for( pattern = 0x01; pattern != 0x00; pattern <<= 1 )
    {
        /*
         * Write the test pattern.
         */
        *address = pattern;

        /*
         * Read it back (immediately is okay for this test).
         */
        if ( *address != pattern )
        {
            return( pattern );
        }
    }

    return( 0 );

}   /* memTestDataBus() */

#pragma GCC optimize ("O0")
/*********************************************************************
 *
 * FUNCTION:    memTestAddressBus()
 *
 * DESCRIPTION: Test the address bus wiring in a memory region by
 *              performing a walking 1's test on the relevant bits
 *              of the address and checking for aliasing. This test
 *              will find single-bit address failures such as stuck
 *              -high, stuck-low, and shorted pins. The base address
 *              and size of the region are selected by the caller.
 *
 * NOTES:       This implementation of the test assumes a data bus
 *              width of 8 bits (unsigned char). 
 *
 * RETURNS:     0 if the test succeeds. 
 *
 *              A non-zero result is the first address at which an
 *              aliasing problem was uncovered. By examining the
 *              contents of memory, it may be possible to gather
 *              additional information about the problem.
 ********************************************************************/
uint8_t *memTestAddressBus( uint8_t *baseAddress, uint32_t nBytes )
{
    uint16_t pattern = 0xAA;
    uint16_t antipattern = 0x55;
    uint32_t bitMask = nBytes - 1;
    uint32_t offset;
    uint32_t testOffset;

    /*
     * Write the default pattern at each of the power-of-two offsets.
     */
    for ( offset = 0x0001; ( offset & bitMask ) != 0; offset <<= 1 )
    {
      baseAddress[offset] = pattern;
    }

    /* 
     * Check for address bits stuck high.
     */

    *baseAddress = antipattern;

    for ( offset = 0x0001; ( offset & bitMask ) != 0; offset <<= 1 )
    {
      if ( baseAddress[offset] != pattern )
       {
        return ( &baseAddress[offset] );
       }
    }
    
    /*
     * Check for address bits stuck low or shorted.
     */

    *baseAddress = pattern;    

    for ( testOffset = 0x0001; ( testOffset & bitMask ) != 0; testOffset <<= 1 )
    {
      baseAddress[testOffset] = antipattern;

      for ( offset = 0x0001; ( offset & bitMask ) != 0; offset <<= 1 )
       {
          if ( ( baseAddress[offset] != pattern ) && ( offset != testOffset ) )
           {
              return ( &baseAddress[testOffset] );
           }
       }

      baseAddress[testOffset] = pattern;
    }

    return( 0 );

}   /* memTestAddressBus() */

#pragma GCC optimize ("O0")
/*********************************************************************
 *
 * FUNCTION:    memTestDevice()
 *
 * DESCRIPTION: Test the integrity of a physical memory device by
 *              performing an increment/decrement test over the
 *              entire region. In the process every storage bit 
 *              in the device is tested as a zero and a one. The
 *              base address and the size of the region are
 *              selected by the caller.
 *
 * NOTES:       This implementation of the test assumes a data bus
 *              width of 8 bits (unsigned char). 
 *
 * RETURNS:     0 if the test succeeds. Also, in that case, the
 *              entire memory region will be filled with zeros.
 *
 *              A non-zero result is the first address at which an
 *              incorrect value was read back. By examining the
 *              contents of memory, it may be possible to gather
 *              additional information about the problem.
 ********************************************************************/
uint8_t *memTestDevice( uint8_t *baseAddress, uint32_t nBytes )  
{
    uint8_t pattern;
    uint8_t antipattern;
    uint32_t offset;

    for ( pattern = 1, offset = 0; offset < nBytes; pattern++, offset++ )
    {
      baseAddress[offset] = pattern;
      
    }
    
    for ( pattern = 1, offset = 0; offset < nBytes; pattern++, offset++ )
    {
      if ( baseAddress[offset] != pattern )
       {
         return( &baseAddress[offset] );
       }

      antipattern = ~pattern;
      baseAddress[offset] = antipattern;
    }

    for (pattern = 1, offset = 0; offset < nBytes; pattern++, offset++)
    {
      antipattern = ~pattern;
 
      if ( baseAddress[offset] != antipattern )
       {
        return( &baseAddress[offset] );
       }

      baseAddress[offset] = 0;
    }
    
    return( 0 );

}   /* memTestDevice() */

uint8_t sram_test_step;
uint32_t sram_test_code;

#pragma GCC optimize ("O0")
/************************************************************************
 *
 * FUNCTION:    xmemTestAll()
 *
 * DESCRIPTION: Выполняет тестирование:
 *                1) шины данных;
 *                2) шины адреса;
 *                3) непрерывного участка внешней SRAM;
 *
 * RETURNS:     0 если все тесты прошли успешно
 *
 *              Ненулевое значение сообщает номер неудавшегося теста.
 *              Глобальные переменные модуля содержат:
 *                sram_test_step  - номер непройденного теста;
 *                sram_test_code  - байт данных для memTestDataBus;
 *                                  адрес ячейки ОЗУ для других функций;
 * 
 ***********************************************************************/
uint8_t xmemTestAll( void )
{
 
  /* Тест шины данных */
  sram_test_step = 1;  
  sram_test_code = ( uint16_t ) memTestDataBus( SRAM_EXT_BEGIN );
  if ( sram_test_code ) return( sram_test_step );
  
  /* Тест шины адреса */
  sram_test_step = 2;
  sram_test_code = ( uint32_t ) memTestAddressBus( SRAM_EXT_BEGIN, SRAM_EXT_SIZE );
  if ( sram_test_code ) return( sram_test_step );

  /* Тест непрерывной области внешней SRAM */
  sram_test_step = 3;
  sram_test_code = ( uint32_t ) memTestDevice( SRAM_EXT_BEGIN, SRAM_EXT_SIZE );
  if ( sram_test_code ) return( sram_test_step );

  /*
  sram_test_code = ( uint32_t ) memTestDevice( SRAM_EXT_BEGIN+100000, 10000 );
  if ( sram_test_code ) return( sram_test_step );
  printf( ">> Test_SRAM (size=2)\n" );
  sram_test_code = ( uint32_t ) memTestDevice( SRAM_EXT_BEGIN+524288-1000, 1000 );
  if ( sram_test_code ) return( sram_test_step );
  printf( ">> Test_SRAM (size=3)\n" );*/

  /* Все тесты прошли успешно */
  sram_test_step = 0;
  sram_test_code = 0;

  return( 0 );  
}
