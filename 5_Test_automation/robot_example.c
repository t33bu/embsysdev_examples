#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

// UART initialization
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)
static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

int main(void)
{
	// UART initialization
	if (!device_is_ready(uart_dev)) {
		printk("UART initialization failed!\r\n");
		return 0;
	} 

	// Wait for everything to initialize and threads to start
	k_msleep(100);
	// Sanity check
	printk("Started serial led example\n");

	/************************************************
	 * HOX! MUOKATKAA OMA UART_TASK TÄMÄN AO.
	 * WHILE-LOOPIN MUKAISEKSI
	 * (ei siis käytetä superlooppia)
	 ***********************************************/
	char rc=0;
	char uart_msg[20];
	int uart_msg_cnt=0;
	memset(uart_msg,0,20);			
	while (true) {
		// Ask UART if data available
		while (uart_poll_in(uart_dev,&rc) == 0) {
			// Add characters into buffer until X is received 
			if (rc != 'X') {
				uart_msg[uart_msg_cnt] = rc;
				uart_msg_cnt++;
			} else {
				// Send response to UART
				// X has been removed, so we need to add it to the end
				// printk("-1X"); example return error

				// Example check with time_parse
				int timer_delay = time_parse(uart_msg); 
				printk("%dX",timer_delay);

				// Clear UART message buffer
				uart_msg_cnt = 0;
				memset(uart_msg,0,20);			
			}
		}
		k_msleep(10);
	}
	return 0;
}

int time_parse(char *str) {
	// testi: palauttaa virhekoodin -1
	return -1;
}