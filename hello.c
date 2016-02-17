
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>

static int mymajor = 245;
static int myminor = 0;
static int number_of_devices = 1;

static struct cdev cdev;
static int hello_open(struct inode *inode, struct file *file){
	printk("Hello, the device opened \n");
	return 0;
}

static struct file_operations fops={
	.owner = THIS_MODULE,
	.open = hello_open,
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

	printk("Register character driver \'hello\'! \n");
	return 0;
err1:
	unregister_chrdev_region(devnu, number_of_devices);
	return ret;
	
}

static void __exit hello_exit(void){
	dev_t devnu = 0;
	devnu = MKDEV(mymajor, myminor);
	cdev_del(&cdev);
	unregister_chrdev_region(devnu, number_of_devices);
	printk("Bye driver \'hello\'!\n");
}

MODULE_AUTHOR("rjf");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("The hello example");
module_init(hello_init);
module_exit(hello_exit);
