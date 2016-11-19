#ifndef KEY_H__
#define KEY_H__
#include "nrf.h"
#include "nrf_drv_config.h"
#include "boards.h"
extern const uint8_t buttons[BUTTONS_NUMBER];

void Key_init(void);
//static void Key_Scan_event_handler(void* p_context);
//static void Key_Scan_event_handler(nrf_timer_event_t event_type, void* p_context);
#endif
