#include <ESP32Lib.h>

// VGA Pins
//const int redPin = 14, greenPin = 19, bluePin = 27, hsyncPin = 32, vsyncPin = 33;

// Button Pins
//const int btnDown = 22, btnUp = 23, btnLeft = 5, btnRight = 18, btnClear = 34, btnFlag = 35;

//VGA3Bit vga;

const int gridSize = 9;
const int blockSize = 12;
const int numBombs = 10;
const int gridOffsetX = 40;
const int gridOffsetY = 10;
int timer = 999;
unsigned long lastTimeUpdate = 0;

int selectedX = 5, selectedY = 5;
int gameGrid[gridSize][gridSize] = {0};
bool revealed[gridSize][gridSize] = {false};
bool flagged[gridSize][gridSize] = {false};
bool waitToStartMine = true;

// Define the 8 available colors
#define BLACK   vga.RGB(0,0,0)
#define BLUE    vga.RGB(0,0,255)
#define RED     vga.RGB(255,0,0)
#define MAGENTA vga.RGB(255,0,255)
#define GREEN   vga.RGB(0,255,0)
#define CYAN    vga.RGB(0,255,255)
#define YELLOW  vga.RGB(255,255,0)
#define WHITE   vga.RGB(255,255,255)

void placeBombs() {
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            gameGrid[y][x] = 0;
            revealed[y][x] = false;
            flagged[y][x] = false;
        }
    }

    int bombsPlaced = 0;
    while (bombsPlaced < numBombs) {
        int x = random(gridSize), y = random(gridSize);
        if (gameGrid[y][x] != 9) {
            gameGrid[y][x] = 9;
            bombsPlaced++;
        }
    }
    
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            if (gameGrid[y][x] == 9) continue;
            int count = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < gridSize && ny >= 0 && ny < gridSize && gameGrid[ny][nx] == 9) {
                        count++;
                    }
                }
            }
            gameGrid[y][x] = count;
        }
    }
}

void drawTimer() {
    vga.fillRect(160, 10, 50, 20, BLACK); // Clear previous time
    vga.setCursor(165, 15);
    vga.setTextColor(WHITE);
    vga.print(timer);
}

void drawTile(int x, int y) {
    int startX = x * blockSize + gridOffsetX;
    int startY = y * blockSize + gridOffsetY;
    int color = CYAN;
    
    if (revealed[y][x]) {
        color = (gameGrid[y][x] == 9) ? RED : WHITE;
    }
    
    vga.fillRect(startX, startY, blockSize, blockSize, color);
    
    // Draw border using fillRect
    vga.fillRect(startX, startY, blockSize, 1, BLUE);
    vga.fillRect(startX, startY, 1, blockSize, BLUE);
    vga.fillRect(startX + blockSize - 1, startY, 1, blockSize, BLUE);
    vga.fillRect(startX, startY + blockSize - 1, blockSize, 1, BLUE);
    
    if (revealed[y][x] && gameGrid[y][x] > 0 && gameGrid[y][x] < 9) {
        char num[2];
        snprintf(num, sizeof(num), "%d", gameGrid[y][x]);
        vga.setCursor(startX + 4, startY + 3);
        vga.setTextColor(BLUE);
        vga.print(num);
    }
    
    if (flagged[y][x]) {
        vga.fillRect(startX + 5, startY + 5, blockSize - 10, blockSize - 10, RED);
    }
}

void updateSelection(int prevX, int prevY) {
    drawTile(prevX, prevY);
    int startX = selectedX * blockSize + gridOffsetX;
    int startY = selectedY * blockSize + gridOffsetY;
    vga.fillRect(startX, startY, blockSize, 2, GREEN);
    vga.fillRect(startX, startY, 2, blockSize, GREEN);
    vga.fillRect(startX + blockSize - 2, startY, 2, blockSize, GREEN);
    vga.fillRect(startX, startY + blockSize - 2, blockSize, 2, GREEN);
}

void resetGame() {
    placeBombs();
    timer = 999;
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            drawTile(x, y);
        }
    }
    drawGrid();
    updateSelection(selectedX, selectedY);
}

void revealTile(int x, int y) {
    if (!flagged[y][x] && !revealed[y][x]) {
        revealed[y][x] = true;
        drawTile(x, y);
        
        if (gameGrid[y][x] == 9) {
            // Hit a bomb, reset the game
            delay(1000); // Wait for a second to show the bomb
            resetGame();
            return;
        }
        
        if (gameGrid[y][x] == 0) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < gridSize && ny >= 0 && ny < gridSize) {
                        revealTile(nx, ny);
                    }
                }
            }
        }
    }
}

void toggleFlag() {
    if (!revealed[selectedY][selectedX]) {
        flagged[selectedY][selectedX] = !flagged[selectedY][selectedX];
        drawTile(selectedX, selectedY);
        checkWin();
    }
}

void drawGrid() {
    for (int x = 0; x <= gridSize; x++) {
        int lineX = gridOffsetX + x * blockSize;
        vga.fillRect(lineX, gridOffsetY, 1, gridSize * blockSize, BLUE);
    }
    
    for (int y = 0; y <= gridSize; y++) {
        int lineY = gridOffsetY + y * blockSize;
        vga.fillRect(gridOffsetX, lineY, gridSize * blockSize, 1, BLUE);
    }
}

void checkWin() {
    int flaggedBombs = 0;
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            if (flagged[y][x] && gameGrid[y][x] == 9) {
                flaggedBombs++;
            }
        }
    }
    
    if (flaggedBombs == numBombs) {
        vga.fillRect(0, 0, 320, 240, BLACK);
        vga.setCursor(120, 110);
        vga.setTextColor(GREEN);
        vga.print("YOU WIN!");
        delay(10000); // Wait for 10 seconds
        resetGame();
    }
}

void runMinesweeper(bool isMultiplayer) {
    vga.clear(0);
    delay(100);
    gameRunning = true;
    if (isMultiplayer) {
        // **Multiplayer: Wait for P2 Ready Signal*
    } else {
        // **Singleplayer: Wait for Any Button to Start**
        vga.setCursor(90, 100);
        vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0));
        vga.print("Press any button to start...");

        while (waitToStartMine) {
          readController();

        if (!waitToStartMine) {  // **Exit the waiting loop when a button is pressed**
                      break;
                  }
        }
    }

    vga.clear(0);
    delay(100);
    randomSeed(analogRead(0));
    resetGame();

    while (gameRunning) {  
    int prevX = selectedX, prevY = selectedY;
    readController();
    
    if (prevX != selectedX || prevY != selectedY) {
        updateSelection(prevX, prevY);
        drawGrid();
        delay(100);
    }

    // Timer countdown logic
    if (millis() - lastTimeUpdate >= 1000) {  // Every second
        lastTimeUpdate = millis();
        timer--;
        drawTimer();

        if (timer <= 0) {
            resetGame();
            timer = 999;  // Reset timer
        }
    }
  }

    //menuLoop();  // Return to menu after exit
}


void mineUp(){
  if (selectedY > 0) selectedY--;
}

void mineDown(){
  if (selectedY < gridSize - 1) selectedY++;
}

void mineLeft(){
  if (selectedX < gridSize - 1) selectedX++;
}

void mineRight(){
  if (selectedX > 0) selectedX--;
}

void mineButton1(){
  if(waitToStartMine){
    waitToStartMine = false;
  }
  else{
    revealTile(selectedX, selectedY);
  }
}

void mineButton2(){
  if(waitToStartMine){
    waitToStartMine = false;
  }
  else{
    toggleFlag();
  }
}