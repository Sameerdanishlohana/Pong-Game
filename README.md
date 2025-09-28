# Pong-Game# 
🎮 Pong 2025 — C++ with Raylib

A modern remake of the classic Pong game, built in **C++** using the **raylib** graphics and audio library.
This project features a clean code structure, smooth gameplay, and a vibrant cyberpunk-inspired design.

## ✨ Features

* **Game Modes**: Menu, difficulty selection, countdown, gameplay, and game-over screen
* **Difficulty Levels**: Easy, Medium, Hard (adjustable CPU paddle speed)
* **Dynamic Countdown**: 3–2–1 countdown before each match starts
* **Pause Menu**: Press `P` to pause, with option to return to the main menu
* **Scoring System**: First to 5 points wins
* **Visual Style**: Neon-inspired color scheme with glowing effects
* **Audio Effects**: Paddle hits, scoring sounds, and countdown chimes
* **Smooth Animations**: Sine-wave fade effects for menus and UI text

## 🎨 Screens & Gameplay

* **Main Menu** – Start the game with ENTER
* **Difficulty Selection** – Choose from 3 AI levels
* **Gameplay** – Player (↑/↓ keys) vs CPU
* **Pause** – Toggle pause with `P`
* **Game Over** – Replay or return to menu

## 🛠️ Tech Stack

* **Language**: C++
* **Graphics/Audio**: [raylib](https://www.raylib.com/)
* **Font/Assets**: Custom font + sound effects

## 🎮 Controls

* **Player Paddle**: `↑` and `↓`
* **Pause/Resume**: `P`
* **Return to Menu (when paused)**: `E`
* **Restart Game**: `ENTER` (on game over screen)

## 🚀 How to Run

1. Clone the repo
2. Install [raylib](https://github.com/raysan5/raylib)
3. Compile and run:

   ```bash
   g++ main.cpp -o pong -lraylib -lm -ldl -lpthread -lGL -lX11
   ./pong
   ```

## 📌 Future Improvements

* Two-player local mode
* Power-ups and special ball mechanics
* Online multiplayer support

---

⚡ **Pong 2025** is a fun project to practice **C++ OOP, game states, and raylib**. Perfect for anyone interested in game development basics.

