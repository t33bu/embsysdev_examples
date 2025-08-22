// *******************************
// This example declares two tasks
// and runs them in parallel

#include <zephyr/kernel.h>

// Task declarations
void task1(void *, void *, void*);
void task2(void *, void *, void*);
#define	STACKSIZE	500
#define	PRIORITY	5
K_THREAD_DEFINE(tid1,STACKSIZE,task1,NULL,NULL,NULL,PRIORITY,0,0);
K_THREAD_DEFINE(tid2,STACKSIZE,task2,NULL,NULL,NULL,PRIORITY,0,0);

// Main program
int main(void) {
	while (true) {
		printk("Hello from main\n");
		k_msleep(1000);
		// k_yield();
	}
	
	return 0;
}

// Task1 function
void task1(void *, void *, void*) {

	while (true) {
		printk("Hello from task 1\n");
		k_msleep(3000);
		// k_yield();
	}
}

// Task2 function
void task2(void *, void *, void*) {

	while (true) {
		printk("Hello from task 2\n");
		k_msleep(5000);
		// k_yield();
	}
}
