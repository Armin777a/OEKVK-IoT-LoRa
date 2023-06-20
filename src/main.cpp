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

float getTemperature(void);
void sendFloatTemperatureLora(float temperture);
uint16_t convertFloatToDecimal(float value);




// ==============================================================================
// Global Variables
// ==============================================================================

// For the nonblocking delay
unsigned long currentMillis, previousMillis;                                  // variables used to implement blink interval
const long interval = 10000;                                                  // interval (in mS) at which to toggle LED_BUILTIN

// For the leds
int ledState = LOW;  

// For the temperature sensor
float temperature = 0;







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

    loraSerial.println("mac pause");
    delay(10000);
    loraSerial.println("mac set pwr 14");
    delay(10000);
}


void loop() {

    currentMillis = millis(); // get the current time

    // is it time to blink the LED?
    if (currentMillis - previousMillis >= interval) { 
        previousMillis = currentMillis; // yes - save the last time we blinked the LED

        if (ledState == LOW) {
            ledState = HIGH;
            
            temperature = getTemperature();
            //sendFloatTemperatureLora(temperature);
        } else {
            ledState = LOW;
        }

        digitalWrite(LED_BUILTIN, ledState); // apply the new LED state
    }
}



// ==============================================================================
// Functions
// ==============================================================================

float getTemperature(void) {
    // 10mV per C, 0C is 500mV
    float mVolts = (float)analogRead(TEMP_SENSOR) * 3300.0 / 4096.0; // Calculate voltage value 3300mV (2^12)=4096
    float temp = (mVolts - 500.0) / 10.0;                            // Gives value to 0.1degC - calculate temperature value

    return temp;
}

void sendFloatTemperatureLora(float temperture) {
    String commandToSend = "radio tx ";

    commandToSend += String(convertFloatToDecimal(temperture));
    commandToSend += "C0";

    loraSerial.println(commandToSend);
    debugSerial.println(commandToSend);
    debugSerial.println(loraSerial.readString());
}

uint16_t convertFloatToDecimal(float value) {
    return value * 100;
}