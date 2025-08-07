#include <until.twi.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum{
    FAILURE,
    SUCCESS,
    NORMINAL_OPERATION,
}twi_status_t;

enum{
    WRITE,
    READ,
};

typedef struct twi_message {
    uint8_t  address;
    uint8_t *buffer;
    size_t   size;
} twi_message_t;

typedef struct twi_isr {
    bool           idle;
    twi_status_t   status;
    twi_message_t *messages;
    size_t         message_count;
} twi_isr_t;

twi_status_t twi_init(const uint32_t scl_frequency);
twi_status_t twi_enqueue(twi_message_t * const messages, size_t message_count);
