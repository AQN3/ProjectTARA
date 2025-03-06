

#include <ESP32Lib.h>
// #include <Ressources/Font6x8.h>
// #include <Ressources/Font8x8.h>

//------- original bitluni pin definition ----------------------------------------------
// const int redPins[] = {2, 4, 12, 13, 14};
// const int greenPins[] = {15, 16, 17, 18, 19};
// const int bluePins[] = {21, 22, 23, 27};
// const int hsyncPin = 32;
// const int vsyncPin =33;

//-------------------- button pin definitions -----------------------
// byte button1 = 26; //right
// byte button2 = 25; //rotate
// byte button3 = 34; //left
// byte button4 = 35; //down fast 
//-------------------------------------------------------------------

boolean waitToStart = true;
boolean button;
boolean button_1;
boolean button_2;
boolean button_3;
boolean button_4;
boolean button_5;
boolean button_1_old;
boolean button_2_old;
boolean button_3_old;
boolean button_4_old;
boolean button_5_old;
int block[4][2]={{0,0},{0,0},{0,0},{0,0}};
int blockExt[4][2]={{0,0},{0,0},{0,0},{0,0}};
int blockOld[4][2]={{0,0},{0,0},{0,0},{0,0}};
int blockTmp[4][2]={{0,0},{0,0},{0,0},{0,0}};
int blockTr[4][2]={{0,0},{0,0},{0,0},{0,0}};
int yLine[4] = {0,0,0,0}; 
int yLineTmp[4] = {0,0,0,0}; 
int yCounter = 0; 
int x = 60; 
int y = 6; 
int z = 10; 
int score, scoreNew; 
int noLoop = -1; 
int rotationDirection = 1; 
int delta = 0;  
int color = 1; 
int colorOld; 
int blockN, blockNext; 
int busy; 
int noDelete = 0; 
int k = 0; 
int a = 40;
int b = 10; 
int counterMenu = 0; 
unsigned long myTime = 0;
int fast = 14; //14; 
int resX = 320; 
int resY = 200; 
int colA, colB, colC;
int squareX = 7; 
int squareY = 6; 
int myColumns = 11; 
int myRaws = 26; 
int level = 1; 
int totalLinesCleared = 0; // Tracks total cleared lines across the game
unsigned long gameStartTime = 0;  // Stores the start time
unsigned long elapsedTime = 0;    // Stores elapsed time in milliseconds


//VGA Device
//VGA3Bit vga;

//initial setup
// void setup()
// {
//   //we need double buffering for smooth animations
//   //vga.setFrameBufferCount(2);
//   vga.setFrameBufferCount(1);
//   //initializing i2s vga (with only one framebuffer)
//   vga.init(vga.MODE320x240, 14, 19, 27, hsyncPin, vsyncPin);
//   //setting the font
//   vga.setFont(Font6x8);
//   randomSeed(analogRead(5)); 
//   pinMode(button1,INPUT);
//   pinMode(button2,INPUT);
//   pinMode(button3,INPUT);
//   pinMode(button4,INPUT);
// }
/*
void processInputs() {
   button_1_old = button_1;
   button_2_old = button_2;
   button_3_old = button_3;
   button_4_old = button_4;
   if (button_1 == 0 ){ 
      if (button_1_old == 0)
         {button_1 = digitalRead(button1);} 
      else 
         {button_1_old = 0;}
   }
   else
   {button_1 = 0;}
   if (button_2 == 0 ){ 
      if (button_2_old == 0)
         {button_2 = digitalRead(button2); delay(50);} 
      else 
         {button_2_old = 0;}
   }
   else
   {button_2 = 0;}
   if (button_3 == 0 ){ 
      if (button_3_old == 0)
         {button_3 = digitalRead(button3);} 
      else 
         {button_3_old = 0;}
   }
   else
   {button_3 = 0;}
   /*if (button_4 == 0 ){ 
      if (button_4_old == 0)
         {button_4 = digitalRead(button4);} 
      else 
         {button_4_old = 0;}
   }
   else
   {button_4 = 0;}
   */
   /*
   if (button_4 == 0 ){digitalRead(button4);} 
   else {button_4 = 0;}
   
   button_4 = digitalRead(button4); 
   button = button_2 || button_4; 
}


void clearInputs() {
  button_1 = 0; 
  button_2 = 0;
  button_3 = 0; 
  button_4 = 0;
  button = 0; 
}
*/

// void drawMenu() {
//   while (button_1 == 0 && button_2 == 0 && button_3 == 0 && button_4 == 0) {
//     processInputs();

//     // Set text color to blue (RGB: 0, 0, 255) BEFORE printing
//     vga.setTextColor(vga.RGB(0, 0, 255), vga.RGB(0, 0, 0));  

//     // Print "SinglePlayer Tetris"
//     vga.setCursor(90, 60);
//     vga.print("SinglePlayer Tetris");

//     // Print "Team 16"
//     vga.setCursor(95, 80);
//     vga.print("Team 16");

//     // Underline both with white
//     vga.line(90, 70, 230, 70, vga.RGB(255, 255, 255));
//     vga.line(95, 90, 170, 90, vga.RGB(255, 255, 255));

//     delay(100);
//     counterMenu++;
//   }
//   vga.clear(0);
//   drawGameScreen(); 
//   drawScore(score);
// }

void drawScore(int i) {
      // Call timer function first
    if (totalLinesCleared >= level * 10) {  
    level++;  
    if (fast > 5) fast -= 3;  
    else fast = max(2, fast - 1);
    }

    //smoothRect(206, 77, 62, 23, 8, 2); 
    setPrint(230, 100, 5);
    vga.print("LEVEL:");
    setPrint(230, 110, 4);
    vga.print(level);
    //smoothRect(200, 107, 74, 22, 8, 3); 
    setPrint(30, 100, 5);
    vga.print("SCORE:");
    setPrint(30, 110, 4);
    vga.print(scoreNew);
}

void drawTimer_tetris() {
    elapsedTime = (millis() - gameStartTime) / 1000;  // Convert to seconds
    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;

    // Clear the old time before updating
    vga.fillRect(230, 16, 50, 10, vga.RGB(0, 0, 0));  // Black background to clear

    // Display "TIME" label
    setPrint(230, 16, 5);
    vga.print("TIME:");

    // Display the timer value
    setPrint(230, 26, 4);
    if (minutes < 10) vga.print("0");
    vga.print(minutes);
    vga.print(":");
    if (seconds < 10) vga.print("0");
    vga.print(seconds);
}


void drawBorder() {
   vga.rect(120, 2, squareX * (myColumns - 1) - 2, squareY * myRaws + 23 - 2, vga.RGB(255, 255, 255));  
   vga.line(120, 4 * squareY + 2, 120 + squareX * (myColumns - 1) - 3, 4 * squareY + 2, vga.RGB(255, 255, 0));

   vga.setTextColor(vga.RGB(255, 255,255), vga.RGB(0, 0, 0));  // Yellow text
   vga.setCursor(137, 7);  // Adjust position (x, y)
   vga.print("TETRIS");

   // Cover area above the yellow line (but below "TETRIS") with a black box
   int boxX = 130;  // Start of game area
   int boxY = 14;   // Position below "TETRIS"
   int boxWidth = squareX * (myColumns - 1) - 20;
   int boxHeight = 4 * squareY - 13;  // Adjust height based on spacing

   vga.fillRect(boxX, boxY, boxWidth, boxHeight, vga.RGB(0,0,0));  // Black box to block unwanted blocks

   setPrint(30, 16, 5);
   vga.print("NEXT BLOCK:");
}



// --------------------- this is for the beginning game window ------------------------ 
void drawStartScreen() {
   drawBorder(); 
   drawGameScreen(); 
   button = 0;
   delay(200);
} 

// ---------------------- this is the main function to draw the game screen -----------
void drawGameScreen() {
  drawBorder(); 
}


// ----------------------- Tetriminos definition --------------------------------------
void blockDef(int i) {
  if (i == 1){
  // O
  block[0][0] = 0;
  block[0][1] = 0;
  block[1][0] = 1;
  block[1][1] = 0;
  block[2][0] = 0;
  block[2][1] = 1;
  block[3][0] = 1;
  block[3][1] = 1;
  color = 2; 
  //colorNew = 1;
  }
  if (i == 2){
  // L
  block[0][0] = -1;
  block[0][1] = 0;
  block[1][0] = 0;
  block[1][1] = 0;
  block[2][0] = 1;
  block[2][1] = 0;
  block[3][0] = -1;
  block[3][1] = 1;
  color = 3; 
  //colorNew = 2;
  }
  if (i == 3){
  // J
  block[0][0] = -1;
  block[0][1] = 0;
  block[1][0] = 0;
  block[1][1] = 0;
  block[2][0] = 1;
  block[2][1] = 0;
  block[3][0] = 1;
  block[3][1] = 1;
  color = 4; 
  //colorNew = 3;
  }
  if (i == 4){
  // I
  block[0][0] = -1;
  block[0][1] = 0;
  block[1][0] = 0;
  block[1][1] = 0;
  block[2][0] = 1;
  block[2][1] = 0;
  block[3][0] = 2;
  block[3][1] = 0;
  color = 5; 
  //colorNew = 4;
  }
  if (i == 5){
  // S
  block[0][0] = -1;
  block[0][1] = 0;
  block[1][0] = 0;
  block[1][1] = 0;
  block[2][0] = 0;
  block[2][1] = 1;
  block[3][0] = 1;
  block[3][1] = 1;
  color = 6; 
  //colorNew = 5;
  }
  if (i == 6){
  // Z
  block[0][0] = -1;
  block[0][1] = 1;
  block[1][0] = 0;
  block[1][1] = 1;
  block[2][0] = 0;
  block[2][1] = 0;
  block[3][0] = 1;
  block[3][1] = 0;
  color = 3; 
  //colorNew = 6;
  }
  if (i == 7){
  // T
  block[0][0] = -1;
  block[0][1] = 0;
  block[1][0] = 0;
  block[1][1] = 0;
  block[2][0] = 0;
  block[2][1] = 1;
  block[3][0] = 1;
  block[3][1] = 0;
  color = 1; 
  //colorNew = 7;
  }
}

// -------------------------- expansion for 4:3 monitors ------------------------------ 
void blockExtension() {
   for (int i = 0; i < 4; i++){
      blockExt[0][0] = block[0][0]*3;
      blockExt[0][1] = block[0][1]*2;
      blockExt[1][0] = block[1][0]*3;
      blockExt[1][1] = block[1][1]*2;
      blockExt[2][0] = block[2][0]*3;
      blockExt[2][1] = block[2][1]*2;
      blockExt[3][0] = block[3][0]*3;
      blockExt[3][1] = block[3][1]*2;
   }
}
 
void blockRotation(int rotationDirection){
  for (int i = 0; i < 4; i++){
     blockOld[0][0] = block[0][0];
     blockOld[0][1] = block[0][1];
     blockOld[1][0] = block[1][0];
     blockOld[1][1] = block[1][1];
     blockOld[2][0] = block[2][0];
     blockOld[2][1] = block[2][1];
     blockOld[3][0] = block[3][0];
     blockOld[3][1] = block[3][1];
  }
  for (int i = 0; i < 4; i++){
     block[0][0] = blockOld[0][1]*rotationDirection;
     block[0][1] = -blockOld[0][0]*rotationDirection;
     block[1][0] = blockOld[1][1]*rotationDirection;
     block[1][1] = -blockOld[1][0]*rotationDirection;
     block[2][0] = blockOld[2][1]*rotationDirection;
     block[2][1] = -blockOld[2][0]*rotationDirection;
     block[3][0] = blockOld[3][1]*rotationDirection;
     block[3][1] = -blockOld[3][0]*rotationDirection;
  }
}
void blockTranslation(int x, int y) {
   for (int i = 0; i < 4; i++){
      blockTr[0][0] = blockExt[0][0] + x;
      blockTr[0][1] = blockExt[0][1] + y;
      blockTr[1][0] = blockExt[1][0] + x;
      blockTr[1][1] = blockExt[1][1] + y;
      blockTr[2][0] = blockExt[2][0] + x;
      blockTr[2][1] = blockExt[2][1] + y;
      blockTr[3][0] = blockExt[3][0] + x;
      blockTr[3][1] = blockExt[3][1] + y;
   }
}

void delBlock(){
  if (noDelete == 1) {noDelete = 0;} 
  else {
      for (int i = 0; i < 4; i++){
         mySquare(blockTr[i][0] * 2 + 31, blockTr[i][1] * 3 + 1, 0); 
      }
   }
}

void drawBlock(){
  for (int i = 0; i < 4; i++){
     mySquare(blockTr[i][0] * 2 + 31, blockTr[i][1] * 3 + 1, color); 
  }
  for (int i = 0; i < 4; i++){
     blockTmp[0][0] = blockTr[0][0];
     blockTmp[0][1] = blockTr[0][1];
     blockTmp[1][0] = blockTr[1][0];
     blockTmp[1][1] = blockTr[1][1];
     blockTmp[2][0] = blockTr[2][0];
     blockTmp[2][1] = blockTr[2][1];
     blockTmp[3][0] = blockTr[3][0];
     blockTmp[3][1] = blockTr[3][1];
  }
}

void drawBlockTmp(){
  for (int i = 0; i < 4; i++){
     mySquare(blockTmp[i][0] * 2 + 31, blockTmp[i][1] * 3 + 1, color); 
  }
}

void checkBlock(){ 
  busy = 0;  
  for (int i = 0; i < 4; i++){
     busy = busy + vgaget(2*blockTr[i][0] + 31, 3*blockTr[i][1] + 1) + vgaget(2*(blockTr[i][0] + 2) + 32, 3*blockTr[i][1] + 1); 
  }
}

void replaceBlock(){
     blockExtension(); 
     blockTranslation(x, y); 
     checkBlock();
     if (busy == 0){
        drawBlock(); 
     }
     else // ---------- else is run if the block cannot get down  ----------------- 
     {
        drawBlockTmp(); 
        checkForFullLine(); // ---- check il the line is filled when the block cennot get down anymore ---------------------- 
        noLoop = 0; 
        noDelete = 1; 
        if (y < 8) {
           delBlock();
           gameOver();
        }
     }
     delay(1);
}

void gameOver() { 
   noLoop = -1; 
   score = 0; 
   level = 1; 
   scoreNew = 0;
   fast = 14;
   //clearInputs(); 
   myTime = 0; 

   vga.setCursor(130, 100);
   vga.print("GAME OVER");
   //smoothRect(20, 91, 80, 25, 11, 1); 

   delay(300);
   vgaTone(660, 200); 
   vgaTone(330, 200);
   vgaTone(165, 200); 
   vgaTone(82, 200);
   waitToStart = true;
   // Wait for any button press to restart
   while (waitToStart) {
      readController(); 
      delay(100);
   }

   // **RESET GAME VARIABLES PROPERLY**
   vga.clear(0);  
   drawGameScreen();  // Ensure screen redraws before generating blocks
   drawScore(score);

   // **Reset block positions and generate first two blocks properly**
   x = 57;
   y = 5;
   blockN = 1 + int(random(7));  // First block
   blockNext = 1 + int(random(7));  // Next block

   blockDef(blockNext);
   drawBlockNext();  // Show next block before game starts
   blockDef(blockN);
   noLoop = 1;  // Allow normal gameplay to resume
}


void drawBlockNext() {  
     static int lastX = 30;  // Track last drawn block position
     static int lastY = 55;

     // **Calculate block size for accurate clearing**
     int blockWidth = squareX * 4;  
     int blockHeight = squareY * 4;  

     // **CLEAR OLD BLOCK**  
     vga.fillRect(lastX - 2, lastY - 40, blockWidth + 4, blockHeight + 44, vga.RGB(0, 0, 0));


     // **New block position directly under "NEXT" text**
     int newX = 30;
     int newY = 55;  // Lower it slightly  

     blockExtension();  
     blockTranslation(newX / squareX, newY / squareY);  
     drawBlock();  

     // **Update last position for the next clear cycle**
     lastX = newX;
     lastY = newY;
}






void checkBlockTranslation(){
     x = x + delta; 
     blockExtension(); 
     blockTranslation(x, y); 
     checkBlock();
     if (busy == 0){
        drawBlock(); 
     }
     else
     {
        x = x - delta; 
        blockExtension(); 
        blockTranslation(x, y); 
        drawBlock(); 
     }
     delay(50);
}

void checkBlockRotation(){
     //x = x + delta; 
     blockExtension(); 
     blockTranslation(x, y); 
     checkBlock();
     if (busy == 0){
        drawBlock(); 
     }
     else
     {
        rotationDirection = -rotationDirection; 
        blockRotation(rotationDirection); 
        blockExtension(); 
        blockTranslation(x, y); 
        drawBlock(); 
     }
     delay(50);
}

void checkForFullLine() { // --------------------- check if the line is full and must be deleted --------------
   for (int i = 0; i < 4; i++){
      for (int j = 45; j < 76; j += 3) {
         //if (vgaget(j, blockTmp[i][1]) >0){k++; }
         if (vgaget(2*j + 32, 3*blockTmp[i][1] + 2) >0){k++; }
      }
      if (k == 11) { // ---- line is full and must be deleted ----------------------------------------------------------
         vga.fillRect(45 * 2 + 31, blockTmp[i][1] * 3 + 1, squareX *11, squareY, vga.RGB(0, 0, 0)); 
         yLineTmp[yCounter] = blockTmp[i][1]; 
         yLine[yCounter] = blockTmp[i][1]; 
         yCounter++; 
         vgaTone(660,30); 
      }
      k = 0; 
    }
    if (yLineTmp[yCounter - 1] < yLine[0]) { // ------------ qui va capito se va < o > (penso >) ----------------------
       for (int i = 0; i < yCounter; i++) { // ------------- inversion ---------------------------------------
          yLine[i] = yLineTmp[yCounter - i - 1]; 
       }
    }
    for (int i = 0; i < yCounter; i++){   
          for (int y = yLine[i] - 2; y > 0; y -= 2) {
              for (int x = 45; x < 76; x += 3) {
                  int newColor = vgaget(2*x + 32, 3*y + 2); 
                  if (newColor > 0) {
                      mySquare(x * 2 + 31, y * 3 + 1, 0);  // Clear old position
                      mySquare(x * 2 + 31, (y + 2) * 3 + 1, newColor);  // Move color correctly
                  }
              }
          }
      }



  if (yCounter != 0) {
      totalLinesCleared += yCounter; 
      int basePoints = 40 * pow(2, (yCounter - 1)); // 40, 100, 300, 1200 pattern
      score += basePoints * (level + 1); // Multiply by level+1 like the original
      scoreNew += basePoints;
      vgaTone(990, 30);
  }

   drawScore(score); 
   yCounter = 0; 
}




//-----------------------------------------------------------------------------------------------
//--------------------- This is the main loop of the game ---------------------------------------
//-----------------------------------------------------------------------------------------------
// void loop()
// {
//   //vga.clear(0);
//   //delay(10);
//   //myBackground(); 
//   //myBall(); 
//   processInputs(); 

//   if (noLoop < 1){ // --------------- to generate new Tetraminos --------------------------------
//      blockN = blockNext; 
//      if (noLoop == -1 ) { // -------------- only at the game beginning  -------------------------
//         drawMenu(); 
//         while (button_1 == 0 && button_2 == 0 && button_3 == 0 && button_4 == 0) {
//            blockN = 2 + int(random(6)); // -------------- tetraminos "O" is excluded -----------------
//            processInputs(); 
//         }
//      }
//      drawGameScreen();
//      drawScore(score);
//      blockNext = 1 + int(random(7));
//      blockDef(blockNext);
//      drawBlockNext();
//      blockDef(blockN);
//      x = 57; 
//      y = 5; 
//      button_1 = 1; 
//      noLoop = 1; 
//   }
//   if (button_2 == 1){ // ------------------------ rotation -------------------------
//      //if (button_5 == 1){rotationDirection = -1;}
//      if (button_2 == 1){rotationDirection = 1;}
//      delBlock();
//      blockRotation(rotationDirection); 
//      checkBlockRotation(); 
//   }
//   if (button_1 == 1 || button_3 == 1){ // ------- translation ----------------------
//      if (button_1 == 1){delta = 3;}
//      if (button_3 == 1){delta = -3;}
//      delBlock();
//      checkBlockTranslation();
//   }
//   myTime++; 
//   if (myTime % fast > fast - 2 || button_4 == 1){ // --- Tetraminos falling ----------
//      if (fast < 3) {fast = 2;}
//      y = y + 2;
//      delBlock(); 
//      replaceBlock(); 
//   }
//   else
//   {delay(10 + 2*fast);}
//   //show the rendering
//   //vga.show();
// }

// External function to return to the menu
extern void menuLoop();

void runTetris(bool isMultiplayer) {
    waitToStart = true;
    gameRunning = true;
    vga.clear(0);  // Clear screen before Tetris
    delay(100);
    
    if (isMultiplayer) {
        // **Multiplayer: Wait for P2 Ready Signal**
        vga.setCursor(90, 100);
        vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0));  
        vga.print("Waiting for P2...");

        while (true) {
            //processInputs();
            
            // Simulated P2 Ready Check (Replace with actual ESP32 communication)
            bool p2Ready = digitalRead(button5) == HIGH;  // Simulate P2 sending ready signal
            if (p2Ready) break;
            
            delay(50);
        }
    } else {
        // **Singleplayer: Wait for Any Button to Start**
        vga.setCursor(90, 100);
        vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0));  
        vga.print("Press any button to start...");

        while (waitToStart){
            readController();
        }
    }

    // **Start Game**
    vga.clear(0);
    gameStartTime = millis();  // Start the timer

    drawGameScreen();  // Initialize game screen
    
    drawScore(score);

    // **Generate the first two blocks before gameplay starts**
    blockN = 1 + int(random(7));  // First block
    blockNext = 1 + int(random(7));  // Next block
    blockDef(blockNext);
    drawBlockNext();  // Display next block
    blockDef(blockN);

    x = 57;
    y = 5;
    noLoop = 1;
    
    while (gameRunning) {  // Main game loop
      if(!gameRunning){
        break;
      }
      drawTimer_tetris();
      drawBorder();
        if (noLoop < 1) { 
            blockN = blockNext;  // Move next block into play
            blockNext = 1 + int(random(7));  // Generate new next block
            blockDef(blockNext);
            drawBlockNext();  // Display next block

            blockDef(blockN);  // Define new current block
            x = 57;
            y = 5;
            noLoop = 1;
        }
        
        /*
        if (button_2 == 1) {  // Rotate
            rotationDirection = 1;
            delBlock();
            blockRotation(rotationDirection);
            checkBlockRotation();
        }
        */
        readController();
        /*
        if (button_1 == 1 || button_3 == 1) {  // Move Left/Right
            delta = (button_1 == 1) ? 3 : -3;
            delBlock();
            checkBlockTranslation();
        }
        */

        myTime++;
        if (myTime % fast > fast - 2) {  // Fast Drop (Button 4)
            if (fast < 3) {fast = 2;}
            y = y + 2;
            delBlock();
            replaceBlock();
        }
        //else {
        delay(10 + 2 * fast);
        //}
    }
    waitToStart = true;
    resetTetris();
}

void resetTetris() {
    level = 1;
    scoreNew = 0;
}


void tetrisButton1(){
  waitToStart = false;
}

void tetrisButton2(){
  waitToStart = false;
}

void tetrisUp(){
  if(~(waitToStart)){
    rotationDirection = 1;
    delBlock();
    blockRotation(rotationDirection);
    checkBlockRotation();
  }
}

void tetrisDown(){
  if(~(waitToStart)){
    if (fast < 3) {fast = 2;}
    y = y + 2;
    delBlock();
    replaceBlock();
  }
}

void tetrisLeft(){
  if(~(waitToStart)){
    delta = 3;
    delBlock();
    checkBlockTranslation();
  }
}

void tetrisRight(){
  if(~(waitToStart)){
    delta = -3;
    delBlock();
    checkBlockTranslation();
  }
}
//-----------------------------------------------------------------------------------------------
//--------------------- This is the end of the main loop ----------------------------------------
//-----------------------------------------------------------------------------------------------



void vgaTone(int freq, int myTime){ // -------- it does not work... yet! ------------------------ 
   //vga.tone(freq);
   delay(myTime); 
   //vga.noTone(); 
}

void vgaline(int x0, int y0, int x1, int y1, int color){
   int a, b, c; 
   if (color == 0){a = 0; b = 0; c = 0;}
   if (color == 1){a = 1; b = 0; c = 0;}
   if (color == 2){a = 0; b = 1; c = 0;}
   if (color == 3){a = 0; b = 0; c = 1;}
   if (color == 4){a = 1; b = 1; c = 0;}
   if (color == 5){a = 1; b = 0; c = 1;}
   if (color == 6){a = 0; b = 1; c = 1;}
   if (color == 7){a = 1; b = 1; c = 1;}
   vga.line(x0, y0, x1 - 1, y1, vga.RGB(a*255, b*255, c*255)); 
}

void myLine(int x0, int y0, int x1, int y1, int color){
   int a, b, c; 
   if (color == 0){a = 0; b = 0; c = 0;}
   if (color == 1){a = 1; b = 0; c = 0;}
   if (color == 2){a = 0; b = 1; c = 0;}
   if (color == 3){a = 0; b = 0; c = 1;}
   if (color == 4){a = 1; b = 1; c = 0;}
   if (color == 5){a = 1; b = 0; c = 1;}
   if (color == 6){a = 0; b = 1; c = 1;}
   if (color == 7){a = 1; b = 1; c = 1;}
   vga.line(x0, y0, x1, y1, vga.RGB(a*255, b*255, c*255)); 
}

int vgaget(int x, int y){  
  if(vga.get(x, y) == 0)     {return 0;} 
  if(vga.get(x, y) == 31)    {return 1;} 
  if(vga.get(x, y) == 992)   {return 2;} 
  if(vga.get(x, y) == 15360) {return 3;} 
  if(vga.get(x, y) == 1023)  {return 4;} 
  if(vga.get(x, y) == 15391) {return 5;} 
  if(vga.get(x, y) == 16352) {return 6;} 
  if(vga.get(x, y) == 16383) {return 7;} 
}


void myColor(int color, int scheme = 2) { 
    // Use `scheme` to select different color palettes 
    switch (scheme) {
        case 2: // Standard Colors
            if (color == 0){colA = 0; colB = 0; colC = 0;} // Black
            if (color == 1){colA = 1; colB = 0; colC = 0;} // Red
            if (color == 2){colA = 0; colB = 1; colC = 0;} // Green
            if (color == 3){colA = 0; colB = 0; colC = 1;} // Blue
            if (color == 4){colA = 1; colB = 1; colC = 0;} // Yellow
            if (color == 5){colA = 1; colB = 0; colC = 1;} // Magenta
            if (color == 6){colA = 0; colB = 1; colC = 1;} // Cyan
            if (color == 7){colA = 1; colB = 1; colC = 1;} // White
            
            break;

        case 1: // Alternative Pastel Colors
            if (color == 0){colA = 0; colB = 0; colC = 0;}
            if (color == 1){colA = 1; colB = 0.5; colC = 0.5;} // Soft Red
            if (color == 2){colA = 0.5; colB = 1; colC = 0.5;} // Soft Green
            if (color == 3){colA = 0.5; colB = 0.5; colC = 1;} // Soft Blue
            if (color == 4){colA = 1; colB = 1; colC = 0.5;} // Light Yellow
            if (color == 5){colA = 1; colB = 0.5; colC = 1;} // Light Magenta
            if (color == 6){colA = 0.5; colB = 1; colC = 1;} // Light Cyan
            if (color == 7){colA = 1; colB = 1; colC = 1;} // White
            break;

        case 3: // High Contrast (Neon)
            if (color == 0){colA = 0; colB = 0; colC = 0;}
            if (color == 1){colA = 1; colB = 0.2; colC = 0.2;} // Neon Red
            if (color == 2){colA = 0.2; colB = 1; colC = 0.2;} // Neon Green
            if (color == 3){colA = 0.2; colB = 0.2; colC = 1;} // Neon Blue
            if (color == 4){colA = 1; colB = 1; colC = 0.2;} // Bright Yellow
            if (color == 5){colA = 1; colB = 0.2; colC = 1;} // Neon Magenta
            if (color == 6){colA = 0.2; colB = 1; colC = 1;} // Neon Cyan
            if (color == 7){colA = 1; colB = 1; colC = 1;} // White
            break;
    }
}


void myRect(int x0, int y0, int w, int h, int color){
   myColor(color); 
   vga.rect(x0, y0, w, h, vga.RGB(255, 255, 255)); 
}

void mySquare(int x0, int y0, int color){
   myColor(color); 
   vga.fillRect(x0 + 1, y0 + 1, squareX - 3, squareY - 2, vga.RGB(colA*255, colB*255, colC*255)); 
    if (color != 0) { myColor(7); }
    if (color == 7) { myColor(7); }
   vga.rect(x0, y0, squareX - 1, squareY, vga.RGB(colA*255, colB*255, colC*255)); 
}

void mySquare2(int x0, int y0, int color){ //---------------- simple version ----------------------------- 
   myColor(color); 
   vga.fillRect(x0, y0, squareX - 1, squareY, vga.RGB(255, 255, 255)); 
}


//void myPrint(int x, int y, String text, int color){
void setPrint(int x, int y, int color){
  myColor(color); 
  vga.setTextColor(vga.RGB(255, 255, 255), vga.RGB(0, 0, 0));
  vga.setCursor(x, y);
  //vga.print(String(text));
}

void smoothRect(int x0, int y0, int w, int h, int r, int color){  //----- 1.6 comes from the rsolution ratio - 320/200 -------------- 
   myLine(x0 + 1.6*r, y0 - 1, x0 + w - 1.6*r, y0 - 1, color); 
   myLine(x0 + 1.6*r, y0 + h, x0 + w - 1.6*r, y0 + h, color); 
   myLine(x0 - 1, y0 + r, x0 - 1, y0 + h - r, color); 
   myLine(x0 + w, y0 + r, x0 + w, y0 + h - r, color); 
   myColor(color); 
   for (int i = 0; i <= 25; i++) {
      vga.dot(x0 + w - r*1.6*(1 - cos(i/25.*3.1415/2.)), y0 + r*(1 - sin(i/25.*3.1415/2.)), vga.RGB(colA*255, colB*255, colC*255)); 
      vga.dot(x0 + r*1.6*(1 - cos(i/25.*3.1415/2.)), y0 + r*(1 - sin(i/25.*3.1415/2.)), vga.RGB(colA*255, colB*255, colC*255));
      vga.dot(x0 + w - r*1.6*(1 - cos(i/25.*3.1415/2.)), y0 + h - r*(1 - sin(i/25.*3.1415/2.)), vga.RGB(colA*255, colB*255, colC*255)); 
      vga.dot(x0 + r*1.6*(1 - cos(i/25.*3.1415/2.)), y0 + h - r*(1 - sin(i/25.*3.1415/2.)), vga.RGB(colA*255, colB*255, colC*255));
   }
}
