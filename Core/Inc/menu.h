#ifndef MENU_H
#define MENU_H

#include "i2c_lcd.h"
#include "encoder.h"

#define MAX_MENU 1      // Menu paling kanan
#define MIN_MENU 0      // Menu paling kiri


// Variabel Global
extern volatile int menu_position;
extern volatile int setting_mode;
extern volatile uint32_t set_value;
extern volatile int start_status;  // 0: Stop, 1: Start
extern volatile int current_screen;   // 0: Screen awal, 1: Screen kedua
extern volatile int secondary_menu_pos; // 0: SCALE, 1: F, 2: OK
extern volatile int is_selecting_scaler;
extern volatile int selected_scaler_index;
extern const int scaler_values[];
extern volatile int current_scaler_index;
extern volatile int is_selecting_frequency;
extern volatile uint32_t last_set_value;
extern I2C_LCD_HandleTypeDef lcd1;

// Fungsi
void menu_init(void);
void menu_update(void);
void menu_display(void);

#endif
