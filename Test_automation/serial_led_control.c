#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>

// I/O pin configurations
static const struct gpio_dt_spec green = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);

// UART initialization
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)
static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

int main(void)
{
	// Led pin initialization
	int ret = gpio_pin_configure_dt(&green, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}
	// set led off
	gpio_pin_set_dt(&green,0);

	// UART initialization
	if (!device_is_ready(uart_dev)) {
		printk("UART initialization failed!\r\n");
		return 0;
	} 

	// Wait for everything to initialize and threads to start
	k_sleep(K_SECONDS(1));
	// Sanity check
	printk("Started serial led example\n");

	// superloop
	char c=0;
	while (true) {
		if (uart_poll_in(uart_dev,&c) == 0) {
			if (c == '0') {
				gpio_pin_set_dt(&green,0);
				uart_poll_out(uart_dev, c);
			} else if (c == '1') {
				gpio_pin_set_dt(&green,1);
				uart_poll_out(uart_dev, c);
			}
		}
	}
	return 0;
}
