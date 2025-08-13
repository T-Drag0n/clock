#include <stdint.h>
#include <avr/io.h>
#include <stdbool.h>

extern const uint32_t SCL_FREQUENCY;
extern volatile bool alarm_triggered;

#define DS3231_ADDRESS 0x68
#define CLOCK_ADDRESS 0x00
#define ALARM_ADDRESS 0x0B
#define STATUS_ADDRESS 0x0F
#define CONTROL_ADDRESS 0x0E

typedef enum{
    CLOCK,
    SET_TIME,
    SET_ALARM,
    ALARM_ON,
}mode_t;

typedef struct time_value{
    uint8_t minute;
    uint8_t hour;
}time_value_t;

uint8_t bcd_to_binary(uint8_t bcd);
uint8_t binary_to_bcd(uint8_t value);
void read_current_time(time_value_t *time);
void read_alarm_time(time_value_t *time);
void write_current_time(time_value_t *time);
void write_alarm_time(time_value_t *time);
void clear_alarm_flag(void);
void rtc_init(void);