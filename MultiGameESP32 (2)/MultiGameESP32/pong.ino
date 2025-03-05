#include <ESP32Lib.h>

// Pong Variables
bool waitToStartPong = true;
int pong_paddleY = 100;
int pong_paddleHeight = 30;
int pong_paddleWidth = 5;
int pong_ballX = 50;
int pong_ballY = 50;
int pong_ballSize = 3;
int pong_ballSpeedX = 2;
int pong_ballSpeedY = 1;
int pong_score = 0;

const int pong_resX = 320;
const int pong_resY = 200;

//Fixed processInputs for Pong (uses MultiGameESP32 buttons)
/*
void processInputsPong() {
    if (digitalRead(button2) == HIGH) { // Right button moves paddle UP
        pong_paddleY -= 3;
    }
    if (digitalRead(button4) == HIGH) { // Left button moves paddle DOWN
        pong_paddleY += 3;
    }

    // Keep paddle within screen bounds
    if (pong_paddleY < 0) pong_paddleY = 0;
    if (pong_paddleY + pong_paddleHeight > pong_resY) pong_paddleY = pong_resY - pong_paddleHeight;
}
*/

void pongDown(){
  pong_paddleY += 3;
  if (pong_paddleY + pong_paddleHeight > pong_resY) pong_paddleY = pong_resY - pong_paddleHeight;
}

void pongUp(){
  pong_paddleY -= 3;
  if (pong_paddleY < 0) pong_paddleY = 0;
}


void pongButton1(){
  waitToStartPong = false;
}

void pongButton2(){
  waitToStartPong = false;
}


void updateBall() {
    pong_ballX += pong_ballSpeedX;
    pong_ballY += pong_ballSpeedY;

    // Bounce off top and bottom walls
    if (pong_ballY <= 0 || pong_ballY >= pong_resY - pong_ballSize) {
        pong_ballSpeedY = -pong_ballSpeedY;
    }

    // Bounce off left wall
    if (pong_ballX <= 0) {
        pong_ballSpeedX = -pong_ballSpeedX;
    }

    // Bounce off paddle
    if (pong_ballX + pong_ballSize >= pong_resX - pong_paddleWidth &&
        pong_ballY >= pong_paddleY && pong_ballY <= pong_paddleY + pong_paddleHeight) {
        pong_ballSpeedX = -pong_ballSpeedX;
        pong_score++;
    }

    // If ball goes past the paddle, reset
    if (pong_ballX > pong_resX) {
        pong_ballX = pong_resX / 2;
        pong_ballY = pong_resY / 2;
        pong_ballSpeedX = -2; // Start moving left
        pong_ballSpeedY = 1;
        pong_score = 0;
    }
}

void drawPaddle() {
    vga.fillRect(pong_resX - pong_paddleWidth, pong_paddleY, pong_paddleWidth, pong_paddleHeight, vga.RGB(255, 255, 255));
}

void drawBall() {
    vga.fillRect(pong_ballX, pong_ballY, pong_ballSize, pong_ballSize, vga.RGB(255, 255, 255));
}

void drawScore() {
    vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0));
    vga.setCursor(10, 10);
    vga.print("Score: ");
    vga.print(pong_score);
}

void runPong(bool isMultiplayer) {
    vga.clear(0);
    delay(100);
    waitToStartPong = true;  // Ensure wait flag is reset
    gameRunning = true;

    if (isMultiplayer) {
        vga.setCursor(90, 100);
        vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0));
        vga.print("Waiting for P2...");

        while (digitalRead(button5) == LOW) { // Wait for button 5 (Select) to start
            delay(50);
        }
    } else {
        vga.setCursor(90, 100);
        vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0));
        vga.print("Press any button to start...");

        while (waitToStartPong) {  
            readController();  

            if (!waitToStartPong) {  
                break;
            }
        }
    }

    vga.clear(0);  // Clear the waiting screen

    //**Main Pong Game Loop**
    while (gameRunning) {  
        readController();
        updateBall();

        vga.clear(0);
        drawPaddle();
        drawBall();
        drawScore();
        vga.show();

        delay(10);
    }

    // **Reset Pong properly before exiting**
    resetPong();
    menuLoop();
}

void resetPong() {
    // **Reset all Pong variables to initial state**
    waitToStartPong = true;
    gameRunning = false;  
    pong_paddleY = 100;  
    pong_ballX = 50;
    pong_ballY = 50;
    pong_ballSpeedX = 2;
    pong_ballSpeedY = 1;
    pong_score = 0;
}


