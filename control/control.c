#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <wiringx.h>
#include <ctype.h>
#define PIN_MOTOR_F1 2
#define PIN_MOTOR_F2 4
#define PIN_MOTOR_B1 6
#define PIN_MOTOR_B2 8
#define PIN_SPEED_1 9
#define PIN_SPEED_2 7

// ———— 运动控制函数 ————
void MoveSet(int p1,int p2,int p3,int p4){
    digitalWrite(p1, HIGH);
    digitalWrite(p2, LOW);
    digitalWrite(p3, HIGH);
    digitalWrite(p4, LOW);
}

void BackSet(int p1,int p2,int p3,int p4){
    digitalWrite(p1, LOW);
    digitalWrite(p2, HIGH);
    digitalWrite(p3, LOW);
    digitalWrite(p4, HIGH);
}

void RightTurn(int p1,int p2,int p3,int p4){
    digitalWrite(p1, HIGH);
    digitalWrite(p2, LOW);
    digitalWrite(p3, LOW);
    digitalWrite(p4, HIGH);
}

void LeftTurn(int p1,int p2,int p3,int p4){
    digitalWrite(p1, LOW);
    digitalWrite(p2, HIGH);
    digitalWrite(p3, HIGH);
    digitalWrite(p4, LOW);
}

void StopAll(int p1,int p2,int p3,int p4){
    digitalWrite(p1, LOW);
    digitalWrite(p2, LOW);
    digitalWrite(p3, LOW);
    digitalWrite(p4, LOW);
}



int main(){
    int motors[4] = { PIN_MOTOR_B1, PIN_MOTOR_B2, PIN_MOTOR_F1, PIN_MOTOR_F2 };
    int speeds[2] = { PIN_SPEED_1, PIN_SPEED_2 };

    if (wiringXSetup("milkv_duo", NULL) == -1) {
        fprintf(stderr, "wiringX setup failed\n");
        return -1;
    }

    // —— 引脚初始化 ——
    for(int i=0; i<4; i++)
        pinMode(motors[i], PINMODE_OUTPUT);
    for(int i=0; i<2; i++){
        //pinMode(speeds[i], PINMODE_OUTPUT);
        wiringXPWMSetPeriod(speeds[i], 50000L);
        wiringXPWMSetDuty(speeds[i], 35000L);
        wiringXPWMEnable(speeds[i], 1);
    }

    int ch;

    // 初始控制模式说明
    printf(
      "控制模式启动：\n"
      "  W: 前进  S: 后退  A: 左转  D: 右转\n"
      "  SPACE: 停止  Q: 退出\n"
    );

    do {
        printf("请输入指令（Q 退出）：");
        fflush(stdout);  // 确保提示立即输出

        // 读取第一个非换行字符
        ch = getchar();
        while (ch == '\n') {
            ch = getchar();  // 丢弃所有残留的换行符
        }

        // 将小写转为大写，方便统一处理
        ch = toupper(ch);

        // 根据指令执行相应动作
        switch (ch) {
            case 'W': MoveSet(motors[0], motors[1], motors[2], motors[3]); break;
            case 'S': BackSet(motors[0], motors[1], motors[2], motors[3]); break;
            case 'A': LeftTurn(motors[0], motors[1], motors[2], motors[3]); break;
            case 'D': RightTurn(motors[0], motors[1], motors[2], motors[3]); break;
            case ' ': StopAll(motors[0], motors[1], motors[2], motors[3]); break;
            case 'Q': break;  // 准备退出
            default:
                printf("无效指令：%c\n", ch);
        }

        usleep(100000);  // 小延时，防止 CPU 占用过高
    } while (ch != 'Q');

    StopAll(motors[0], motors[1], motors[2], motors[3]);
    printf("已退出控制，电机已停止。\n");
    return 0;
}