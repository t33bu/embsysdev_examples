#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/timing/timing.h>

/****************************
 * Remember to add lines:
 * CONFIG_TIMING_FUNCTIONS=y
 * CONFIG_HEAP_MEM_POOL_SIZE=1024
 * to prj.conf
 ****************************/

// Thread initializations
void red_task(void *, void *, void*);
void debug_task(void *, void *, void*);
#define STACKSIZE 1024
#define PRIORITY 5
K_THREAD_DEFINE(red_thread,STACKSIZE,red_task,NULL,NULL,NULL,PRIORITY,0,0);
K_THREAD_DEFINE(debug_thread,STACKSIZE,debug_task,NULL,NULL,NULL,PRIORITY,0,0);

// I/O pin configurations
static const struct gpio_dt_spec red = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

// Create FIFO buffer
K_FIFO_DEFINE(data_fifo);

// Data in FIFO
struct data_t {
	void *fifo_reserved;
	uint64_t time;
};

// Initialize leds
void init_leds() {

	// Led pin initialization
	int ret = gpio_pin_configure_dt(&red, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}
	// set led off
	gpio_pin_set_dt(&red,0);

	printk("Led initialized ok\n");
}

int main(void)
{
	init_leds();
    timing_init();

	// Wait for everything to initialize and threads to start
	// k_msleep(100);

	printk("Main thread runs once\n");

	return 0;
}

void red_task(void *, void *, void*) {

	while (true) {

    	timing_start();
		timing_t start_time = timing_counter_get();

		gpio_pin_set_dt(&red,1);
		printk("Red on\n");
		k_sleep(K_SECONDS(1));
		
		gpio_pin_set_dt(&red,0);
		printk("Red off\n");
		k_sleep(K_SECONDS(1));

		struct data_t *buf = k_malloc(sizeof(struct data_t));
		if (buf == NULL) {
			return;
		}
        
		timing_stop();
		timing_t end_time = timing_counter_get();
        uint64_t diff = timing_cycles_to_ns(timing_cycles_get(&start_time, &end_time));

		buf->time = diff;
		k_fifo_put(&data_fifo, buf);
		// printk("Red added to fifo: %lld\n",buf->time);

		k_yield();
	}
}

void debug_task(void *, void *, void*) {

	// Store received data
	struct data_t *received;

	while (true) {

		received = k_fifo_get(&data_fifo, K_FOREVER);
		printk("Debug received: %lld\n", received->time);
		k_free(received);

		k_yield();
	}
}
