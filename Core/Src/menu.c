#include "menu.h"

// Variabel Global
volatile int menu_position = 0;     // 0: Start, 1: Set
volatile int setting_mode = 0;      // 0: Navigasi, 1: Set nilai
volatile int set_value = 0;
volatile int start_status = 0;      // 0: Stop, 1: Start// Nilai yang diatur
volatile int current_screen = 0;   // 0: Screen awal, 1: Screen kedua
volatile int secondary_menu_pos = 0; // 0: SCALE, 1: F, 2: OK


static int last_menu_position = -1;
static int last_start_status = -1;
static int last_secondary_menu_pos = -1;
static int last_screen = -1; // Menyimpan screen terakhir yang ditampilkan


volatile uint32_t last_update_time = 0;  // Waktu terakhir perubahan encoder
volatile int encoder_speed = 1;          // Kecepatan encoder (default 1)


void menu_init(void) {
    menu_position = 0;
    setting_mode = 0;
    set_value = 0;
    start_status = 0;
}

void menu_update(void) {
    uint32_t current_time = HAL_GetTick();  // Waktu saat ini dalam milidetik
    uint32_t delta_time = current_time - last_update_time;  // Selisih waktu

    if (current_screen == 0) {  // Screen awal
        // Navigasi menu utama
        if (setting_mode == 0) {
            if (encoder_value > last_encoder_value) {  // Bergerak ke kanan
                if (menu_position < MAX_MENU) {  // Batas kanan
                    menu_position++;
                }
                last_encoder_value = encoder_value;  // Update posisi terakhir
            } else if (encoder_value < last_encoder_value) {  // Bergerak ke kiri
                if (menu_position > MIN_MENU) {  // Batas kiri
                    menu_position--;
                }
                last_encoder_value = encoder_value;  // Update posisi terakhir
            }
        }
    } else if (current_screen == 1) {  // Screen kedua
        // Navigasi screen kedua
        if (encoder_value > last_encoder_value) {  // Bergerak ke bawah
            if (secondary_menu_pos < 2) {  // Batas bawah
                secondary_menu_pos++;
            }
            last_encoder_value = encoder_value;
        } else if (encoder_value < last_encoder_value) {  // Bergerak ke atas
            if (secondary_menu_pos > 0) {  // Batas atas
                secondary_menu_pos--;
            }
            last_encoder_value = encoder_value;
        }
    }

    // Simpan waktu terakhir update
    last_update_time = current_time;
}

void menu_display(void) {
    if (current_screen != last_screen) {
        lcd_clear(&lcd1);  // Bersihkan layar hanya saat berpindah screen
        last_screen = current_screen;  // Simpan screen terakhir
        if (current_screen == 0) {
            last_start_status = -1;  // Paksa pembaruan ulang tulisan "START"/"STOP"
            last_menu_position = -1;  // Paksa pembaruan ulang posisi menu
        }
    }

    if (current_screen == 0) {  // Screen awal
        lcd_gotoxy(&lcd1, 0, 0);
        lcd_puts(&lcd1, "F:");

        lcd_gotoxy(&lcd1, 0, 1);
        lcd_puts(&lcd1, "V:");

        lcd_gotoxy(&lcd1, 13, 1);
        lcd_puts(&lcd1, "SET");

        if (start_status != last_start_status) {
            lcd_gotoxy(&lcd1, 7, 1);
            if (start_status == 0) {
                lcd_puts(&lcd1, "START");
            } else if (start_status == 1) {
                lcd_puts(&lcd1, "STOP ");
            }
            last_start_status = start_status;  // Simpan status terakhir
        }

        if (menu_position != last_menu_position) {
            // Hapus arrow dari posisi sebelumnya
            if (last_menu_position == 0) {
                lcd_gotoxy(&lcd1, 6, 1);
                lcd_puts(&lcd1, " ");
            } else if (last_menu_position == 1) {
                lcd_gotoxy(&lcd1, 12, 1);
                lcd_puts(&lcd1, " ");
            }

            // Tampilkan arrow di posisi baru
            if (menu_position == 0) {
                lcd_gotoxy(&lcd1, 6, 1);
                lcd_puts(&lcd1, ">");
            } else if (menu_position == 1) {
                lcd_gotoxy(&lcd1, 12, 1);
                lcd_puts(&lcd1, ">");
            }

            last_menu_position = menu_position;  // Simpan posisi terakhir
        }
    } else if (current_screen == 1) {  // Screen kedua
        lcd_gotoxy(&lcd1, 1, 0);
        lcd_puts(&lcd1, "SCALE:");

        lcd_gotoxy(&lcd1, 1, 1);
        lcd_puts(&lcd1, "F:");

        lcd_gotoxy(&lcd1, 14, 0);
        lcd_puts(&lcd1, "OK");

        // Hapus semua arrow
        lcd_gotoxy(&lcd1, 0, 0);
        lcd_puts(&lcd1, " ");
        lcd_gotoxy(&lcd1, 0, 1);
        lcd_puts(&lcd1, " ");
        lcd_gotoxy(&lcd1, 13, 0);
        lcd_puts(&lcd1, " ");

        // Tampilkan arrow sesuai posisi
        if (secondary_menu_pos == 0) {  // Arrow di "SCALE"
            lcd_gotoxy(&lcd1, 0, 0);
            lcd_puts(&lcd1, ">");
        } else if (secondary_menu_pos == 1) {  // Arrow di "F"
            lcd_gotoxy(&lcd1, 0, 1);
            lcd_puts(&lcd1, ">");
        } else if (secondary_menu_pos == 2) {  // Arrow di "OK"
            lcd_gotoxy(&lcd1, 13, 0);
            lcd_puts(&lcd1, ">");
        }
    }
}


