#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <wiringx.h>       
#define I2C_DEV    "/dev/i2c-0"
#define SLAVE_ADDR 0x78    

int main() {
    wiringXSetup("milkv_duo", NULL);  
    int fd = wiringXI2CSetup(I2C_DEV, SLAVE_ADDR);
    if (fd < 0) {
        fprintf(stderr, "Failed to setup I2C on %s at 0x%02X\n", I2C_DEV, SLAVE_ADDR);
        return 1;
    }

    while (1) {
        int lo = wiringXI2CReadReg8(fd,1);
        // 4. 解析低四位为四路寻迹头状态
        printf("Sensors: [%d %d %d %d]\n",
               (lo >> 0) & 1,
               (lo >> 1) & 1,
               (lo >> 2) & 1,
               (lo >> 3) & 1);

        usleep(200000);
    }

    close(fd);
    return 0;
}
