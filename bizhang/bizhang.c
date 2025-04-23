#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <termios.h>
#include <wiringx.h>
#include <ctype.h>
#include <time.h>
#define Trig 10
#define Echo 11
#define PIN_MOTOR_F1 2
#define PIN_MOTOR_F2 4
#define PIN_MOTOR_B1 6
#define PIN_MOTOR_B2 8
//上面的四个控制方向，下面的用来控制转速
#define PIN_SPEED_1 9
#define PIN_SPEED_2 7
#define dis 10

void ModeSet (int *PIN_LIST){
    for(int i = 0;i < 6;i++){
        pinMode(PIN_LIST[i],PINMODE_OUTPUT);
    }
}
//设置电机的周期
void PeriodSet(int *PIN_LIST,int period){
    for(int i = 0;i < 6;i++){
        wiringXPWMSetPeriod(PIN_LIST[i],period);
    }
}
//前进
void MoveSet(int pin1,int pin2,int pin3,int pin4,int time){
    digitalWrite(pin1,HIGH);
    digitalWrite(pin2,LOW);
    digitalWrite(pin3,HIGH);
    digitalWrite(pin4,LOW);
    sleep(time);
}
//后退
void BackSet(int pin1,int pin2,int pin3,int pin4,int time){
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,HIGH);
    digitalWrite(pin3,LOW);
    digitalWrite(pin4,HIGH);
    sleep(time);
}
//停止
void StopSet(int pin1,int pin2,int pin3,int pin4,int time){
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,LOW);
    digitalWrite(pin3,LOW);
    digitalWrite(pin4,LOW);
    sleep(time);
}
//右转
void turn(int pin1,int pin2,int pin3,int pin4,int time){
    digitalWrite(pin1,HIGH);
    digitalWrite(pin2,LOW);
    digitalWrite(pin3,LOW);
    digitalWrite(pin4,HIGH);
    sleep(time);
}

static long getMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000L + tv.tv_usec;
}
// 测距
float measureDistance(){
    digitalWrite(Trig, LOW);  usleep(2);
    digitalWrite(Trig, HIGH); usleep(10);
    digitalWrite(Trig, LOW);

    while (digitalRead(Echo) == LOW);
    long start = getMicroseconds();
    while (digitalRead(Echo) == HIGH);
    long end   = getMicroseconds();
    return (end - start) / 58.0f;
}

int main() {
    if(wiringXSetup("milkv_duo", NULL) == -1) {
        wiringXGC();
        return 1;
    }
    int PIN_LIST[6] = {PIN_MOTOR_F1,PIN_MOTOR_F2,PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_SPEED_1,PIN_SPEED_2};
    pinMode(PIN_MOTOR_B1,PINMODE_OUTPUT);
    pinMode(PIN_MOTOR_B2,PINMODE_OUTPUT);
    pinMode(PIN_MOTOR_F1,PINMODE_OUTPUT);
    pinMode(PIN_MOTOR_F2,PINMODE_OUTPUT);
    pinMode(Trig, PINMODE_OUTPUT);
    pinMode(Echo, PINMODE_INPUT);
    //pinMode(PIN_SPEED_1, PINMODE_OUTPUT);
    //pinMode(PIN_SPEED_2, PINMODE_OUTPUT);
    wiringXPWMSetPeriod(PIN_SPEED_1,50000L);
    wiringXPWMSetPeriod(PIN_SPEED_2,50000L);
    wiringXPWMSetDuty(PIN_SPEED_1, 35000L);
    wiringXPWMEnable(PIN_SPEED_1,1);
    wiringXPWMSetDuty(PIN_SPEED_2, 35000L);
    wiringXPWMEnable(PIN_SPEED_2, 1);
    time_t start = time(NULL);               // 记录起始时间
    time_t now;
    do {
        float dist = measureDistance();
        printf("距离: %.2f cm\r", dist);
        fflush(stdout);
            if(dist < dis){
                BackSet(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2,1);
                turn(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2,1);
                StopSet(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2,1);
            }
        usleep(100000);
        now = time(NULL);                    // 获取当前时间
    } while (difftime(now, start) < 60.0);
    StopSet(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2,1);
    return 0;
}
/*time_t start = time(NULL);               // 记录起始时间
    time_t now;
    do {
        float dist = measureDistance();
        printf("距离: %.2f cm\r", dist);
        fflush(stdout);
            if(dist < dis){
                BackSet(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2,1);
                turn(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2,1);
                StopSet(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2,1);
            }
        usleep(100000);
        now = time(NULL);                    // 获取当前时间
    } while (difftime(now, start) < 60.0);   // 差值小于 60 秒时继续循环*/