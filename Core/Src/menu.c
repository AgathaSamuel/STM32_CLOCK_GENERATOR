#include "menu.h"

// Variabel Global
volatile int menu_position = 0;     // 0: Start, 1: Set
volatile int setting_mode = 0;      // 0: Navigasi, 1: Set nilai
volatile int set_value = 0;
volatile int start_status = 0;      // 0: Stop, 1: Start// Nilai yang diatur

static int last_menu_position = -1;
static int last_start_status = -1;

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
    // Navigasi menu
    // Perubahan nilai encoder
	if (setting_mode == 0){
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
	}
    // Pengaturan nilai
    else if (setting_mode == 1) {
        // Hitung kecepatan encoder berdasarkan delta waktu
        if (delta_time > 0) {
            encoder_speed = 100 / delta_time;  // Semakin cepat, semakin besar encoder_speed
            if (encoder_speed > 10) encoder_speed = 10;  // Batas maksimum percepatan
        }

        if (encoder_value > last_encoder_value) {
            set_value += encoder_speed;  // Tambahkan nilai sesuai kecepatan
            last_encoder_value = encoder_value;
        } else if (encoder_value < last_encoder_value) {
            set_value -= encoder_speed;  // Kurangi nilai sesuai kecepatan
            last_encoder_value = encoder_value;
        }

        if (set_value < 0) set_value = 0;

        // Simpan waktu terakhir update
        last_update_time = current_time;
    }
}

void menu_display(void) {
//    lcd_clear(&lcd1);

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

    // Update posisi menu hanya jika ada perubahan
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

}
