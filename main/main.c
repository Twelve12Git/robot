#include <stdio.h>
#include "soc/soc.h"
#include "driver/gpio.h"
#include "encoder.h"

// пины энкодэров (подключать только к GPIO 32-39, на этом завязана двоичная логика)
#define ENC_A1 36 // пины 32-39 только для чтения в регистре GPIO_IN1_REG (0x3FF44040)
#define ENC_A2 39 // все сломается при включении bluetooth в спящем режиме
#define ENC_B1 35
#define ENC_B2 34 

#define MOTOR_B1 26 // пины мотров любые GPIO поддерживающие ШИМ
#define MOTOR_B2 25 
#define MOTOR_A1 33
#define MOTOR_A2 32

#define MOTORS_PWM_FREQ 9000 // настройка ШИМа драйвера моторов
#define MOTORS_PWM_RESOLUTION 8 // 0-255
#define MOTOR_PWM_CHANNEL_A1 0 // таймер под ШИМ
#define MOTOR_PWM_CHANNEL_A2 1 
#define MOTOR_PWM_CHANNEL_B1 2 
#define MOTOR_PWM_CHANNEL_B2 3

#define BUTTON1 22 // пины кнопок любые GPIO(предполагаю хардварный стягивающий резистор, иначе менять pinMode)
#define BUTTON2 23

/*
struct driver{
  // логика написана под pololu DRV8833
  uint8_t a_1;
  uint8_t a_2;
  uint8_t b_1;
  uint8_t b_2;
  void init(){
    ledcSetup(MOTOR_PWM_CHANNEL_A1, MOTORS_PWM_FREQ, MOTORS_PWM_RESOLUTION);
    ledcSetup(MOTOR_PWM_CHANNEL_A2, MOTORS_PWM_FREQ, MOTORS_PWM_RESOLUTION);
    ledcSetup(MOTOR_PWM_CHANNEL_B1, MOTORS_PWM_FREQ, MOTORS_PWM_RESOLUTION);
    ledcSetup(MOTOR_PWM_CHANNEL_B2, MOTORS_PWM_FREQ, MOTORS_PWM_RESOLUTION);
    ledcAttachPin(a_1, MOTOR_PWM_CHANNEL_A1);
    ledcAttachPin(a_2, MOTOR_PWM_CHANNEL_A2);
    ledcAttachPin(b_1, MOTOR_PWM_CHANNEL_B1);
    ledcAttachPin(b_2, MOTOR_PWM_CHANNEL_B2);
  }
  void set_motors_speed(int16_t speed_a, int16_t speed_b){
    speed_a = constrain(speed_a, -255, 255);
    speed_b = constrain(speed_b, -255, 255);
    if(speed_a >= 0){
      ledcWrite(MOTOR_PWM_CHANNEL_A1, 255);
      ledcWrite(MOTOR_PWM_CHANNEL_A2, 255-speed_a);
    }else{
      ledcWrite(MOTOR_PWM_CHANNEL_A1, 255+speed_a);
      ledcWrite(MOTOR_PWM_CHANNEL_A2, 255);
    }
    if(speed_b >= 0){
      ledcWrite(MOTOR_PWM_CHANNEL_B1, 255);
      ledcWrite(MOTOR_PWM_CHANNEL_B2, 255-speed_b);
    }else{
      ledcWrite(MOTOR_PWM_CHANNEL_B1, 255+speed_b);
      ledcWrite(MOTOR_PWM_CHANNEL_B2, 255);
    }
  }
};
*/

encoder_t enc_right = {.in1=ENC_A1, .in2=ENC_A2};
encoder_t enc_left = {.in1=ENC_B1, .in2=ENC_B2};

void setup() {
    init_encoder(&enc_right);
    init_encoder(&enc_left);
}

void loop() {
    printf("%d\t\t%d\n", (int)enc_left.counter, (int)enc_right.counter);
}


void app_main(void)
{
    setup();
    while(1) loop();
}
