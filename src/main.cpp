#include <Arduino.h>

// ==============================================================================
// LoRaWAN Terminal Application for the Microchip RN2903 LoRa Technology Module
// ==============================================================================


// End-device Address:          12345678
// EUI (optional):              0004A30B0023876D
// Network Session Key:         12345678912345678912345678912345
// Application Session Key:     12345678912345678912345678912345



// =============================================================================
// Defines
// =============================================================================

#define loraSerial Serial2                                                    // Serial interface used to communicate with RN2903 
#define debugSerial SerialUSB                                                 // Serial interface used for debugging


// =============================================================================
// Function prototypes
// =============================================================================
bool rn2903HardwareReset(void);                                               // places the module in a known state



// =============================================================================
// Global variables
// =============================================================================
unsigned long currentMillis, previousMillis;                                  // variables used to implement blink interval
const long interval = 250;                                                    // interval (in mS) at which to toggle LED_BUILTIN
int ledState = LOW;                                                           // used to set the LED value





// =============================================================================
// Setup and main function
// =============================================================================

void setup() {
    while (!debugSerial);                                                       // wait for a serial terminal application to connect to the board

    debugSerial.begin(115200);                                                  // initialize the debug serial port
    loraSerial.begin(57600);                                                    // initialize the RN2903 serial port

    debugSerial.println("Microchip Technology ExpLoRer Starter Kit");           // start-up messages
    debugSerial.println("2023 LoRa Tanfolyam");
    debugSerial.println("Lab 1: Commands\n");
    debugSerial.print("Resetting the RN2903 Module...");                        // Hardware-Reset the LoRa module to place in a known state
    
    if (rn2903HardwareReset()) {
        debugSerial.println("success.\n");
        
    }
}

void loop() {

    while (debugSerial.available()) {
        loraSerial.write(debugSerial.read());                                     // relay commands to RN2903 from terminal
    }

    while (loraSerial.available()) {
        debugSerial.write(loraSerial.read());                                     // relay responses to terminal from RN2903
        
    }
}




// =============================================================================
// Local functions
// =============================================================================

bool rn2903HardwareReset(void) {
  char readChar;
  unsigned long currentTime, triggerTime;

  pinMode(LORA_RESET, OUTPUT);                     // Reset the LoRa module via hardware reset pin and read out the status line within the first second
  digitalWrite(LORA_RESET, LOW);
  delay(150);
  digitalWrite(LORA_RESET, HIGH);
  delay(150);

  currentTime = millis();
  triggerTime = currentTime + 1000;
  while ((readChar = loraSerial.read()) != 0x0A) {
    currentTime = millis();
    if (currentTime >= triggerTime) { 
      return false;
    }
  }
  return true;
}