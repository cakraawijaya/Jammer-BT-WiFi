#include <SPI.h>          // Library for SPI (Serial Peripheral Interface) communication
#include <nRF24L01.h>     // Definitions of NRF24L01-specific registers and constants
#include <RF24.h>         // Main library for controlling the NRF24L01 module


/* 
  ESP32 + NRF24L01 module with NRF24L01 adapter

  ESP32 SPI connections to NRF24L01 adapter:
  --------------------------------

  NRF24L01    ESP32
  --------    -----
  VCC         3.3V                // Power supply: 3.3V
  GND         GND                 // Ground
  CE          GPIO 4              // Chip Enable (activates TX/RX mode)
  CSN (CS)    GPIO 5              // Chip Select for SPI communication
  SCK         GPIO 18             // Clock SPI
  MOSI        GPIO 23             // Master Out Slave In
  MISO        GPIO 19             // Master In Slave Out
  IRQ         Not used

  Notes:
  - The NRF24L01 module must use 3.3V (do not connect directly to 5V).
  - For NRF24L01 PA+LNA modules (with external antenna), it is recommended to:
    1. Add a 10uF–100uF capacitor between VCC and GND, or
    2. Use an NRF24L01 adapter with a 3.3V regulator.
*/

// Create a radio object with the following configuration:
// CE  -> GPIO 4
// CSN -> GPIO 5
RF24 radio(4, 5);


// Function to initialize the system and required peripherals, executed once at startup
void setup() {

  // Start Serial Monitor communication at 115200 baud
  Serial.begin(115200);

  // Give the Serial Monitor time to initialize
  delay(3000);

  // Display the program title in the Serial Monitor
  Serial.print("=== TEST NRF24L01 PADA ESP32 ===");
  
  // Make sure all Serial data has been transmitted
  Serial.flush();

  // Manually initialize SPI for the ESP32
  // Format: SPI.begin(SCK, MISO, MOSI, SS)
  // SCK  = GPIO 18
  // MISO = GPIO 19
  // MOSI = GPIO 23
  // SS   = GPIO 5 (CSN)
  SPI.begin(18, 19, 23, 5);
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
