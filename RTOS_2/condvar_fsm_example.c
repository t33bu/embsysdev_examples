#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>

// Led thread initialization
void red_task(void *, void *, void*);
void green_task(void *, void *, void*);
#define STACKSIZE 500
#define PRIORITY 5
K_THREAD_DEFINE(red_thread,STACKSIZE,red_task,NULL,NULL,NULL,PRIORITY,0,0);
K_THREAD_DEFINE(green_thread,STACKSIZE,green_task,NULL,NULL,NULL,PRIORITY,0,0);

// Condition variables
K_MUTEX_DEFINE(thread_mutex);
K_CONDVAR_DEFINE(red_signal);
K_CONDVAR_DEFINE(green_signal);

int led_state = 0;

// Main program
int main(void)
{

	printk("Main thread runs\n");

	while (true) {
		switch(led_state) {
			case 0:
				// Run red thread once
				k_condvar_broadcast(&red_signal);
				led_state = 1;
				break;
			case 1:
				// Run green thread once
				k_condvar_broadcast(&green_signal);
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

	while(true) {
		k_condvar_wait(&red_signal, &thread_mutex, K_FOREVER);
		printk("Red thread runs\n");
	}
}

// Task to handle green led
void green_task(void *, void *, void*) {
	
	while(true) {
		k_condvar_wait(&green_signal, &thread_mutex, K_FOREVER);
		printk("Green thread runs\n");
	}
}

