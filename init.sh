cat >> /mnt/system/duo-init.sh << 'EOF'
duo-pinmux -w GP2/PWM_10
duo-pinmux -w GP4/PWM_5
duo-pinmux -w GP6/PWM_9
duo-pinmux -w GP8/PWM_7
duo-pinmux -w GP9/PWM_4
duo-pinmux -w GP7/PWM_8
duo-pinmux -w GP1/IIC0_SDA
duo-pinmux -w GP0/IIC0_SCL
EOF

chmod +x /mnt/system/duo-init.sh
reboot