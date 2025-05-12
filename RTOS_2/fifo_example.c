#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>

/****************************
 * Remember to add line:
 * CONFIG_HEAP_MEM_POOL_SIZE=1024
 * to prj.conf
 ****************************/

// Thread initializations
void red_task(void *, void *, void*);
void green_task(void *, void *, void*);
#define STACKSIZE 500
#define PRIORITY 5
K_THREAD_DEFINE(red_thread,STACKSIZE,red_task,NULL,NULL,NULL,PRIORITY,0,0);
K_THREAD_DEFINE(green_thread,STACKSIZE,green_task,NULL,NULL,NULL,PRIORITY,0,0);

// Create FIFO buffer
K_FIFO_DEFINE(data_fifo);

// Data in FIFO
struct data_t {
	void *fifo_reserved;
	int count;
};

int main(void)
{

	// Wait for everything to initialize and threads to start
	// k_msleep(100);

	printk("Main thread runs once\n");

	return 0;
}

void red_task(void *, void *, void*) {

	int count = 0;

	while (true) {

		struct data_t *buf = k_malloc(sizeof(struct data_t));
		if (buf == NULL) {
			return;
		}
		buf->count = count;
		k_fifo_put(&data_fifo, buf);
		printk("Red added to fifo: %d\n",buf->count);

		// Increase counter
		count++;

		// Add data to fifo every 1s
		k_msleep(1000);
	}
}

void green_task(void *, void *, void*) {

	// Store received data
	struct data_t *received;

	while (true) {

		received = k_fifo_get(&data_fifo, K_FOREVER);
		printk("Green received from fifo: %d\n", received->count);
		k_free(received);

		k_yield();
	}
}

