#include "menu.h"

// Variabel Global
volatile int menu_position = 0;     // 0: Start, 1: Set
volatile int setting_mode = 0;      // 0: Navigasi, 1: Set nilai
volatile uint32_t set_value = 0;
volatile int start_status = 0;      // 0: Stop, 1: Start// Nilai yang diatur
volatile int current_screen = 0;   // 0: Screen awal, 1: Screen kedua
volatile int secondary_menu_pos = 0; // 0: SCALE, 1: F, 2: OK
volatile int is_selecting_scaler = 0;          // Apakah user sedang memilih scaler
volatile int selected_scaler_index = 0;       // Indeks scaler yang dipilih
volatile int current_scaler_index = 0;
const int scaler_values[] = {1, 10, 100, 1000, 10000, 100000};  // Daftar scaler
volatile int is_selecting_frequency = 0;
volatile uint32_t last_set_value = 0;

static int last_menu_position = -1;
static int last_start_status = -1;
static int last_secondary_menu_pos = -1;
static int last_screen = -1; // Menyimpan screen terakhir yang ditampilkan


volatile uint32_t last_update_time = 0;  // Waktu terakhir perubahan encoder
volatile int encoder_speed = 1;          // Kecepatan encoder (default 1)

uint32_t last_blink_time = 0;  // Waktu terakhir blink
int blink_state = 1;           // 1: Tampilkan "SCALE", 0: Hapus "SCALE"


void menu_init(void) {
    menu_position = 0;
    setting_mode = 0;
    set_value = 0;
    start_status = 0;
}

void menu_update(void) {
    uint32_t current_time = HAL_GetTick();
    uint32_t delta_time = current_time - last_update_time;

    if (is_selecting_scaler)
    {  // Jika sedang memilih scaler
    	if(current_time - last_blink_time >= 500)
    	{
    		blink_state = !blink_state;  // Toggle blink state
    		last_blink_time = current_time;
    	}
        if (encoder_value > last_encoder_value) {
            if (selected_scaler_index < (sizeof(scaler_values) / sizeof(scaler_values[0])) - 1) {
                selected_scaler_index++;  // Pindah ke scaler berikutnya
            }
            last_encoder_value = encoder_value;
        } else if (encoder_value < last_encoder_value) {
            if (selected_scaler_index > 0) {
                selected_scaler_index--;  // Pindah ke scaler sebelumnya
            }
            last_encoder_value = encoder_value;
        }
        last_update_time = current_time;  // Perbarui waktu terakhir

    }
    else if(is_selecting_frequency)
    {
    	if(current_time - last_blink_time >= 500)
    	{
    		blink_state = !blink_state;  // Toggle blink state
    		last_blink_time = current_time;
    	}
        if (encoder_value > last_encoder_value) {
            set_value += scaler_values[selected_scaler_index];
            last_encoder_value = encoder_value;
        } else if (encoder_value < last_encoder_value) {
        	set_value -= scaler_values[selected_scaler_index];
            last_encoder_value = encoder_value;
        }
        last_update_time = current_time;  // Perbarui waktu terakhir
    }

    else if (current_screen == 0)
    {  // Update untuk screen1
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
    else if (current_screen == 1)
    {  // Update untuk screen2
        if (encoder_value > last_encoder_value) {  // Bergerak ke kanan
            if (secondary_menu_pos < 2) {         // Batas kanan
                secondary_menu_pos++;
            }
            last_encoder_value = encoder_value;  // Update posisi terakhir
        } else if (encoder_value < last_encoder_value) {  // Bergerak ke kiri
            if (secondary_menu_pos > 0) {         // Batas kiri
                secondary_menu_pos--;
            }
            last_encoder_value = encoder_value;  // Update posisi terakhir
        }
    }
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

    	if(!is_selecting_scaler || blink_state)
    	{
    		lcd_gotoxy(&lcd1, 1, 0);
    		lcd_puts(&lcd1, "SCALE:");
    	}
    	else
    	{
            lcd_gotoxy(&lcd1, 1, 0);
            lcd_puts(&lcd1, "     ");  // Hapus tulisan "SCALE" saat blink off
    	}

    	if(!is_selecting_frequency || blink_state)
    	{
    		lcd_gotoxy(&lcd1, 1, 1);
    		lcd_puts(&lcd1, "F:");
    	}
    	else
    	{
            lcd_gotoxy(&lcd1, 1, 1);
            lcd_puts(&lcd1, " ");  // Hapus tulisan "SCALE" saat blink off
    	}
        lcd_gotoxy(&lcd1, 14, 0);
        lcd_puts(&lcd1, "OK");

        // Jika sedang memilih scaler
        if (is_selecting_scaler) {
            lcd_gotoxy(&lcd1, 7, 0);
            lcd_puts(&lcd1, "       ");
            lcd_gotoxy(&lcd1, 7, 0);
            char scaler_buffer[10];
            sprintf(scaler_buffer, "%d", scaler_values[selected_scaler_index]);  // Tampilkan nilai scaler
            lcd_puts(&lcd1, scaler_buffer);
        }else {
            lcd_gotoxy(&lcd1, 7, 0);
            char scaler_buffer[10];
            sprintf(scaler_buffer, "%d", scaler_values[current_scaler_index]);  // Nilai scaler terakhir
            lcd_puts(&lcd1, scaler_buffer);
        }
        if (is_selecting_frequency)
        {
        	lcd_gotoxy(&lcd1, 3, 1);
        	lcd_puts(&lcd1, "                  ");
        	lcd_gotoxy(&lcd1, 3, 1);

        	char set_value_buffer[20];
        	format_number(set_value, set_value_buffer);
        	lcd_puts(&lcd1, set_value_buffer);
        }
        else
        {
        	lcd_gotoxy(&lcd1, 3, 1);
        	lcd_gotoxy(&lcd1, 3, 1);

        	char set_value_buffer[20];
        	format_number(last_set_value, set_value_buffer);
        	lcd_puts(&lcd1, set_value_buffer);
        }

        // Hapus semua arrow
        lcd_gotoxy(&lcd1, 0, 0);
        lcd_puts(&lcd1, " ");
        lcd_gotoxy(&lcd1, 0, 1);
        lcd_puts(&lcd1, " ");
        lcd_gotoxy(&lcd1, 13, 0);
        lcd_puts(&lcd1, " ");

        // Tampilkan arrow sesuai posisi
        if (!is_selecting_scaler || !is_selecting_frequency) {  // Hanya tampilkan arrow jika tidak memilih scaler
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
}

void format_number(uint32_t value, char *buffer) {
    char temp[16];
    sprintf(temp, "%u", value);  // Konversi angka ke string tanpa koma
    int len = strlen(temp);
    int comma_count = (len - 1) / 3;  // Hitung jumlah koma yang diperlukan
    int new_len = len + comma_count;

    buffer[new_len] = '\0';  // Terminasi string
    int j = new_len - 1;
    int counter = 0;

    // Salin karakter dan tambahkan koma
    for (int i = len - 1; i >= 0; i--) {
        buffer[j--] = temp[i];
        counter++;
        if (counter == 3 && i > 0) {
            buffer[j--] = '.';  // Tambahkan koma
            counter = 0;
        }
    }
}



