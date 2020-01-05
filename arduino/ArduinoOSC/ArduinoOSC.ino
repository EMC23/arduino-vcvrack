#include <SLIPEncodedSerial.h>
#include <OSCData.h>
#include <OSCBundle.h>
#include <OSCBoards.h>
#include <OSCTiming.h>
#include <OSCMessage.h>
#include <OSCMatch.h>
#include <SLIPEncodedUSBSerial.h>

/**
 * Using OSC library: https://github.com/CNMAT/OSC
 * Open Sound Control: http://opensoundcontrol.org
 * Clearer example: https://github.com/CNMAT/OSC/issues/29
 */

SLIPEncodedSerial SLIPSerial(Serial);
int testValue = 1337;

// basic timer
unsigned long timer = 0;             // will store last time  updated
unsigned long timerInterval = 1000;  // interval at which to time (milliseconds)

void setup() {
  SLIPSerial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // check for new messages as fast as possible
  receiveOSC();

  unsigned long currentMillis = millis();
  if (currentMillis - timer > timerInterval) {
    timer = currentMillis;
    sendOSC();
  }
}

void receiveOSC() {
  OSCMessage messageIN;
  int size;

  // receive a bundle
  if (SLIPSerial.available()) {
    while (!SLIPSerial.endofPacket()) {
      size = SLIPSerial.available();
      if (size > 0) {
        while (size--) {
          messageIN.fill(SLIPSerial.read());
        }
        if (!messageIN.hasError()) {
          messageIN.dispatch("/test/in", action);
        }
      }
    }
  }
}

void action(OSCMessage &msgIn) {
  testValue = msgIn.getInt(0);
}

void sendOSC() {
  OSCMessage msg("/test/out");
  msg.add(testValue);
  SLIPSerial.beginPacket();
  msg.send(SLIPSerial); // send the bytes to the SLIP stream
  SLIPSerial.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message
}
