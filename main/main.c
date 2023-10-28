#include <stdio.h>
#include "soc/soc.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "encoder.h"
#include "motor_driver.h"
#include "odometry.c"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

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

int64_t enc_left_count, enc_right_count;
encoder_t enc_right = {.in1=ENC_A1, .in2=ENC_A2, .counter=&enc_right_count};
encoder_t enc_left = {.in1=ENC_B1, .in2=ENC_B2, .counter=&enc_left_count};

robot_odometry_t robot = {
    .angle = 0,
    .x = 0,
    .y = 0,
    .encoder_tick_per_rot = 300,
    .weel_d = 39,
    .wheelbase_width = 93
};

void vTask_update_pos(void* params){
    const TickType_t delay = 100 / portTICK_PERIOD_MS;
    while(true){
        static int64_t enc_left_count_prev, enc_right_count_prev;
        update_pos(&robot, enc_left_count-enc_left_count_prev, enc_right_count-enc_right_count_prev);
        enc_left_count_prev = enc_left_count;
        enc_right_count_prev = enc_right_count;
        vTaskDelay(delay);
    }
}

void setup() {
    init_encoder(&enc_right);
    init_encoder(&enc_left);
    init_driver(&drv);
    xTaskCreate(vTask_update_pos, "calc_odometry", 1<<16, NULL, tskIDLE_PRIORITY, NULL);
    set_speed(&drv, 0, 40);
}


void loop() {
    printf("%d\t\t%d\t%f\t%f\t%f\n", (int)enc_left_count, (int)enc_right_count, robot.x, robot.y, robot.angle*57.2957795131);
}


void app_main(void)
{
    setup();
    while(1) loop();
}
