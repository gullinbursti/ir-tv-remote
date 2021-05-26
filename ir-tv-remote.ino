
#define MARK_EXCESS_MICROS    20 // recommended for the cheap VS1838 modules

#include <Arduino.h>
#include <IRremote.h>
#include "PinDefinitionsAndMore.h"


#define BTN_PIN 5

//uint16_t sAddress = 0x707; // SAMSUNG
uint16_t sAddress = 0x0; // LIVING RM
uint8_t sRepeats = 0;
  
// pwr btn
//uint8_t sCommand = 0x2; // SAMSUNG
uint8_t sCommand = 0xC; // LIVING RM

// mute btn 
//uint8_t sCommand = 0xF; // SAMSUNG

// src btn
//uint8_t sCommand = 0x38; // LIVING RM

// vol up
//uint8_t sCommand = 0x10; // LIVING RM

// vol dn
//uint8_t sCommand = 0x11; // LIVING RM
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);

  digitalWrite(BTN_PIN, LOW);

  Serial.begin(115200);
  delay(4000);
  
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(IR_RECEIVE_PIN);
  
  IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK);
  Serial.print(F("Ready to send IR signals at pin "));
  Serial.println(IR_SEND_PIN);
}


void loop() {
//  Serial.println(digitalRead(BTN_PIN));
  
  if (digitalRead(BTN_PIN) == HIGH) {
    Serial.print(F("Sending... "));  
    IRData IRSendData;
    IRSendData.address = sAddress;
    IRSendData.command = sCommand;
    IRSendData.flags = IRDATA_FLAGS_EMPTY;
  
//    IRSendData.protocol = SAMSUNG;
    IRSendData.protocol = RC6;
    Serial.print(F("Sending... "));
    Serial.print(F("ADDR=0x"));
    Serial.print(sAddress, HEX);
    Serial.print(F(" CMD=0x"));
    Serial.print(sCommand, HEX);
    Serial.print(F(" via "));
    Serial.println(getProtocolString(IRSendData.protocol));
    IrSender.write(&IRSendData, sRepeats);
    delay(333);
  }
  
  if (IrReceiver.decode()) {  // Grab an IR code
    
    // Check if the buffer overflowed
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
      Serial.println("IR code too long. Edit IRremoteInt.h and increase RAW_BUFFER_LENGTH");
      
    } else {
      Serial.print("=-= PROTOCOL: ");
      Serial.print(getProtocolString(IrReceiver.decodedIRData.protocol));
      Serial.print(" =-= ADDR: 0x");
      Serial.print(IrReceiver.decodedIRData.address, HEX);
      Serial.print(" =-= CMD: 0x");
      Serial.print(IrReceiver.decodedIRData.command, HEX);
      Serial.print(" =-= RAW: 0x");
      Serial.print(IrReceiver.decodedIRData.decodedRawData, HEX);
      Serial.print(" =-= ");           
      Serial.println();
      IrReceiver.printIRResultShort(&Serial);
      Serial.println();
      Serial.println();
    }
    
    IrReceiver.resume();
  }
}
