/*
* Generic crc32 helper function
*/

#ifndef __CRC32_H__
#define __CRC32_H__

#include <stdint.h>
#include <stdio.h>

extern uint32_t crc32(const uint8_t *s, size_t len);

#endif /* __CRC32_H__ */
