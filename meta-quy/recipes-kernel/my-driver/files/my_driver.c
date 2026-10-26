#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DEVICE_NAME "my_driver"
#define CLASS_NAME  "my_class"

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;

static int dev_open(struct inode *inodep, struct file *filep) {
    pr_info("my_driver: Device opened\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    char msg[] = "Hello from Character Driver in QEMU Yocto!\n";
    int msg_len = sizeof(msg);

    if (*offset >= msg_len) return 0;
    if (copy_to_user(buffer, msg, msg_len)) return -EFAULT;

    *offset += msg_len;
    return msg_len;
}

static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    pr_info("my_driver: Received write operation (%zs bytes)\n", len);
    return len;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    pr_info("my_driver: Device closed\n");
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init my_driver_init(void) {
    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    cdev_init(&my_cdev, &fops);
    cdev_add(&my_cdev, dev_num, 1);

    my_class = class_create(CLASS_NAME);
    device_create(my_class, NULL, dev_num, NULL, DEVICE_NAME);

    pr_info("my_driver: Module loaded successfully in QEMU!\n");
    return 0;
}

static void __exit my_driver_exit(void) {
    device_destroy(my_class, dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("my_driver: Module unloaded!\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nguyen Van Quy");
MODULE_DESCRIPTION("Character Device Driver for QEMU Yocto");
