#include <SPI.h>          // Library untuk komunikasi SPI (Serial Peripheral Interface)
#include <nRF24L01.h>     // Definisi register dan konstanta khusus modul NRF24L01
#include <RF24.h>         // Library utama untuk mengontrol modul NRF24L01


/* 
  Arduino Uno + modul NRF24L01

  Koneksi SPI Arduino Uno ke modul NRF24L01:
  -----------------------------------------

  NRF24L01    Arduino Uno
  --------    -----------
  VCC         5V                  // Catu daya 5V
  GND         GND                 // Ground
  CE          D7                  // Chip Enable (mengaktifkan mode TX/RX)
  CSN (CS)    D8                  // Chip Select untuk komunikasi SPI
  SCK         D13                 // Clock SPI
  MOSI        D11                 // Master Out Slave In
  MISO        D12                 // Master In Slave Out
  IRQ         Tidak digunakan

  Catatan:
  - Modul NRF24L01 wajib menggunakan tegangan 3.3V (jangan hubungkan ke 5V).
  - Untuk modul NRF24L01 PA+LNA (dengan antena eksternal), sebaiknya:
    1. Tambahkan kapasitor 10uF–100uF di antara pin VCC dan GND, atau
    2. Gunakan adapter NRF24L01 dengan regulator 3.3V.
*/

// Membuat objek radio dengan konfigurasi:
// CE  -> Pin D7
// CSN -> Pin D8
RF24 radio(7, 8);


// Fungsi untuk mengirim 1 karakter ke Serial
int serial_putc(char c, FILE *) {
  Serial.write(c);
  return c;
}


// Fungsi untuk mengaktifkan printf() ke Serial Monitor
void printf_begin(void) {
  static FILE serial_stdout;
  fdev_setup_stream(&serial_stdout, serial_putc, NULL, _FDEV_SETUP_WRITE);
  stdout = &serial_stdout;
}


void setup() {
  // Memulai komunikasi Serial Monitor dengan baudrate 115200 bps
  Serial.begin(115200);

  // Memberi waktu agar Serial Monitor siap menerima data
  delay(3000);

  // Menampilkan judul program di Serial Monitor
  Serial.print("=== TEST NRF24L01 PADA ARDUINO UNO ===");

  printf_begin();
  
  // Pastikan semua data Serial selesai dikirim
  Serial.flush();

  // Inisialisasi SPI
  // Pada Arduino Uno cukup menggunakan SPI.begin()
  // karena pin SPI sudah fixed:
  // MOSI = D11, MISO = D12, SCK = D13
  SPI.begin();
}


void loop() {
  // Mencoba menginisialisasi modul NRF24L01
  // Fungsi radio.begin() akan mengembalikan:
  // - true  -> jika modul berhasil terdeteksi
  // - false -> jika modul tidak ditemukan atau gagal komunikasi
  if (!radio.begin()) {

    // Jika modul tidak terdeteksi, tampilkan pesan error
    Serial.println("\n\n❌ NRF24L01 TIDAK TERDETEKSI");

    // Menampilkan kemungkinan penyebab kegagalan
    Serial.println("Kemungkinan:");
    Serial.println("- Wiring salah");
    Serial.println("- Adapter NRF24L01 rusak");
    Serial.println("- Modul NRF24L01 rusak");
    Serial.println("- Suplai 3.3V tidak stabil");

    // Program dihentikan di sini (infinite loop)
    // ESP32 akan berhenti dan tidak melanjutkan eksekusi
    while (1);
  }

  // Jika modul berhasil terdeteksi
  Serial.println("\n\n✅ NRF24L01 TERDETEKSI");
  Serial.println("Modul kondisinya masih baik.");
  Serial.println();

  // Menampilkan informasi detail modul ke Serial Monitor, seperti:
  // - Register konfigurasi
  // - Channel
  // - Data rate
  // - PA level
  // - CRC
  // - Address pipe
  radio.printDetails();

  // Tunggu 2 detik sebelum melakukan pengecekan ulang
  delay(2000);
}