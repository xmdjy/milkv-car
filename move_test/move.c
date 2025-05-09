#include <stdio.h>
#include <unistd.h>

#include <wiringx.h>
#define PIN_MOTOR_F1 2
#define PIN_MOTOR_F2 4
#define PIN_MOTOR_B1 6
#define PIN_MOTOR_B2 8
//上面的四个控制方向，下面的用来控制转速
#define PIN_SPEED_1 9
#define PIN_SPEED_2 7
//开输出模式
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

//主程序调试
int main(){
    int PIN_LIST[6] = {PIN_MOTOR_F1,PIN_MOTOR_F2,PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_SPEED_1,PIN_SPEED_2};
    if (wiringXSetup("milkv_duo", NULL) == -1) {
        fprintf(stderr, "wiringX setup failed\n");
        wiringXGC();
        return -1;
    }
    printf("Platform: %s\n", wiringXPlatform());

    pinMode(PIN_MOTOR_B1,PINMODE_OUTPUT);
    pinMode(PIN_MOTOR_B2,PINMODE_OUTPUT);
    pinMode(PIN_MOTOR_F1,PINMODE_OUTPUT);
    pinMode(PIN_MOTOR_F2,PINMODE_OUTPUT);
    //pinMode(PIN_SPEED_1, PINMODE_OUTPUT);
    //pinMode(PIN_SPEED_2, PINMODE_OUTPUT);

    wiringXPWMSetPeriod(PIN_SPEED_1,50000L);
    wiringXPWMSetPeriod(PIN_SPEED_2,50000L);

    //wiringXPWMSetDuty(PIN_SPEED_1, 30000L);
    wiringXPWMEnable(PIN_SPEED_1, 1);
    //wiringXPWMSetDuty(PIN_SPEED_2, 30000L);
    wiringXPWMEnable(PIN_SPEED_2, 1);

    //MoveSet(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2,2);
    //StopSet(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2,3);
    
    wiringXPWMSetDuty(PIN_SPEED_1, 35000L);
    wiringXPWMSetDuty(PIN_SPEED_2, 35000L);
    /*经过调试发现75%的高电平占比是最适合转弯的*/
   
    //MoveSet(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2,2);
    //StopSet(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2,3);

    turn(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2,1);
    StopSet(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2,3);
    
}