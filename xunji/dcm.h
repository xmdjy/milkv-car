#ifndef MOTOR
#define MOTOR
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <wiringx.h>
#include <ctype.h>
#include <stdint.h>
#include<math.h>
#include <time.h>
#define LEFT 0
#define RIGHT 1
#define PIN_MOTOR_F1 2
#define PIN_MOTOR_F2 4
#define PIN_MOTOR_B1 6
#define PIN_MOTOR_B2 8
#define PIN_SPEED_1 9
#define PIN_SPEED_2 7
#define PERIOD 50000
#define DUTY 35000
#define MAX_SPEED 125  //满速为125cm/s
extern int speed;
extern long duty;
void Move(int pin1,int pin2,int pin3,int pin4);
void Back(int pin1,int pin2,int pin3,int pin4);
void Left(int pin1,int pin2,int pin3,int pin4);
void Right(int pin1,int pin2,int pin3,int pin4);
void Stop(int pin1,int pin2,int pin3,int pin4);
//void move(float speed1,float speed2);
void init(void);
int speed_cal(long duty);
long duty_cal(int speed);
#endif

#ifndef TRACK
#define TRACK
#define I2C_DEV    "/dev/i2c-0"
#define I2C_IP 0x78
#define Wi1 1
#define Wi2 3.5
//Wi定义了内外侧传感器的权值，用于计算误差
extern int fd;
extern int lo;
extern int s1,s2,s3,s4;
#endif

#ifndef PID
#define PID
#define baseSpeed 78
extern float kp,ki,kd;
extern float err,err_las,err_sum,err_diff;
extern float pid;
float err_cal(int s1,int s2,int s3,int s4,float K);
float pid_cal(float err,float err_sum,float err_diff);
void update();
float speed_del();
void setWheel(int wheelId, float speed);
void move(float speed1,float speed2);
#endif