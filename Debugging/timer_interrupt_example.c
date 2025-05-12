#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

// I/O pin configurations
static const struct gpio_dt_spec red = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

// Timer initializations
struct k_timer timer;
void timer_handler(struct k_timer *timer_id);
bool red_state = false;

int main(void)
{
	// Led pin initialization
	int ret = gpio_pin_configure_dt(&red, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}
	// set led off
	gpio_pin_set_dt(&red,0);

	// Timer initialization
	k_timer_init(&timer, timer_handler, NULL);
	k_timer_start(&timer, K_SECONDS(1), K_SECONDS(1)); // start delay 1s, period 1s

	// Wait for everything to initialize
	k_msleep(100);

	// Sanity check
	printk("Program started..\n");

	while (true) {
		// debug how many times timer has expired since last 10 seconds
		uint32_t num = k_timer_status_get(&timer);
		printk("Expired: %d\n",num);
		k_sleep(K_SECONDS(10));
	}

	return 0;
}

// control led using timer interrupt
void timer_handler(struct k_timer *timer_id) {
	if (red_state == false) {
		gpio_pin_set_dt(&red,1);
		red_state = true;
	} else {
		gpio_pin_set_dt(&red,0);
		red_state = false;
	}
}

