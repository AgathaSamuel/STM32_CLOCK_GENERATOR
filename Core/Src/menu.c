#include "menu.h"

// Variabel Global
volatile int menu_position = 0;     // 0: Start, 1: Set
volatile int setting_mode = 0;      // 0: Navigasi, 1: Set nilai
volatile int set_value = 0;
volatile int start_status = 0;      // 0: Stop, 1: Start// Nilai yang diatur


void menu_init(void) {
    menu_position = 0;
    setting_mode = 0;
    set_value = 0;
    start_status = 0;
}

void menu_update(void) {
    // Navigasi menu
    // Perubahan nilai encoder
    if (encoder_value > last_encoder_value) {  // Bergerak ke kanan
        if (menu_position < MAX_MENU) {       // Batas kanan
            menu_position++;
        }
        last_encoder_value = encoder_value;  // Update posisi terakhir
    } else if (encoder_value < last_encoder_value) {  // Bergerak ke kiri
        if (menu_position > MIN_MENU) {       // Batas kiri
            menu_position--;
        }
        last_encoder_value = encoder_value;  // Update posisi terakhir
    }
    // Pengaturan nilai
    else if (setting_mode == 1) {
        if (encoder_value > 0) {
            set_value++;
            encoder_value = 0;
        } else if (encoder_value < 0) {
            set_value--;
            encoder_value = 0;
        }

        if (set_value < 0) set_value = 0;

    }
}

void menu_display(void) {
    lcd_clear(&lcd1);

    lcd_gotoxy(&lcd1, 0, 0);
    lcd_puts(&lcd1, "F:");

    lcd_gotoxy(&lcd1, 0, 1);
    lcd_puts(&lcd1, "V:");

    lcd_gotoxy(&lcd1, 7, 1);
    if (start_status == 0){
    	lcd_puts(&lcd1, "START");
    }
    else if (start_status == 1){
    	lcd_puts(&lcd1, "STOP");
    }

    lcd_gotoxy(&lcd1, 13, 1);
    lcd_puts(&lcd1, "SET");

    if (menu_position == 0){
    	lcd_gotoxy(&lcd1, 6, 1);
    } else if (menu_position == 1){
    	lcd_gotoxy(&lcd1, 12, 1);
    }
    lcd_puts(&lcd1, ">");


}
