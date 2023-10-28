#include "include/encoder.h"

void enc_interrupt_handler(encoder_t* enc){
    enc->state = (enc->state << 2) | (gpio_get_level(enc->in1)<<1 | gpio_get_level(enc->in2));
    *(enc->counter) += (((enc->state&0x8) >> 3) != ((enc->state&0x2) >> 1)) ? ((((enc->state&0x2)>>1) != (enc->state&0x1))?1:-1):0;
}

esp_err_t init_encoder(encoder_t* enc){
    // esp_err_t err;
    // if(err != ESP_OK) return err; // TODO add errors handling
    gpio_install_isr_service(ESP_INTR_FLAG_EDGE);

    gpio_reset_pin(enc->in1);
    gpio_set_direction(enc->in1, GPIO_MODE_INPUT);
    gpio_set_intr_type(enc->in1, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(enc->in1, (gpio_isr_t)enc_interrupt_handler, (void*) enc);

    gpio_reset_pin(enc->in2);
    gpio_set_direction(enc->in2, GPIO_MODE_INPUT);
    gpio_set_intr_type(enc->in2, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(enc->in2, (gpio_isr_t)enc_interrupt_handler, (void*)enc);
   
    return ESP_OK;
}