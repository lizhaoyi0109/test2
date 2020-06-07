#include "led_opr.h"


static int led_init(int which)
{
    printk(KERN_DEBUG  "myCode%s %s line %d\r\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

static int led_ioctl(int which, int states)
{
    printk(KERN_DEBUG  "myCode%s %s line %d\r\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

static led_operations gstBoard_led_Ops = {
    .init_fun    = led_init,
    .ioctl_fun   = led_ioctl,
};


static led_operations *get_led_operation(void)
{
    return &gstBoard_led_Ops;
}











