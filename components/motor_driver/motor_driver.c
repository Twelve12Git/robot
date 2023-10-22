#include "include/motor_driver.h"

void set_speed(const driver_t* drv, int32_t speed_a, int32_t speed_b){
    speed_a = constrain(speed_a, -(1<<drv->pwm_timer_config.duty_resolution), 1<<drv->pwm_timer_config.duty_resolution);
    speed_b = constrain(speed_b, -(1<<drv->pwm_timer_config.duty_resolution), 1<<drv->pwm_timer_config.duty_resolution);
    const int32_t max_duty = (1<<drv->pwm_timer_config.duty_resolution) - 1;

    if(speed_a >= 0){
        ledc_set_duty(drv->pwm_timer_config.speed_mode, drv->a_1_pwm, max_duty);
        ledc_set_duty(drv->pwm_timer_config.speed_mode, drv->a_2_pwm, max_duty-speed_a);
    }else{
        ledc_set_duty(drv->pwm_timer_config.speed_mode, drv->a_1_pwm, max_duty+speed_a);
        ledc_set_duty(drv->pwm_timer_config.speed_mode, drv->a_2_pwm, max_duty);
    }
    if(speed_b >= 0){
        ledc_set_duty(drv->pwm_timer_config.speed_mode, drv->b_1_pwm, max_duty);
        ledc_set_duty(drv->pwm_timer_config.speed_mode, drv->b_2_pwm, max_duty-speed_b);
    }else{
        ledc_set_duty(drv->pwm_timer_config.speed_mode, drv->b_1_pwm, max_duty+speed_b);
        ledc_set_duty(drv->pwm_timer_config.speed_mode, drv->b_2_pwm, max_duty);
    }
}


esp_err_t init_driver(const driver_t* drv){
    const ledc_channel_config_t channel_config_templtate = {
        .speed_mode=drv->pwm_timer_config.speed_mode,
        .intr_type=LEDC_INTR_DISABLE,
        .timer_sel=drv->pwm_timer_config.timer_num,
        .flags={.output_invert=0}
    };

    ESP_ERROR_CHECK(ledc_timer_config(&drv->pwm_timer_config));

    const gpio_num_t gpios[] = {drv->a_1, drv->a_2, drv->b_1, drv->b_2};
    const ledc_channel_t* channels = {&(drv->a_1_pwm), &(drv->a_2_pwm), &(drv->b_1_pwm), &(drv->b_2_pwm)};

    for(uint8_t i = 0; i < 4; i++){
        ledc_channel_config_t tmp = channel_config_templtate;
        tmp.channel = channels[i];
        tmp.gpio_num = gpios[i];
        ESP_ERROR_CHECK(ledc_channel_config(&tmp));
    }
    return ESP_OK;
}