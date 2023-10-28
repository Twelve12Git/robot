#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

typedef struct robot_odometry{
    double x, y; // координаты в мм
    double angle; // угол относительно стартовой точки в радианах
    const double weel_d; // диаметр колеса в мм
    const double wheelbase_width; // ширина колесной базы в мм
    const uint16_t encoder_tick_per_rot; // кол-во тиков энкодера за оборот колеса
} robot_odometry_t;


void update_pos(robot_odometry_t* odometry, int16_t delta_left, int16_t delta_right){
    double distance_left  = ((double)delta_left  / (double)odometry->encoder_tick_per_rot) * PI * odometry->weel_d; // в миллиметрах 
    double distance_right = ((double)delta_right /(double) odometry->encoder_tick_per_rot) * PI * odometry->weel_d;

    if(distance_left == distance_right){ // рассматриваем 2 варианта: если робот едет прямо или по дуге
        // робот едет прямо
        odometry->x += distance_left * cos(odometry->angle);
        odometry->y += distance_left * sin(odometry->angle);
    }
    else{
        // робот едет по дуге
        double distance = (distance_left + distance_right)/2; // длина дуги
        double teta = (double) (distance_left-distance_right)/odometry->wheelbase_width; // центральный угол дуги
        double l = (distance/teta)*sin(teta); // длина хорды = D * sin(a), D = L/a
        
        odometry->angle += teta;
        odometry->x +=  l * cos(odometry->angle);
        odometry->y +=  l * sin(odometry->angle);

            
    }
}