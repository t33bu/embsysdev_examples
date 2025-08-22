#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/timing/timing.h>

// I/O pin configurations
static const struct gpio_dt_spec red = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

// Red led thread initialization
#define STACKSIZE 500
#define PRIORITY 5
void red_led_task(void *, void *, void*);
K_THREAD_DEFINE(red_thread,STACKSIZE,red_led_task,NULL,NULL,NULL,PRIORITY,0,0);

bool red_state = false;

// Timer initializations
// ***************************
// Remember to add line:
// CONFIG_TIMING_FUNCTIONS=y
// to prj.conf
// ***************************

// #define DEBUG

// Initialize leds
void init_led() {

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
	// Timing: just for fun
	// Remember to add "CONFIG_TIMING_FUNCTIONS=y" to prj.conf
    timing_init();
    timing_start();
	timing_t start_time = timing_counter_get();

	init_led();

	// Wait for everything to initialize
	k_msleep(100);

	printk("Program started..\n");

	timing_t end_time = timing_counter_get();
	timing_stop();
    uint64_t timing_ns = timing_cycles_to_ns(timing_cycles_get(&start_time, &end_time));
	printk("Initialization: %lld\n", timing_ns);

	while (true) {
		k_msleep(100);
	}

	return 0;
}

// Task to handle red led
void red_led_task(void *, void *, void*) {
	
	printk("Red led thread started\n");
	while (true) {
    	timing_start();
		timing_t red_start_time = timing_counter_get();

		// 1. set led on 
		gpio_pin_set_dt(&red,1);
		// printk("Red on\n");
		
		// 2. sleep for 2 seconds
		k_sleep(K_SECONDS(1));
		
		// 3. set led off
		gpio_pin_set_dt(&red,0);
		// printk("Red off\n");
		
		// 4. sleep for 2 seconds
		k_sleep(K_SECONDS(1));

		// measure task duration
		timing_t red_end_time = timing_counter_get();
		timing_stop();
    	uint64_t timing_ns = timing_cycles_to_ns(timing_cycles_get(&red_start_time, &red_end_time));
		printk("Red task: %lld\n", timing_ns);
	}
}