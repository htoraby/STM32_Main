#ifndef USER_DEBUG_H
#define USER_DEBUG_H

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

void getRegistersFromStack(uint32_t *pulFaultStackAddress);

#ifdef __cplusplus
}
#endif

#endif // USER_DEBUG_H
