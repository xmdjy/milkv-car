#include "dcm.h"
float left_speed,right_speed;
int turn_ass = 60;
int main(){
    init();
    time_t start = time(NULL);               // 记录起始时间
    time_t now;
    int fd = wiringXI2CSetup(I2C_DEV,I2C_IP);
    do{
        int lo = wiringXI2CReadReg8(fd,1);
        s1=(lo>>0)&1,s2=(lo>>1)&1,s3=(lo>>2)&1,s4=(lo>>3)&1;
        update();
        if(speed_del()==0){
            left_speed = baseSpeed;
            right_speed = baseSpeed;
        }if(speed_del()!=0){
            left_speed = (baseSpeed+speed_del()+turn_ass);
            right_speed = (baseSpeed-speed_del()-turn_ass);
        }
        move(left_speed,right_speed);
        printf("当前的速度为:left:%f,right:%f\n",left_speed,right_speed);
        usleep(10000);
        now = time(NULL);
    }while(difftime(now, start) < 180.0);
    Stop(PIN_MOTOR_B1,PIN_MOTOR_B2,PIN_MOTOR_F1,PIN_MOTOR_F2);
} 