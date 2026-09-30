# Cat Bot (Sesame Robot) - PlatformIO Project & Software

This directory contains the complete, ready-to-flash PlatformIO firmware and PC software for your Cat Bot / Sesame Robot.

---

## Quick Start: Flash Firmware via PlatformIO

1. **Open in VS Code / PlatformIO**:
   - Open VS Code.
   - Go to **File -> Open Folder...** and select **`I:\cat-bot`** (or `I:\cat bot`).
   - PlatformIO will automatically detect `platformio.ini` right at the root.

2. **Upload to Board**:
   - Plug your **Sesame Distro Board V2** into your PC via USB-C.
   - Click the PlatformIO **Upload** checkmark/arrow icon in the bottom status bar, or open the terminal and run:
     ```powershell
     pio run -t upload
     ```
   - *Note: If upload fails due to COM port busy, close any open serial monitor first.*

---

## Hardware Configuration (Pre-Configured)

The firmware is already configured in `src/main.cpp` for **Sesame Distro Board V2 (ESP32-S3)**:

- **Servo Pins (MG90S)**:
  - Motor 0 (R1 Front Right Hip): `GPIO 4`
  - Motor 1 (R2 Back Right Hip): `GPIO 5`
  - Motor 2 (L1 Front Left Hip): `GPIO 6`
  - Motor 3 (L2 Back Left Hip): `GPIO 7`
  - Motor 4 (R4 Back Right Leg): `GPIO 15`
  - Motor 5 (R3 Front Right Leg): `GPIO 16`
  - Motor 6 (L3 Front Left Leg): `GPIO 17`
  - Motor 7 (L4 Back Left Leg): `GPIO 18`
- **OLED Display (I2C)**:
  - `SDA`: `GPIO 8`
  - `SCL`: `GPIO 9`
- **Libraries**:
  - `ESP32Servo @ 3.0.9` (pinned to prevent multi-servo channel corruption)
  - `Adafruit SSD1306` + `Adafruit GFX`

---

## Controlling the Robot

### 1. Web Controller (Wi-Fi Captive Portal)
1. Power up the Cat Bot.
2. On your phone or computer, connect to Wi-Fi network:
   - **SSID**: `Sesame-Controller`
   - **Password**: `12345678`
3. Open your browser and go to `http://192.168.4.1` or `http://sesame-robot.local`.
4. Control poses (`Stand`, `Rest`, `Wave`, `Dance`, `Cute`), walking directions, and individual motor sliders.

### 2. Animation Designer (Sesame Studio)
- Located in `software/sesame-studio/`.
- Double-click **`run_studio.bat`** to start the visual joint animator.
- Adjust joint sliders, preview leg angles, and generate C++ code for new routines.

### 3. Python Command Line Controller
- Located in `software/`.
- Double-click **`run_controller.bat`** to launch an interactive terminal to send poses, walk commands, and face expressions via the robot's REST API.

---

## Project Structure

```
I:\cat-bot\
├── platformio.ini              # PlatformIO root project configuration
├── src/
│   ├── main.cpp                # Core robot firmware & REST server
│   ├── captive-portal.h        # Web remote control HTML/CSS/JS
│   ├── face-bitmaps.h          # OLED facial expressions
│   └── movement-sequences.h    # Poses (Stand, Rest, Wave, Dance, Walk)
├── software/
│   ├── sesame-studio/          # Visual animation creator (run_studio.bat)
│   ├── cat_bot_controller.py   # Python API controller
│   └── run_controller.bat      # 1-click launcher for Python controller
└── debugging/
    └── sesame-motor-tester.ino # Standalone motor calibration sketch
```
