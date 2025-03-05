#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>
#include "BluetoothSerial.h"

bool gameRunning = false;  // Flag to track if a game is running


// VGA Pin Definitions
const int redPins[] = {2, 4, 12, 13, 14};
const int greenPins[] = {15, 16, 17, 18, 19};
const int bluePins[] = {21, 22, 23, 27};
const int hsyncPin = 32;
const int vsyncPin = 33;

// Button Definitions
byte button1 = 26; // Right
byte button2 = 25; // Rotate (not used in menu)
byte button3 = 34; // Left
byte button4 = 35; // downfast for Tetris
byte button5 = 21; // select
byte gameArea = 0; // select


uint8_t address[6] = {0x00, 0x14, 0x03, 0x05, 0x0C, 0x4F};  // Change this to reflect real MAC address of your slave BT device
//uint8_t address[6] = {0x00, 0x14, 0x03, 0x05, 0x0C, 0x4F};  // Change this to reflect real MAC address of your slave BT device
//uint8_t address[6] = {0x00, 0x14, 0x03, 0x05, 0x0B, 0x7F};  // Change this to reflect real MAC address of your slave BT device
//uint8_t address[6] = {0x00, 0x14, 0x03, 0x05, 0x0C, 0x10};  // Change this to reflect real MAC address of your slave BT device
//uint8_t address[6] = {0x00, 0x14, 0x03, 0x05, 0x0C, 0x10};  // Change this to reflect real MAC address of your slave BT device
//uint8_t address[6] = {0x00, 0x21, 0x07, 0x34, 0xD3, 0x11};  // Change this to reflect real MAC address of your slave BT device

// VGA Device
VGA3Bit vga;

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif
BluetoothSerial SerialBT;

String myName = "ESP32-BT-Master";

// External function declarations (menu)
extern void menuLoop();

void readController(){
  if (SerialBT.available()) {
        uint8_t inVal = SerialBT.read();  // Read incoming data as string
        
        if(~(inVal) & 0x01){ // Button 1 (Select)
          switch (gameArea){
            case 0:
              menuSelect();
              break;
            case 1:
              tetrisButton1();
              break;
            case 2:
              pongButton1();
              break;
            case 3:
              mineButton1();
              break;
          }
        }

        if(~(inVal) & 0x02){ // Button 2 (Exit)
          menuBack();
          delay(200);  // Debounce to prevent double press
        }

        if(~(inVal) & 0x20){ // Left
          switch (gameArea){
            case 0:
              break;
            case 1:
              tetrisLeft();
              break;
            case 2:
              break;
            case 3:
              mineLeft();
              break;
          }
        }

        if(~(inVal) & 0x10){ // Right
          switch (gameArea){
            case 0:
              break;
            case 1:
              tetrisRight();
              break;
            case 2:
              break;
            case 3:
              mineRight();
              break;
          }
        }
        
        if(~(inVal) & 0x08){ // Up
          switch (gameArea){
            case 0:
              menuUp();
              break;
            case 1:
              tetrisUp();
              break;
            case 2:
              pongUp();
              break;
            case 3:
              mineUp();
              break;
          }
        }

        if(~(inVal) & 0x04){ // Down
          switch (gameArea){
            case 0:
              menuDown();
              break;
            case 1:
              tetrisDown();
              break;
             case 2:
              pongDown();
              break;
            case 3:
              mineDown();
              break;
          }
        }
      }
}

void stopGame() {
    switch (gameArea) {
        case 1:
            tetrisButton2();
            break;
        case 2:
            pongButton2();
            break;
        case 3:
            mineButton2();
            break;
    }
}


void setup() {
    // Initialize VGA with correct pin mapping
    vga.setFrameBufferCount(1);
    vga.init(vga.MODE320x200, 14, 19, 27, hsyncPin, vsyncPin);
    vga.setFont(Font6x8);  // Set font for text display
    vga.setFont(Font6x8);
    randomSeed(analogRead(5)); 
    pinMode(button1,INPUT);
    pinMode(button2,INPUT);
    pinMode(button3,INPUT);
    pinMode(button4,INPUT);
    pinMode(button5,INPUT);

    //Bluetooth Setup:
    bool connected;
    Serial.begin(9600);
    SerialBT.begin(myName, true);
    connected = SerialBT.connect(address);

    // Start the menu
    menuLoop();
}

void loop() {
    // Empty, as the menuLoop() handles everything
}