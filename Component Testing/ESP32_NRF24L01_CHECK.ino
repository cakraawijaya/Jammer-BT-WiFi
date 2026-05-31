#include <SPI.h>          // Library untuk komunikasi SPI (Serial Peripheral Interface)
#include <nRF24L01.h>     // Definisi register dan konstanta khusus modul NRF24L01
#include <RF24.h>         // Library utama untuk mengontrol modul NRF24L01


/* 
  ESP32 + modul NRF24L01

  Koneksi SPI ESP32 ke modul NRF24L01:
  --------------------------------

  NRF24L01    ESP32
  --------    -----
  VCC         3.3V                // Catu daya 3.3V
  GND         GND                 // Ground
  CE          GPIO 4              // Chip Enable (mengaktifkan mode TX/RX)
  CSN (CS)    GPIO 5              // Chip Select untuk komunikasi SPI
  SCK         GPIO 18             // Clock SPI
  MOSI        GPIO 23             // Master Out Slave In
  MISO        GPIO 19             // Master In Slave Out
  IRQ         Tidak digunakan

  Catatan:
  - modul NRF24L01 PA+LNA wajib menggunakan tegangan 3.3V (jangan hubungkan ke 5V).
  - Untuk modul NRF24L01 PA+LNA, agar suplai daya lebih stabil sebaiknya:
    1. Menambahkan kapasitor 10uF–100uF di antara pin VCC dan GND atau
    2. Gunakan regulator 3.3V eksternal (pakai adapter NRF24L01)
*/

// Membuat objek radio dengan konfigurasi:
// CE  -> GPIO 4
// CSN -> GPIO 5
RF24 radio(4, 5);


void setup() {
  // Memulai komunikasi Serial Monitor dengan baudrate 115200 bps
  Serial.begin(115200);

  // Memberi waktu agar Serial Monitor siap menerima data
  delay(3000);

  // Menampilkan judul program di Serial Monitor
  Serial.print("=== TEST NRF24L01 PADA ESP32 ===");
  
  // Pastikan semua data Serial selesai dikirim
  Serial.flush();

  // Inisialisasi SPI secara manual untuk ESP32
  // Format: SPI.begin(SCK, MISO, MOSI, SS)
  // SCK  = GPIO 18
  // MISO = GPIO 19
  // MOSI = GPIO 23
  // SS   = GPIO 5 (CSN)
  SPI.begin(18, 19, 23, 5);
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