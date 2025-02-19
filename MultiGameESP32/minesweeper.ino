#include <ESP32Lib.h>

// VGA Pins
//const int redPin = 14, greenPin = 19, bluePin = 27, hsyncPin = 32, vsyncPin = 33;

// Button Pins
const int btnDown = 22, btnUp = 23, btnLeft = 5, btnRight = 18, btnClear = 34, btnFlag = 35;

VGA3Bit vga;

const int gridSize = 10;
const int blockSize = 20;
const int numBombs = 13;
const int gridOffsetX = 40;
const int gridOffsetY = 20;

int selectedX = 5, selectedY = 5;
int gameGrid[gridSize][gridSize] = {0};
bool revealed[gridSize][gridSize] = {false};
bool flagged[gridSize][gridSize] = {false};

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
        vga.setCursor(startX + 7, startY + 6);
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

    if (isMultiplayer) {
        // **Multiplayer: Wait for P2 Ready Signal*
    } else {
        // **Singleplayer: Wait for Any Button to Start**
        randomSeed(analogRead(0));
        resetGame()
    }

    while (true) {  // Main game loop
        int prevX = selectedX, prevY = selectedY;
        if (digitalRead(button2) == LOW && selectedY > 0) selectedY--;
        if (digitalRead(button4) == LOW && selectedY < gridSize - 1) selectedY++;
        if (digitalRead(button3) == LOW && selectedX > 0) selectedX--;
        if (digitalRead(button1) == LOW && selectedX < gridSize - 1) selectedX++;
        if (digitalRead(button5) == LOW) revealTile(selectedX, selectedY);
        if (digitalRead(button6)) == LOW) toggleFlag();
        
        if (prevX != selectedX || prevY != selectedY) {
            updateSelection(prevX, prevY);
            drawGrid();
            delay(100);
        }
    }

    menuLoop();  // Return to menu after exit
