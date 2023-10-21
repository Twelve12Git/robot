#include <stdio.h>
#include "driver/gpio.h"
#include "esp_intr_alloc.h"

#ifndef ENC_TYPE_DEFINE
#define ENC_TYPE_DEFINE
typedef struct encoder{
    gpio_num_t in1;
    gpio_num_t in2;
    uint8_t state;
    int64_t* counter; 
} encoder_t;
#endif



esp_err_t init_encoder(encoder_t*);
void enc_interrupt_handler(encoder_t*);