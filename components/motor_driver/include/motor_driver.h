#include <stdio.h>
#include "driver/gpio.h"
#include "driver/ledc.h"

#ifndef constrain
#define constrain(v, min, max)((v < min) ? min : ((v > max) ? max : v))
#endif 

#ifndef DRIVER_TYPE_DEF
#define DRIVER_TYPE_DEF
typedef struct driver{
  // логика написана под pololu DRV8833
  const gpio_num_t a_1;
  const gpio_num_t a_2;
  const gpio_num_t b_1;
  const gpio_num_t b_2;

  ledc_channel_t a_1_pwm;
  ledc_channel_t a_2_pwm;
  ledc_channel_t b_1_pwm;
  ledc_channel_t b_2_pwm;

  const ledc_timer_config_t pwm_timer_config;

} driver_t;
#endif

void set_speed(const driver_t*, int32_t, int32_t);
esp_err_t init_driver(const driver_t* drv);