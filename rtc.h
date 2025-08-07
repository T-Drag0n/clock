#include <stdint.h>

#define DS3231_ADDRESS 0x68

uint8_t bcd_to_binary(uint8_t bcd);
uint8_t binary_to_bcd(uint8_t value);
