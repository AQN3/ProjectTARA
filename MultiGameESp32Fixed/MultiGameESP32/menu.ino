#include <ESP32Lib.h>

// Menu Variables
int selectedOption = 0;
const int totalOptions = 3;
const char* menuOptions[] = {"Tetris", "Pong", "Minesweeper"};

// Game Mode Variables
bool isMultiplayer = false;  // Tracks whether Multiplayer or Singleplayer is selected
bool gameSelected = false;
int selectedMode = 0;
const int totalModes = 2;
const char* modeOptions[] = {"Singleplayer", "Multiplayer"};

// Function declarations
extern void runTetris(bool multiplayer);
extern void runPong(bool multiplayer);
extern void runMinesweeper();

void drawMenu() {
    vga.clear(0);
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0));

    for (int i = 0; i < totalOptions; i++) {
        vga.setCursor(100, 60 + i * 20);
        if (i == selectedOption) {
            vga.setTextColor(vga.RGB(0, 255, 0), vga.RGB(0, 0, 0)); // Highlight selection
        } else {
            vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0));
        }
        vga.print(menuOptions[i]);
    }
}

void drawModeMenu() {
    vga.clear(0);
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0));

    for (int i = 0; i < totalModes; i++) {
        vga.setCursor(80, 80 + i * 20);
        if (i == selectedMode) {
            vga.setTextColor(vga.RGB(0, 255, 0), vga.RGB(0, 0, 0)); // Highlight selection
        } else {
            vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0));
        }
        vga.print(modeOptions[i]);
    }
}



void menuDown(){
  if (!gameSelected) {  // If we're in the main menu
    selectedOption = (selectedOption + 1) % totalOptions;
    drawMenu();
  } else {  // If we're selecting Singleplayer or Multiplayer
    selectedMode = (selectedMode + 1) % totalModes;
    drawModeMenu();
  }
  Serial.println(selectedOption);
}

void menuUp(){
  if (!gameSelected) {  // If we're in the main menu
    selectedOption = (selectedOption - 1 + totalOptions) % totalOptions;
    drawMenu();
  } else {  // If we're selecting Singleplayer or Multiplayer
    selectedMode = (selectedMode - 1 + totalModes) % totalModes;
    drawModeMenu();
  }
}


void menuBack() {
    if (gameRunning) {  
        gameRunning = false;  // Stop game logic
        stopGame();  // Ensure game loop fully stops
    }
    
    gameArea = 0;  // Reset game selection
    vga.clear(0);  // Clear screen
    drawMenu();  // Return to menu
}



void menuSelect() {
    vga.clear(0);
    
    if (!gameSelected) {  // Step 1: Select Game, Go to Mode Selection
        drawModeMenu();
        gameSelected = true;  // Now in mode selection
    } else {  // Step 2: Select Mode, Start Game
        Serial.println("Game Starting...");
        isMultiplayer = (selectedMode == 1);
        gameRunning = true;  // Game officially starts

        switch (selectedOption) {
            case 0:
                gameArea = 1;
                runTetris(isMultiplayer);
                break;
            case 1:
                gameArea = 2;
                runPong(isMultiplayer);
                break;
            case 2:
                gameArea = 3;
                runMinesweeper(isMultiplayer);
                break;
        }
        
        gameSelected = false;  // Reset for next menu interaction
    }
}



void menuLoop() {
    drawMenu();

    while (true) {
        readController();
        //processInputs();
        /*
        if (digitalRead(button2) == HIGH) {  // Move Up
            selectedOption = (selectedOption - 1 + totalOptions) % totalOptions;
            drawMenu();
            delay(150);
        }
        if (digitalRead(button4) == HIGH) {  // Move Down
            selectedOption = (selectedOption + 1) % totalOptions;
            drawMenu();
            delay(150);
        }
        if (digitalRead(button5) == HIGH) {  // Select Game
            vga.clear(0);
            delay(100);
        */
          /*
            if (selectedOption == 0 || selectedOption == 1) {  // **Tetris or Pong Selected**
                drawModeMenu();  // **Open Game Mode Selection**
                while (true) {  // Loop for mode selection
                    processInputs();
                    if (digitalRead(button2) == HIGH) {  // Move Up
                        selectedMode = (selectedMode - 1 + totalModes) % totalModes;
                        drawModeMenu();
                        delay(150);
                    }
                    if (digitalRead(button4) == HIGH) {  // Move Down
                        selectedMode = (selectedMode + 1) % totalModes;
                        drawModeMenu();
                        delay(150);
                    }
                    if (digitalRead(button5) == HIGH) {  // Confirm Mode
                        isMultiplayer = (selectedMode == 1);  // Set multiplayer flag
                        vga.clear(0);
                        delay(100);
                        if (selectedOption == 0) {
                            runTetris(isMultiplayer);  // Pass mode to Tetris
                        } else {
                            runPong(isMultiplayer);  // Pass mode to Pong
                        }
                        return;
                    }
                }
            }
            //if (selectedOption == 2) runMinesweeper();
            */
            //vga.clear(0);
            //delay(100);
            //drawMenu();
      }   
}