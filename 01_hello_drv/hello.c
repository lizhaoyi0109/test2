#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <linux/cdev.h>

#define MIN(a, b) (a < b ? a : b)


static int guiMojorHello = 0;
static struct cdev gpstHelle_cdev;
static struct class *gpstHello_class;
static int guiDevnoCount = 1;


static char kernel_buf[1024];


module_param(guiMojorHello, int, 0644);
MODULE_PARM_DESC(guiMojorHello, "Major device number");

module_param(guiDevnoCount, int, 0644);
MODULE_PARM_DESC(guiDevnoCount, "device number");



static int hello_drv_open (struct inode *node, struct file *file)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

static ssize_t hello_drv_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
    int err;
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    err = copy_to_user(buf, kernel_buf, MIN(1024, size));
    return MIN(1024, size);

}

static ssize_t hello_drv_write (struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
	int err;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	err = copy_from_user(kernel_buf, buf, MIN(1024, size));
	return MIN(1024, size);
}


static int hello_drv_close (struct inode *node, struct file *file)
{
	printk(KERN_DEBUG "%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

struct file_operations stHello_drv = {
    .owner	 = THIS_MODULE,
    .open    = hello_drv_open,
    .read    = hello_drv_read,
    .write   = hello_drv_write,
    .release = hello_drv_close,
};

static int __init hello_drv_init(void)
{
    dev_t uiDevno = 0;
    int uiRes = 0;
    int i = 0;
    
    printk(KERN_DEBUG  "myCode%s %s line %d\r\n", __FILE__, __FUNCTION__, __LINE__);
    if(guiMojorHello)
    {
        uiDevno = MKDEV(guiMojorHello, 0);
        uiRes = register_chrdev_region(uiDevno, guiDevnoCount, "hello");
    }
    else
    {
        uiRes = alloc_chrdev_region(&uiDevno, 0, guiDevnoCount, "hello");
        guiMojorHello = MAJOR(uiDevno);
    }
    if(uiRes)
    {
        printk(KERN_DEBUG  "myCode%s %s line %d\r\n", __FILE__, __FUNCTION__, __LINE__);
        return -ENOMEM;
    }

    /*gpstHelle_cdev = cdev_alloc();
    if(IS_ERR(gpstHelle_cdev))
    {
        printk("%s %s line %d", __FILE__, __FUNCTION__, __LINE__);
        unregister_chrdev_region(uiDevno, guiDevnoCount);
        return -ENOMEM;
    }*/
    cdev_init(&gpstHelle_cdev, &stHello_drv);
    gpstHelle_cdev.owner = THIS_MODULE;
    uiRes = cdev_add(&gpstHelle_cdev, uiDevno, guiDevnoCount);
    if(uiRes)
    {
        printk(KERN_DEBUG  "myCode%s %s line %d\r\n", __FILE__, __FUNCTION__, __LINE__);
        //cdev_put(gpstHelle_cdev);
        unregister_chrdev_region(uiDevno, guiDevnoCount);
        return -1;
    }

    gpstHello_class = class_create(THIS_MODULE, "hello_class");
    if(IS_ERR(gpstHello_class))
    {
        printk(KERN_DEBUG "myCode%s %s line %d\r\n", __FILE__, __FUNCTION__, __LINE__);
        cdev_del(&gpstHelle_cdev);
        //cdev_put(gpstHelle_cdev);
        unregister_chrdev_region(uiDevno, guiDevnoCount);
        return -1;
    }

    for(i = 0; i < guiDevnoCount; i++)
    {
        device_create(gpstHello_class, NULL, MKDEV(guiMojorHello, i), NULL, "hello%d", i);
    }
    return 0;
    
}

static void __exit hello_drv_exit(void)
{
    dev_t uiDevno = 0;
    int i = 0;
    
    printk(KERN_DEBUG "myCode%s %s line %d\r\n", __FILE__, __FUNCTION__, __LINE__);
    
    for(i = 0; i < guiDevnoCount;i++)
    {
        device_destroy(gpstHello_class, MKDEV(guiMojorHello, i ));
    }
    class_destroy(gpstHello_class);
    cdev_del(&gpstHelle_cdev);
    /*cdev_put(gpstHelle_cdev);*/
    
    unregister_chrdev_region(uiDevno, guiDevnoCount);
}




module_init(hello_drv_init);
module_exit(hello_drv_exit);
MODULE_LICENSE("GPL");




