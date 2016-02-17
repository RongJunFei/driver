#include <linux/cdev.h>
#include <linux/module.h>


int __init hello_init(void){
	printk("Hello Linux device!\n");
	return 0;
}

void __exit hello_exit(void){
	printk("Bye Linux device!\n");
}

MODULE_AUTHOR("rjf");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("The hello example");
module_init(hello_init);
module_exit(hello_exit);
