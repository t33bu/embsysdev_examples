#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>

// Led thread initialization
void red_task(void *, void *, void*);
void green_task(void *, void *, void*);
#define STACKSIZE 500
#define PRIORITY 5
// Simple way
// K_THREAD_DEFINE(red_thread,STACKSIZE,red_led_task,NULL,NULL,NULL,PRIORITY,0,0);
// K_THREAD_DEFINE(green_thread,STACKSIZE,green_led_task,NULL,NULL,NULL,PRIORITY,0,0);
K_THREAD_STACK_DEFINE(red_stack_area, STACKSIZE);
K_THREAD_STACK_DEFINE(green_stack_area, STACKSIZE);
struct k_thread red_thread_data;
struct k_thread green_thread_data;

int led_state = 0;

// Main program
int main(void)
{

	printk("Main thread runs\n");

	// Thread ids
	k_tid_t red_tid;
	k_tid_t green_tid;	

	while (true) {
		// Create threads
		red_tid = k_thread_create(&red_thread_data, red_stack_area, STACKSIZE,
                                 red_task,NULL, NULL, NULL,PRIORITY, 0, K_FOREVER);
		green_tid = k_thread_create(&green_thread_data, green_stack_area, STACKSIZE,
                                 green_task,NULL, NULL, NULL,PRIORITY, 0, K_FOREVER);
		switch(led_state) {
			case 0:
				// Run red thread once
				k_thread_start(red_tid);
				led_state = 1;
				break;
			case 1:
				// Run red thread once
				k_thread_start(green_tid);
				led_state = 0;
				break;
			default:
				k_oops();
		}
		k_msleep(10);
	}

	return 0;
}

// Task to handle red led
void red_task(void *, void *, void*) {

	printk("Red thread runs\n");
}

// Task to handle green led
void green_task(void *, void *, void*) {
	
	printk("Green thread runs\n");
}

