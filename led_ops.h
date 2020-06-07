#ifndef _LED_OPS_H
#define _LED_OPS_H



struct led_operations {
    int (*init_fun)(int which);
    int (*ioctl_fun)(int which, int states);
};

static led_operations *get_led_operation(void);


#endif




