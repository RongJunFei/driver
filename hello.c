#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include "command.h"

#define MAX 127

static int mymajor = 245;
static int myminor = 0;
static int number_of_devices = 1;
static char data[MAX - 1] = "The device name is hello. \n";

struct class * hello_class;
struct device * hello_device;

static struct cdev cdev;

static int hello_open(struct inode *inode, struct file *file){
	printk("Hello, the device opened \n");
	return 0;
}

static ssize_t hello_read(struct file *filep, char __user * buff, size_t count, loff_t * offp) {
	int ret = 0;

	if(*offp == 0){
		if(count > MAX)
			count = MAX;
		copy_to_user(buff, data, count);
		ret = count;
	}
	*offp = ret;

	return ret;
}

static ssize_t hello_write(struct file *filep, const char __user * buff, size_t count, loff_t *offp){
	ssize_t ret = 0;
	if(count > MAX){
		ret = -EFAULT;
	}else {
		memset(data, 0, MAX);
		copy_from_user(data, buff, count);
		data[count] = '\0';
		ret = count;
	}
	return ret;
}

static long hello_ioctl(struct file * filep, unsigned int cmd, unsigned long arg){
	long ret = 0;

	switch(cmd){
	case ONE:
		printk("The command is one \n");
		break;
	case TWO:
		printk("The command is two \n");
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}

static struct file_operations fops={
	.owner = THIS_MODULE,
	.open = hello_open,
	.read = hello_read,
	.write = hello_write,
	.unlocked_ioctl = hello_ioctl,
};

static void char_reg_cdev(void){
	printk("Hello init function! \n");
}

static int __init hello_init(void){
	int ret;
	dev_t devnu = 0;

	devnu = MKDEV(mymajor, myminor);

	ret = register_chrdev_region(devnu, number_of_devices, "hello");
	if(ret < 0){
		printk("Hello: can't get major number %d \n", mymajor);
		return ret;
	}

	cdev_init(&cdev, &fops);
	cdev.owner = THIS_MODULE;
	ret = cdev_add(&cdev, devnu, 1);
	if(ret){
		printk("Error %d adding cdev add! \n", ret);
		goto err1;
	}

	char_reg_cdev();
	
	hello_class = class_create(THIS_MODULE, "hello");
	if(IS_ERR(hello_class)){
		printk("Err: failed in createing class! \n");
		goto err2;
	}

	hello_device = device_create(hello_class, NULL, devnu, NULL, "coll");
	if(IS_ERR(hello_device)){
		printk("Err: failed in createing device! \n");
		goto err3;
	}

	printk("Register character driver \'hello\'! \n");
	return 0;

err3:
	class_destroy(hello_class);
err2:
	cdev_del(&cdev);
err1:
	unregister_chrdev_region(devnu, number_of_devices);
	return ret;

}

static void __exit hello_exit(void){
	dev_t devnu = 0;

	devnu = MKDEV(mymajor, myminor);
	cdev_del(&cdev);
	unregister_chrdev_region(devnu, number_of_devices);
	class_destroy(hello_class);
	device_destroy(hello_class,devnu);

	printk("Bye driver \'hello\'!\n");
}

MODULE_AUTHOR("rjf");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("The hello example");
module_init(hello_init);
module_exit(hello_exit);
