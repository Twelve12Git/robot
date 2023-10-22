#include <stdio.h>
#include "soc/soc.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "encoder.h"
#include "motor_driver.h"

// пины энкодэров !все сломается при включении bluetooth в спящем режиме!
#define ENC_A1 36 
#define ENC_A2 39
#define ENC_B1 35
#define ENC_B2 34 

// пины мотров любые GPIO поддерживающие ШИМ
#define MOTOR_A1 26 
#define MOTOR_A2 25 
#define MOTOR_B1 32
#define MOTOR_B2 33

// пины кнопок любые GPIO(предполагаю хардварный стягивающий резистор, иначе менять pinMode)
#define BUTTON1 22 
#define BUTTON2 23

driver_t drv = {
    .a_1=MOTOR_A1,
    .a_2=MOTOR_A2,
    .b_1=MOTOR_B1,
    .b_2=MOTOR_B2,

    .a_1_pwm=LEDC_CHANNEL_0,
    .a_2_pwm=LEDC_CHANNEL_1,
    .b_1_pwm=LEDC_CHANNEL_2,
    .b_2_pwm=LEDC_CHANNEL_3,

    .pwm_timer_config = {
        .speed_mode=LEDC_LOW_SPEED_MODE,
        .duty_resolution=LEDC_TIMER_8_BIT,
        .timer_num=LEDC_TIMER_0,
        .freq_hz=1<<10
    }
};

encoder_t enc_right = {.in1=ENC_A1, .in2=ENC_A2};
encoder_t enc_left = {.in1=ENC_B1, .in2=ENC_B2};

void setup() {
    init_encoder(&enc_right);
    init_encoder(&enc_left);
    init_driver(&drv);

    set_speed(&drv, 100, -100);
}

void loop() {
    // printf("%d\t\t%d\n", (int)enc_left.counter, (int)enc_right.counter);
}


void app_main(void)
{
    setup();
    while(1) loop();
}
