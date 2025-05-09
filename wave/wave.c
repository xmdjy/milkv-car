#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <termios.h>
#include <wiringx.h>
#include <ctype.h>
#define Trig 14
#define Echo 15

static long getMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000L + tv.tv_usec;
}
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
    float dis;
    if (wiringXSetup("milkv_duo", NULL) == -1) {
        fprintf(stderr, "wiringX setup failed\n");
        wiringXGC();
        return -1;
    }
    printf("Platform: %s\n", wiringXPlatform());

    pinMode(Trig, PINMODE_OUTPUT);
    pinMode(Echo, PINMODE_INPUT);

    printf(
        "控制模式：\n"
        "  W: 开始测距并显示距离\n"
        "  Q: 退出程序\n"
    );
    int ch;
    do{
        printf("请输入指令（Q 退出）：");
        fflush(stdout);  // 确保提示立即输出

        // 读取第一个非换行字符
        ch = getchar();
        while (ch == '\n') {
            ch = getchar();  // 丢弃所有残留的换行符
        }
        // 将小写转为大写，方便统一处理
        ch = toupper(ch);
        if(ch == 'W'){
            dis = measureDistance();
            printf("距离: %.2f cm\n", dis);
        }
    }while(ch != 'Q');
    usleep(100000);
    return 0;
}
/*do {
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
*/