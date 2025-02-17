#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>

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

// VGA Device
VGA3Bit vga;

// External function declarations (menu)
extern void menuLoop();

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

    // Start the menu
    menuLoop();
}

void loop() {
    // Empty, as the menuLoop() handles everything
}
