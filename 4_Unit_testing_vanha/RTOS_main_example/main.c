#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

// UART initialization
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)
static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

// error codes
#define COLOR_ERROR		1
#define TIME_ERROR		2
#define COMMAND_ERROR	3
#define COMMAND_OK		0

// Parser
int parser(char *command);

int main(void)
{
	// UART initialization
	if (!device_is_ready(uart_dev)) {
		printk("UART initialization failed!\r\n");
		return 0;
	} 

	// Wait for everything to initialize and threads to start
	k_sleep(K_SECONDS(1));
	// Sanity check
	// printk("Started serial led example\n");

	// UART helpers
	char c=0;
	int cnt = 0;
	char buffer[16];

	// superloop
	while (true) {

		if (uart_poll_in(uart_dev,&c) == 0) {
			if (c == '\n' || c == '\r') {
				// printk(buffer);
				// here you call parser
				int ret = parser(buffer);
				// check parser return value
				if (ret == COMMAND_OK) {
					// send signal / message to mailbox
				} 
				// clear buffer and counter
				cnt = 0;
				memset(buffer,0,16);
			} else {
				// add received character to buffer
				buffer[cnt] = c;
				cnt++;
			}
		}
	}
	return 0;
}

int parser(char *command) {
	int ret = COMMAND_OK;

	// add your own code from googletest here!!
	// to check the buffer for correct sequence
	
	return ret;
}
