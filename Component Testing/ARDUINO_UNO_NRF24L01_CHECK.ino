#include <SPI.h>          // Library for SPI (Serial Peripheral Interface) communication
#include <nRF24L01.h>     // Definitions of NRF24L01-specific registers and constants
#include <RF24.h>         // Main library for controlling the NRF24L01 module


/* 
  Arduino Uno + NRF24L01 module with NRF24L01 adapter

  Arduino Uno SPI connections to NRF24L01 adapter:
  -----------------------------------------

  NRF24L01    Arduino Uno
  --------    -----------
  VCC         5V                  // Power supply: 5V
  GND         GND                 // Ground
  CE          D7                  // Chip Enable (activates TX/RX mode)
  CSN (CS)    D8                  // Chip Select for SPI communication
  SCK         D13                 // Clock SPI
  MOSI        D11                 // Master Out Slave In
  MISO        D12                 // Master In Slave Out
  IRQ         Not used

  Notes:
  - The NRF24L01 module must use 3.3V (do not connect directly to 5V).
  - For NRF24L01 PA+LNA modules (with external antenna), it is recommended to:
    1. Add a 10uF–100uF capacitor between VCC and GND, or
    2. Use an NRF24L01 adapter with a 3.3V regulator.
*/

// Create a radio object with the following configuration:
// CE  -> Pin D7
// CSN -> Pin D8
RF24 radio(7, 8);


// Function to send one character to Serial
int serial_putc(char c, FILE *) {
  Serial.write(c);
  return c;
}


// Function to enable printf() output to the Serial Monitor
void printf_begin(void) {
  static FILE serial_stdout;
  fdev_setup_stream(&serial_stdout, serial_putc, NULL, _FDEV_SETUP_WRITE);
  stdout = &serial_stdout;
}


// Function to initialize the system and required peripherals, executed once at startup
void setup() {

  // Start Serial Monitor communication at 115200 baud
  Serial.begin(115200);

  // Give the Serial Monitor time to initialize
  delay(3000);

  // Display the program title in the Serial Monitor
  Serial.print("=== TEST NRF24L01 PADA ARDUINO UNO ===");

  // Enable printf() support for the Serial Monitor
  printf_begin();
  
  // Make sure all Serial data has been transmitted
  Serial.flush();

  // Initialize SPI
  // On Arduino Uno, simply use SPI.begin()
  // because the SPI pins are fixed:
  // MOSI = D11, MISO = D12, SCK = D13
  SPI.begin();
}


// Function to repeatedly execute the main program logic
void loop() {

  // Attempt to initialize the NRF24L01 module
  // radio.begin() returns:
  // - true  -> if the module is successfully detected
  // - false -> if the module is not found or communication fails
  if (!radio.begin()) {

    // If the module is not detected, display an error message
    Serial.println("\n\n❌ NRF24L01 TIDAK TERDETEKSI");

    // Display possible causes of failure
    Serial.println("Kemungkinan:");
    Serial.println("- Wiring salah");
    Serial.println("- Adapter NRF24L01 rusak");
    Serial.println("- Modul NRF24L01 rusak");
    Serial.println("- Suplai 3.3V tidak stabil");

    // Stop the program here (infinite loop)
    // The MCU will halt and not continue execution
    while (1);
  }

  // If the module is successfully detected
  Serial.println("\n\n✅ NRF24L01 TERDETEKSI");
  Serial.println("Modul kondisinya masih baik.");
  Serial.println();

  // Display detailed module information in the Serial Monitor, such as:
  // - Configuration registers
  // - Channel
  // - Data rate
  // - PA level
  // - CRC
  // - Pipe addresses
  radio.printDetails();

  // Wait 2 seconds before checking again
  delay(2000);
}
