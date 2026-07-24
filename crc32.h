/*
 * Generic crc32 helper function
 */

#pragma once

#include <stdint.h>
#include <stdio.h>

extern uint32_t
crc32(const uint8_t* s, size_t len);
