#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringx.h>

#define BZR_PIN    13     // 物理排针 6 对应 GP4/PWM_5
#define BASE_MS    250   // 四分音符基础时长

int notes[56] = {
    131,131,165,196,196,165,196,131,
    131,165,196,196,165,131,131,165,
    165,196,165,196,131,165,165,196,
    165,131,131,165,196,196,165,196,
    131,131,165,196,196,165,131,131,
    165,165,196,165,196,131,165,165,
    196,165,131
  };  

int beats[56] = {
    4,4,4,4,4,4,4,4,
    4,4,4,4,4,2,4,4,
    4,4,4,4,4,4,4,4,
    4,4,4,4,4,4,4,4,
    4,4,4,4,4,4,2,4,
    4,4,4,4,4,4,4,4,
    4,4,4,4,4,2
  }; 

int main(void) {
    // 1. 平台初始化
    if (wiringXSetup("milkv_duo", NULL) == -1) {
        fprintf(stderr, "wiringXSetup failed\n");
        return -1;
    }

    // 2. 引脚模式设为 PWM 输出
    pinMode(BZR_PIN, PINMODE_OUTPUT);          // PWM 模式 :contentReference[oaicite:7]{index=7}

    // 3. 遍历音符播放
    for (int i = 0; i < 56; i++) {
        // 计算周期和占空比，单位：纳秒
        uint64_t period_ns = 1000000000ULL / notes[i];
        uint64_t duty_ns   = period_ns / 2;

        // 配置并启动 PWM
        wiringXPWMSetPeriod(BZR_PIN, period_ns); // 设置周期 :contentReference[oaicite:8]{index=8}
        wiringXPWMSetDuty(BZR_PIN, duty_ns);     // 设置占空比 :contentReference[oaicite:9]{index=9}
        wiringXPWMEnable(BZR_PIN, 1);            // 使能 PWM 输出 :contentReference[oaicite:10]{index=10}

        // 延时播放：时长 = BASE_MS * (4.0 / beats[i])
        int duration_ms = (int)(BASE_MS * (4.0f / beats[i]));
        usleep(duration_ms * 1000);

        // 关闭 PWM
        wiringXPWMEnable(BZR_PIN, 0);

        // 音符间短暂间隔
        usleep(5000); // 5 ms 间隔
    }

    return 0;
}
