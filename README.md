## Team Members and Roles

| Name             | Role(s)                    | Key Contributions / Responsibilities |
|------------------|---------------------------|--------------------------------------|
| Rafik Ibrahim    | Electrical Engineer (EE)   | Designed and assembled main console PCB; implemented power system; integrated Bluetooth, joystick, VGA, and DFPlayer Mini; conducted hardware testing, firmware programming, and signal validation. |
| Annabelle Werner | Computer Engineer (COE)    | Developed Minesweeper game; managed multiplayer functionality; ensured controller input integrity; integrated software with hardware; tested ESP32-Bluetooth communication. |
| Tanvi Verma      | Computer Engineer (COE)    | Developed Pong game logic; implemented dual-screen synchronization; designed main UI with Anh; handled input mapping, game testing, debugging, and GitHub/Discord coordination. |
| Anh Nguyen       | Computer Engineer (COE)    | Developed Tetris game; designed and fabricated console enclosure; optimized gameplay; collaborated on UI design; managed part procurement, budgeting, and assembly. |

# Project TARA – Multiplayer Retro Game Console

**Project TARA** is an innovative retro-style multiplayer game console developed by a team of 4 Pitt ECE students over a 13-week project. It combines nostalgia, strategy, and engineering innovation to deliver a fun, engaging, and accessible gaming experience.  

The console features **three classic games**: **Tetris, Minesweeper, and Pong**, playable in both **single-player and multiplayer modes**. Its unique **dual-screen setup** ensures dynamic gameplay while maintaining independent player experiences.
<img width="1590" height="835" alt="image" src="https://github.com/user-attachments/assets/47c2d415-1db1-461b-8241-413e16bf4791" />

---

## Table of Contents
- [Overview](#overview)
- [System Architecture](#system-architecture)
- [Games and Gameplay](#games-and-gameplay)
- [System Requirements](#system-requirements)
- [Hardware Design](#hardware-design)
- [Software Architecture](#software-architecture)
- [How to Play](#how-to-play)
- [Testing and Results](#testing-and-results)
- [Future Work](#future-work)

---

## Overview

Project TARA provides **accessible, social, and cognitively engaging gaming**. Unlike modern consoles with complex graphics, this design focuses on simplicity, skill-building, and social interaction.  

- Single-player and multiplayer modes allow **solo challenges or head-to-head competition**.  
- Multiplayer formats include racing for **high score** or competing for **longest survival**.  
- Dual-screen setup ensures **independent gameplay** for each player.  

---

## System Architecture

The system consists of **three key subsystems**:

1. **Main Game Console Board** – Hosts two ESP32 microcontrollers for game logic, VGA output, and multiplayer communication.
2. **Wireless Game Controllers** – Each uses an ATmega328P to read joystick and button inputs, transmitted via Bluetooth.
3. **Audio Playback Board** – Dedicated ATmega328P controls a DFPlayer Mini module for modular and responsive audio.

**Multiplayer synchronization** is handled via **UART communication** between the ESP32 boards to avoid Wi-Fi latency and ensure real-time gameplay.
<img width="838" height="471" alt="image" src="https://github.com/user-attachments/assets/2f21b001-18ac-4820-b602-cc1ed073dafe" />
<img width="590" height="517" alt="image" src="https://github.com/user-attachments/assets/ddaeec5d-de03-4702-8814-db7b50ee6ed3" />

---

## Games and Gameplay

### Tetris
- 10x20 matrix, falling blocks, line clearing, and increasing speed with levels.  
- Multiplayer mode can send “garbage lines” to opponents.  

### Pong
- Two-player table tennis game, ball physics, and paddle collision.  
- Players start with 3 lives; ball speed increases with hits.  
- Multiplayer uses separate screens; game status synced over UART.  

### Minesweeper
- 9x9 grid with hidden bombs, recursive reveal, and countdown timers.  
- Multiplayer is cooperative: both players must complete the stage together.  

---

## System Requirements

### Required Features
- Reliable power supply for console and controllers.
- Controller battery life ≥ 6 hours.
- Menu navigation via controller.
- Wireless input latency < 100 ms.
- Real-time game logic.
- Dual-screen VGA output.
- Clear game state transitions.

### Desired Features
- Background music playback.
- Extended controller battery life (up to 12 hours).
- Controller LED power indicator.
- On-screen controller connection confirmation.
- Start/exit games directly from controller.

---

## Hardware Design

- **Controller PCB** – ATmega328P, joystick, buttons, HC-05 Bluetooth, and battery management.
<img width="830" height="641" alt="image" src="https://github.com/user-attachments/assets/44e7121e-f45c-4cdf-a66c-310249d032c6" />
<img width="621" height="468" alt="image" src="https://github.com/user-attachments/assets/58eb63a6-db8d-497d-9481-5c83f599ac6d" />

- **Main Console PCB** – Two ESP32s, VGA connectors, UART communication lines.
<img width="659" height="509" alt="image" src="https://github.com/user-attachments/assets/2cdc5bb6-18ef-45b1-ac44-045537bd00bf" />
<img width="644" height="291" alt="image" src="https://github.com/user-attachments/assets/e62cc114-e7ab-4438-a643-bb6dd6d257aa" />

- **Audio PCB** – ATmega328P + DFPlayer Mini, decoupled from main game logic for reliability.
<img width="928" height="717" alt="image" src="https://github.com/user-attachments/assets/b5dd2f9c-be2a-4c02-8baa-94bdfdc79a89" />
<img width="727" height="548" alt="image" src="https://github.com/user-attachments/assets/fe5d8eb5-2b33-4356-b8c6-354998fad7f7" />

- **Enclosures** – 3D-printed shells for controllers and modular console box for main board.
<img width="862" height="348" alt="image" src="https://github.com/user-attachments/assets/b1357a5a-c4ae-4de1-b11a-f5d57215645e" />
<img width="975" height="299" alt="image" src="https://github.com/user-attachments/assets/c0c48e69-bcc7-43db-8ced-666492e64bb7" />
<img width="1409" height="852" alt="image" src="https://github.com/user-attachments/assets/bb86ffbe-4466-4e02-a1dc-e513e0d14c86" />
<img width="719" height="495" alt="image" src="https://github.com/user-attachments/assets/8e287086-6db8-4be5-a17d-e35c6d1d7826" />
<img width="719" height="495" alt="image" src="https://github.com/user-attachments/assets/ad70872c-e735-4835-a494-708325800291" />


---

## Software Architecture

- Written in **C++** optimized for ESP32.
- **Menu system** on VGA display; controlled via joystick/buttons.
<img width="518" height="294" alt="image" src="https://github.com/user-attachments/assets/d9174632-3733-4f21-a567-7d22e0b3915f" /> <img width="517" height="247" alt="image" src="https://github.com/user-attachments/assets/da4c9007-0fc0-4ce7-8868-be0052660c1a" />

- **Finite state machine**: BOOT → MENU → GAME → GAME_OVER → MENU.
- Multiplayer handled via UART between ESP32 boards.
- Game modules:
  - Tetris: matrix-based block control.
    <img width="788" height="493" alt="image" src="https://github.com/user-attachments/assets/9d83922b-f27e-47a4-b59e-6ea96787215c" />

  - Pong: ball physics, scoring, and lives.
    <img width="769" height="515" alt="image" src="https://github.com/user-attachments/assets/aaedd23d-7c59-4462-81e0-99b8f44b563b" />

  - Minesweeper: grid, bombs, flags, and countdown timers.
    <img width="862" height="456" alt="image" src="https://github.com/user-attachments/assets/8d11c44c-c5ab-4dba-a741-3c4d4aaaeecf" />

---

## How to Play

1. **Power on the console** and controllers.  
2. **Pair controllers** with the console via the Bluetooth connection screen.
   <img width="975" height="315" alt="image" src="https://github.com/user-attachments/assets/b226f5f8-d5f6-49fa-9620-dd120793f82a" />  
4. **Select a game** using the joystick and buttons.  
5. **Choose mode**: Single-player or Multiplayer.  
6. **Play** using controller inputs:
   - Tetris: move, rotate, drop blocks.
   - Pong: move paddle and hit the ball.
   - Minesweeper: reveal or flag tiles.
7. **Game Over** screen displays scores and options to replay or return to menu.
   <img width="706" height="344" alt="image" src="https://github.com/user-attachments/assets/388ac685-b90f-4e3f-b6c1-c8e435c58f8a" />
  
8. In multiplayer:
   - P1 controls menu navigation.
   - Both players must confirm readiness before gameplay starts.
   - Final results synchronize across both screens.

---

## Testing and Results

- **Controller battery runtime:** ~72 hours (measured)
  <img width="975" height="584" alt="image" src="https://github.com/user-attachments/assets/6f264c1c-b9c9-4481-8582-2dcb74b64fb1" />

- **Input latency:** 38–52 ms  
- **VGA timing:** 320x200 @ 60 Hz, stable across monitors  
- **Audio response:** ~27 ms  
- **Bluetooth pairing:** ~4 s average
  <img width="951" height="471" alt="image" src="https://github.com/user-attachments/assets/8d0623d5-e917-4571-86bd-5dcba1a92270" />
  <img width="907" height="449" alt="image" src="https://github.com/user-attachments/assets/ae28a40a-1235-4a29-bb08-ce4a9439dfd5" />

- **Continuous gameplay:** 3× 120 min sessions without issues  
- **Multiplayer synchronization:** Fully stable via UART

  

---

## Future Work

- Add **more games** to the console.  
- Implement **modular game cartridges** for easy expansion.  
- Enhance **controller ergonomics** and battery efficiency.  
- Introduce **online multiplayer capabilities** while maintaining low-latency performance.

---
## Quick Start / How to Play

Follow these steps to get started with **Project TARA**:

### 1. Power On
- Connect the **main console** to a wall power supply.
- Turn on both **wireless controllers**.

### 2. Pair Controllers
- The console will display a **Bluetooth pairing screen**.
- Wait for each controller to show **“Connected!”** before proceeding.

### 3. Navigate the Menu
- Use the **joystick** and **buttons** to select:
  - **Game**: Tetris, Pong, or Minesweeper  
  - **Mode**: Single-player or Multiplayer

### 4. Gameplay Controls

**Tetris**
- Move piece: **Left / Right joystick**
- Rotate piece: **Rotate button**
- Hard drop: **Drop button**

**Pong**
- Move paddle: **Joystick Up/Down**
- Optional special buttons for game features (if configured)

**Minesweeper**
- Move cursor: **Joystick**
- Reveal tile: **Action button**
- Flag tile: **Flag button**

### 5. Game Over
- Score and results are displayed on-screen.
- Press the **menu button** to return to the main menu or replay.

### 6. Multiplayer Notes
- Player 1 controls **menu navigation and game start**.
- Both players must **confirm readiness** before multiplayer games start.
- Tetris & Pong: results sync after both players finish.  
- Minesweeper: both players must finish or exit together.

---

**Tip:** Make sure controllers are fully charged for uninterrupted gaming. Enjoy the retro multiplayer experience!
