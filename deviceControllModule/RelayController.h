#ifndef RELAYCONTROLLER_H
#define RELAYCONTROLLER_H

#include <Arduino.h>

class RelayController {
public:
    // Constructor with parameters to initialize relay pins and count
    RelayController(int* _relayPin, const int _RELAY_COUNT) 
        :  RELAY_COUNT(_RELAY_COUNT),relayPin(_relayPin) {
        initRelay(); // Initialize the relays during construction
    }

    // Default constructor
    RelayController() : relayPin(nullptr), RELAY_COUNT(0) {
    }

    // Method to set the relay pin array
    void setRelayPin(int* _relayPin) {
        relayPin = _relayPin;
    }

    // Method to control relay state
    void relayWrite(int id, bool state) {
        if (id >= 0 && id < RELAY_COUNT) { // Ensure 'id' is within valid range
            digitalWrite(relayPin[id], !state);
            states[id] = state;
        } else {
            //Serial.println("Invalid relay ID!");
        }
    }

    void relayWriteAll() {
        for(int i = 0; i < RELAY_COUNT; i++){
          digitalWrite(relayPin[i], !(states[i]));
        }
    }

    // Method to test each relay
    void testRelay() {
        for (int i = 0; i < RELAY_COUNT; i++) {
            relayWrite(i, HIGH);
            delay(300);
            relayWrite(i, LOW);
            delay(100);
        }
    }

    // Method to log the relay configuration
    void Log() {
        Serial.print("Relay count: ");
        Serial.println(RELAY_COUNT);

        Serial.print("Relay pins: ");
        for (int i = 0; i < RELAY_COUNT; i++) {
            Serial.print(relayPin[i]);
            Serial.print(" ");
        }
        Serial.println();
    }

    int getStates(){
      int st = 0;
      for(int i = 0 ; i<RELAY_COUNT;i++){
        st<<=1;
        st|= states[i];
      }
      return st;
    }

private:
    // Method to initialize relay pins
    void initRelay() {
        if (relayPin != nullptr) {
            for (int i = 0; i < RELAY_COUNT; i++) {
                pinMode(relayPin[i], OUTPUT);
                relayWrite(i, LOW); // Set relays to LOW initially
                states[i] = false;
            }
        } else {
            //Serial.println("Relay pin array not set!");
        }
    }

    
    int* relayPin;       // Pointer to the array of relay pins
    const int RELAY_COUNT;     // Number of relays connected
    bool states[16];
};

#endif // RELAYCONTROLLER_H
