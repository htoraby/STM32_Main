#ifndef USER_DEBUG_H
#define USER_DEBUG_H

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

 /*!
 * \brief getRegistersFromStack
 * \param pulFaultStackAddress
 */
void getRegistersFromStack(uint32_t *pulFaultStackAddress);

/*!
 * \brief checkRegistersFromStack
 */
void checkRegistersFromStack();

#ifdef __cplusplus
}
#endif

#endif // USER_DEBUG_H
