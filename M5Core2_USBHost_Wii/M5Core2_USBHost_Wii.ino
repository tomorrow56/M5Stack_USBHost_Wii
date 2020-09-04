#include "src\usb\usbhub.h"
#include "src\usb\Wii.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include <M5Core2.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
WII Wii(&Btd, PAIR); // This will start an inquiry and then pair with your Wiimote - you only have to do this once
//WII Wii(&Btd); // After that you can simply create the instance like so and then press any button on the Wiimote

bool printAngle;

void setup() {
/*
 * M5.begin(SerialEnable, I2CEnable, DisplayEnable);
 */
  M5.begin(true, true, true);
  delay(50);
  Serial.println("USB Host test");

  M5.Lcd.setCursor(70, 100);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("M5USB_Wii");
  delay(1000);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.fillScreen(BLACK);

  #if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    M5.Lcd.print(F("\r\nOSC did not start"));
    while(1); //halt
  }
  Serial.print(F("\r\nWiimote Bluetooth Library Started"));

  M5.Lcd.setTextColor(RED);
  M5.Lcd.print(F("Wiimote Bluetooth Library Started\r\n"));
  delay(500);
  M5.Lcd.print(F("Press 1 & 2 on the Wiimote\r\n"));
}

int ROW = 0;
int SkipCount = 0;
int SkipCountNun = 0;
int WiiPitch;
int WiiRoll;
int NunchuckPitch;
int NunchuckRoll;
int AnalogHatX;
int AnalogHatY;
boolean WiiRemoteConnected = false;

void loop() {
  Usb.Task();

  if (Wii.wiimoteConnected) {
    if(WiiRemoteConnected == false){
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.print(F("WiiRemoteConnected\r\n"));
      ROW++;
      WiiRemoteConnected = true;
      M5.Lcd.setTextColor(WHITE);
    }
    if (Wii.getButtonClick(HOME)) { // You can use getButtonPress to see if the button is held down
      Serial.print(F("\r\nHOME"));
      M5.Lcd.print(F("HOME\r\n"));
      ROW++;
      M5.Lcd.print(F("Wii disconnect\r\n"));
      WiiRemoteConnected = false;
      ROW++;
      Wii.disconnect();
    }
    else {
      if (Wii.getButtonClick(LEFT)) {
        Wii.setLedOff();
        Wii.setLedOn(LED1);
        Serial.print(F("\r\nLeft"));
        M5.Lcd.print(F("Left\r\n"));
        ROW++;
      }
      if (Wii.getButtonClick(RIGHT)) {
        Wii.setLedOff();
        Wii.setLedOn(LED3);
        Serial.print(F("\r\nRight"));
        M5.Lcd.print(F("Right\r\n"));
        ROW++;
    }
      if (Wii.getButtonClick(DOWN)) {
        Wii.setLedOff();
        Wii.setLedOn(LED4);
        Serial.print(F("\r\nDown"));
        M5.Lcd.print(F("Down\r\n"));
        ROW++;
      }
      if (Wii.getButtonClick(UP)) {
        Wii.setLedOff();
        Wii.setLedOn(LED2);
        Serial.print(F("\r\nUp"));
        M5.Lcd.print(F("Up\r\n"));
        ROW++;
      }

      if (Wii.getButtonClick(PLUS)) {
        Serial.print(F("\r\nPlus"));
        M5.Lcd.print(F("Plus\r\n"));
        ROW++;
      }
      if (Wii.getButtonClick(MINUS)) {
        Serial.print(F("\r\nMinus"));
        M5.Lcd.print(F("Minus\r\n"));
        ROW++;
      }

      if (Wii.getButtonClick(ONE)) {
        Serial.print(F("\r\nOne"));
        M5.Lcd.print(F("One\r\n"));
        ROW++;
      }
      if (Wii.getButtonClick(TWO)) {
        Serial.print(F("\r\nTwo"));
        M5.Lcd.print(F("Two\r\n"));
        ROW++;
      }

      if (Wii.getButtonClick(A)) {
        printAngle = !printAngle;
        Serial.print(F("\r\nA"));
        M5.Lcd.print(F("A\r\n"));
        ROW++;
     }
      if (Wii.getButtonClick(B)) {
        Wii.setRumbleToggle();
        Serial.print(F("\r\nB"));
        M5.Lcd.print(F("B\r\n"));
        ROW++;
     }
    }
#if 1 // Set this to 1 in order to see the angle of the controllers
    if (printAngle) {
      SkipCount++;
      WiiPitch = Wii.getPitch();
      WiiRoll = Wii.getRoll();
      
      if (Wii.nunchuckConnected) {
        NunchuckPitch = Wii.getNunchuckPitch();
        NunchuckRoll = Wii.getNunchuckRoll();
      }
      if(SkipCount > 3000){
        Serial.print(F("\r\nPitch: "));
        Serial.print(WiiPitch);
        Serial.print(F("\tRoll: "));
        Serial.print(WiiRoll);
        
        M5.Lcd.print(F("Pitch: "));
        M5.Lcd.print(WiiPitch);
        M5.Lcd.print(F(" Roll: "));
        M5.Lcd.print(WiiRoll);
        if (Wii.nunchuckConnected) {
          Serial.print(F("\tNunchuck Pitch: "));
          Serial.print(NunchuckPitch);
          Serial.print(F("\tNunchuck Roll: "));
          Serial.print(NunchuckRoll);
          
          M5.Lcd.print(F(" Nun Pitch: "));
          M5.Lcd.print(NunchuckPitch);
          M5.Lcd.print(F(" Nun Roll: "));
          M5.Lcd.print(NunchuckRoll);
        }
        M5.Lcd.print(F("\r\n"));
        ROW++;
        SkipCount = 0;
      }
    }
#endif
  }
#if 1 // Set this to 1 if you are using a Nunchuck controller
  if (Wii.nunchuckConnected) {
    if (Wii.getButtonClick(Z)) {
      Serial.print(F("\r\nZ"));
      M5.Lcd.print(F("Z\r\n"));
      ROW++;
    }
    if (Wii.getButtonClick(C)) {
      Serial.print(F("\r\nC"));
      M5.Lcd.print(F("C\r\n"));
      ROW++;
    }
    if (Wii.getAnalogHat(HatX) > 137 ||  Wii.getAnalogHat(HatX) < 117 || Wii.getAnalogHat(HatY) > 137 || Wii.getAnalogHat(HatY) < 117) {
      SkipCountNun++;
      AnalogHatX = Wii.getAnalogHat(HatX);
      AnalogHatY = Wii.getAnalogHat(HatY);

      if(SkipCountNun > 5000){
        Serial.print(F("\r\nHatX: "));
        Serial.print(AnalogHatX);
        Serial.print(F("\tHatY: "));
        Serial.print(AnalogHatY);
        
        M5.Lcd.print(F("HatX: "));
        M5.Lcd.print(AnalogHatX);
        M5.Lcd.print(F(" HatY: "));
        M5.Lcd.print(AnalogHatY);
        M5.Lcd.print(F("\r\n"));
        ROW++;
        SkipCountNun = 0;
      }
    }
  }
#endif
  if(ROW >28){
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.fillScreen(BLACK);
    ROW = 0;
  }
}
