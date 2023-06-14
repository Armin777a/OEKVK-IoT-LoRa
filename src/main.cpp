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
// Main Program
// ==============================================================================

void setup() {
    String a;

    #ifdef beePin
        digitalWrite(beePin, HIGH);
        pinMode(beePin, OUTPUT);
    #endif

    pinMode(LED_BUILTIN, OUTPUT);

    debugSerial.begin(57600);
    loraSerial.begin(LoRaBee.getDefaultBaudRate());

    delay(5000);

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
    loraSerial.println("mac tx uncnf 6 AABBCC");
    debugSerial.println("mac tx uncnf 6 AABBCC");
    debugSerial.println(loraSerial.readString());

    delay(10000);
}