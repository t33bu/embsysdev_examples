#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

// Condition Variables
K_MUTEX_DEFINE(red_mutex);
K_CONDVAR_DEFINE(red_signal);
K_MUTEX_DEFINE(green_mutex);
K_CONDVAR_DEFINE(green_signal);

// Thread initializations
#define STACKSIZE 500
#define PRIORITY 5
void red_task(void *, void *, void*);
K_THREAD_DEFINE(red_thread,STACKSIZE,red_task,NULL,NULL,NULL,PRIORITY,0,0);
void green_task(void *, void *, void*);
K_THREAD_DEFINE(green_thread,STACKSIZE,green_task,NULL,NULL,NULL,PRIORITY,0,0);

int main(void)
{

	// Wait for everything to initialize and threads to start
	k_msleep(100);

	printk("Main thread runs\n");

	// Start sequence
	k_condvar_broadcast(&red_signal);

	return 0;
}

void red_task(void *, void *, void*) {
	while (true) {
		// Wait for signal
		k_condvar_wait(&red_signal, &red_mutex, K_FOREVER);

		printk("Red thread runs\n");

		k_condvar_broadcast(&green_signal);
		// Sleep nor yield not needed
		// k_yield();
	}
}

void green_task(void *, void *, void*) {
	while (true) {
		// Wait for signal
		k_condvar_wait(&green_signal, &green_mutex, K_FOREVER);

		printk("Green thread runs\n");

		k_condvar_broadcast(&red_signal);
		// Sleep nor yield not needed
		// k_yield();
	}
}

