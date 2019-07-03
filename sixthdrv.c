#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>
#include <linux/poll.h>


static struct class *sixthdrv_class;
static struct class_device *sixthdrv_class_dev;

static DECLARE_WAIT_QUEUE_HEAD(button_waitq);

/* 中断事件标志，中断服务程序将它置一，sixth_drv_read将它清零 */
static volatile int ev_press = 0;

static struct fasync_struct *button_async;

struct pin_desc{
    unsigned int pin;
    unsigned int key_val;
};

/* 键值：按下时，0x01, 0x02, 0x03, 0x04 */
