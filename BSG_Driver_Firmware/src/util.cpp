#include "util.h"

#include <Arduino.h>
#include <errno.h>
#include <sys/unistd.h>

extern "C" {
    // enable printf functionality
    int _write(int file, char *data, int len) {
        if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
            errno = EBADF;
            return -1;
        }
        
        size_t written = Serial.write(data, len);
        Serial.flush();
        return written;
    }
}

void printHex(const uint8_t* buf, uint16_t size) {
	printf("       ");
	for(uint8_t i = 0; i < 16; i++) {
		printf("%1X  ", i);
	}
	printf("\n%04X  ", 0);
	for(uint16_t i = 0; i < size; i++) {
		printf("%02X ", buf[i]);
		if(i % 16 == 15) {
			printf("\n%04X  ", i+1);
		}
	}
	printf("\n");
}

// enable SWD debug (standard configuration claims USB_NOE (PA13) for whatever stupid reason -.-)
const PinMap PinMap_USB[] = {
  {PA_11, USB, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, GPIO_AF_NONE)}, // USB_DM
  {PA_12, USB, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, GPIO_AF_NONE)}, // USB_DP
  // {PA_13, USB, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_USB)}, // USB_NOE
  {NC,    NP,  0}
};