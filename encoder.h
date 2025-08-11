#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

typedef struct {
    volatile uint8_t *pin_reg;
    uint8_t CLK;
    uint8_t DT;
    uint8_t CLK_last_value;
    int8_t value;
    int8_t min;
    int8_t max;
}encoder_t;

extern encoder_t encoder_mode;
extern encoder_t encoder_mins;
extern encoder_t encoder_hours;
extern encoder_t encoder_confirm;

void encoder(encoder_t *encoder);
