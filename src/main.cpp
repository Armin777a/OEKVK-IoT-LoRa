#include <Arduino.h>
#include <Sodaq_RN2483.h>



// ==============================================================================
// LoRaWAN LED Blink Example
// ==============================================================================

// End-device Address:          12345678
// EUI (optional):              0004A30B0023876D
// Network Session Key:         12345678912345678912345678912345
// Application Session Key:     12345678912345678912345678912345





// ==============================================================================
// Defines
// ==============================================================================

#if defined(ARDUINO_AVR_SODAQ_MBILI) || defined(ARDUINO_AVR_SODAQ_TATU)

// MBili
#define debugSerial Serial
#define loraSerial Serial1
#define beePin 20
#elif defined(ARDUINO_SODAQ_AUTONOMO)

// Autonomo
#define debugSerial SerialUSB
#define loraSerial Serial1
#define beePin BEE_VCC
#elif defined(ARDUINO_SODAQ_ONE) || defined(ARDUINO_SODAQ_ONE_BETA)

// Sodaq One
#define debugSerial SerialUSB
#define loraSerial Serial1
#elif defined(ARDUINO_SODAQ_EXPLORER)
#define debugSerial SerialUSB
#define loraSerial Serial2

#else
#error "Please select Autonomo, Mbili, or Tatu"

#endif





// ==============================================================================
// Function prototypes
// ==============================================================================

void sendData(int data);




// ==============================================================================
// Global Variables
// ==============================================================================

// For the nonblocking delay
unsigned long currentMillis, previousMillis;                                  // variables used to implement blink interval
const long interval = 20000;                                                  // interval (in mS) at which to toggle LED_BUILTIN

// For the leds
int ledState = LOW;  

// For the timer
int timerClock = 0;
int isTimerRunning = 0;







// ==============================================================================
// Main Program
// ==============================================================================

void setup() {
    String a;

    #ifdef beePin
        digitalWrite(beePin, HIGH);
        pinMode(beePin, OUTPUT);
    #endif


    // Initialize the pins
    pinMode(LED_BUILTIN, OUTPUT);                                               // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_GREEN, OUTPUT);                                                 // initialize digital pin LED_GREEN as an output.
    pinMode(TEMP_SENSOR, INPUT);                                                // temperature sensor pin initialization
    analogReadResolution(12);

    // Initialize the serial ports
    debugSerial.begin(115200);
    loraSerial.begin(LoRaBee.getDefaultBaudRate());

    delay(5000);

    // Initialize LoRaWAN
    loraSerial.println("mac join abp");
    debugSerial.println("mac join abp");
    a = loraSerial.readString();
    debugSerial.println(a);

    while (a != "ok\r\naccepted\r\n") {
        debugSerial.println("mac join abp");
        a = loraSerial.readString();
        debugSerial.println(a);
        delay(1000);
    }

    debugSerial.println("csatlakozva");
    delay(10000);
    debugSerial.println("Kuldes megkezdve");
}


void loop() {


    currentMillis = millis(); // get the current time

    // is it time to blink the LED?
    if (currentMillis - previousMillis >= interval) { 
        previousMillis = currentMillis; // yes - save the last time we blinked the LED

        if (ledState == LOW) {
            ledState = HIGH;
        } else {
            ledState = LOW;
        }
        digitalWrite(LED_BUILTIN, ledState); // apply the new LED state



        sendData(isTimerRunning);
    }

    while (loraSerial.available()) {
        debugSerial.write(loraSerial.read());        // relay responses to terminal from RN2903
    }
}



// ==============================================================================
// Functions
// ==============================================================================

void sendData(int data) {
    String commandToSend = "mac tx uncnf 6 ";

    commandToSend += String(data, HEX);

    loraSerial.println(commandToSend);
    debugSerial.println(commandToSend);
    debugSerial.println(loraSerial.readString());
}



