
typedef enum{
    FAILURE,
    SUCCESS,
    NORMINAL_OPERATION,
}twi_status_t;

enum{
    WRITE,
    READ,
};

void twi_init();
void twi_write()
