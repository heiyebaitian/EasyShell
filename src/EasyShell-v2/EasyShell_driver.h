#ifndef __EASY_SHELL_DRIVER__H
#define __EASY_SHELL_DRIVER__H
#pragma once

#include "EasyShell.h"

uint8_t eshell_serial_available();
uint32_t eshell_serial_read();
uint32_t eshell_printf(const char *format, ...);

#endif