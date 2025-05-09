#include "dcm.h"
int fd,lo,s1,s2,s3,s4;
int speed;
long duty;
float err,err_las,err_sum,err_diff;
void Move(int pin1,int pin2,int pin3,int pin4){
    digitalWrite(pin1,HIGH);
    digitalWrite(pin2,LOW);
    digitalWrite(pin3,HIGH);
    digitalWrite(pin4,LOW);
}
void Back(int pin1,int pin2,int pin3,int pin4){
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,HIGH);
    digitalWrite(pin3,LOW);
    digitalWrite(pin4,HIGH);
}
void Stop(int pin1,int pin2,int pin3,int pin4){
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,LOW);
    digitalWrite(pin3,LOW);
    digitalWrite(pin4,LOW);
}
void Left(int pin1,int pin2,int pin3,int pin4){
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,HIGH);
    digitalWrite(pin3,HIGH);
    digitalWrite(pin4,LOW);
}
void Right(int pin1,int pin2,int pin3,int pin4){
    digitalWrite(pin1,HIGH);
    digitalWrite(pin2,LOW);
    digitalWrite(pin3,LOW);
    digitalWrite(pin4,HIGH);
}
void init(){
    wiringXSetup("milkv_duo", NULL);
    //fd = wiringXI2CSetup(I2C_DEV,I2C_IP);
    //lo = wiringXI2CReadReg8(fd,1);
    pinMode(PIN_MOTOR_F1,PINMODE_OUTPUT);
    pinMode(PIN_MOTOR_F2,PINMODE_OUTPUT);
    pinMode(PIN_MOTOR_B1,PINMODE_OUTPUT);
    pinMode(PIN_MOTOR_B2,PINMODE_OUTPUT);
    //pinMode(PIN_SPEED_1,PINMODE_OUTPUT);
    //pinMode(PIN_SPEED_2,PINMODE_OUTPUT);
    wiringXPWMSetPeriod(PIN_SPEED_1,PERIOD);
    wiringXPWMSetDuty(PIN_SPEED_1,DUTY);
    wiringXPWMSetPeriod(PIN_SPEED_2,PERIOD);
    wiringXPWMSetDuty(PIN_SPEED_2,DUTY);
    wiringXPWMEnable(PIN_SPEED_1, 1);
    wiringXPWMEnable(PIN_SPEED_2, 1);
}
int speed_cal(long duty){
    int rec_speed = (int)((MAX_SPEED/PERIOD)*duty);
    return rec_speed;
}
long duty_cal(int speed){
    return (speed*400);
}
void move(float speed1,float speed2){
    speed1 = fmax(fmin(speed1, MAX_SPEED), -MAX_SPEED);
    speed2 = fmax(fmin(speed2, MAX_SPEED), -MAX_SPEED);
    if(speed1>0&&speed2>0){
        Move(PIN_MOTOR_F1,PIN_MOTOR_F2,PIN_MOTOR_B1,PIN_MOTOR_B2);
        wiringXPWMSetDuty(PIN_SPEED_1,duty_cal(speed1));
        wiringXPWMSetDuty(PIN_SPEED_2,duty_cal(speed2));
    }
    else if(speed1<0&&speed2<0){
        Back(PIN_MOTOR_F1,PIN_MOTOR_F2,PIN_MOTOR_B1,PIN_MOTOR_B2);
        wiringXPWMSetDuty(PIN_SPEED_1,duty_cal(-speed1));
        wiringXPWMSetDuty(PIN_SPEED_2,duty_cal(-speed2));
    }
    else if(speed1<0&&speed2>0){
        Right(PIN_MOTOR_F1,PIN_MOTOR_F2,PIN_MOTOR_B1,PIN_MOTOR_B2);
        wiringXPWMSetDuty(PIN_SPEED_1,duty_cal(-speed1));
        wiringXPWMSetDuty(PIN_SPEED_2,duty_cal(speed2));
    }
    else if(speed1>0&&speed2<0){
        Left(PIN_MOTOR_F1,PIN_MOTOR_F2,PIN_MOTOR_B1,PIN_MOTOR_B2);
        wiringXPWMSetDuty(PIN_SPEED_1,duty_cal(speed1));
        wiringXPWMSetDuty(PIN_SPEED_2,duty_cal(-speed2));
    }
}
float err_cal(int s1,int s2,int s3,int s4,float K){
    float t,output;
    if(s1==0&&s2==1&&s3==1&&s4==0){//0110
        t = 0;
    }
    if(s1==0&&s2==1&&s3==0&&s4==0){//0100
        t = -0.85;
    }
    if(s1==0&&s2==0&&s3==1&&s4==0){//0010
        t = 0.85;
    }
    /*if(s1==1&&s2==0&&s3==1&&s4==1){//1011
        t = 0.3;
    }
    if(s1==1&&s2==1&&s3==0&&s4==1){//1101
        t = -0.3;
    }*/
    if(s1==1&&s2==0&&s3==0&&s4==0){
        t = -2.85;
    }
    if(s1==0&&s2==0&&s3==0&&s4==1){
        t = 2.85;
    }
    if(s1==1&&s2==1&&s3==0&&s4==0){
        t = -3.35;
    }
    if(s1==0&&s2==0&&s3==1&&s4==1){
        t = 3.35;
    }
    if(s1==1&&s2==1&&s3==1&&s4==0){
        t = -3.65;
    }
    if(s1==0&&s2==1&&s3==1&&s4==1){
        t = 3.65;
    }
    if(s1==1&&s2==1&&s3==1&&s4==1){
        t = 0;
    }
    output = t * K;
    return output;
}
float pid_cal(float err,float err_sum,float err_diff){
    float kp=55.25,ki=0,kd=0;//kp42是一个还不错的参数
    float output;
    output = kp*err + ki*err_sum + kd*err_diff;
    return output;
}
void update(){
    err = err_cal(s1,s2,s3,s4,1);
    err_sum += err;
    err_diff = err - err_las;
    err_las = err;
}
float speed_del(){
    float output;
    float kw = 1;
    output = pid_cal(err,err_sum,err_diff)*kw;
    return output;
}
