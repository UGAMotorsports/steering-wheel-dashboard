/*
 * easyusbprintln.c
 *
 *  Created on: May 10, 2025
 *      Author: bhargav
 */

#include "easyusbprintln.h"

#include "usb_device.h"
#include <stdarg.h>

extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

void USB_Println(const char *format, ...) {
    char buffer[128];  // Adjust size as needed
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    CDC_Transmit_FS((uint8_t*)buffer, strlen(buffer));
}
