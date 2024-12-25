# Smart Lock Project 🛡️🔒

## Overview 📖
The Smart Lock Project is a secure, IoT-based locking system integrating facial recognition, motion detection, and remote control. Designed for homes and offices, this system uses an ESP32-CAM for camera-based features, MQTT protocol for communication, and Python for advanced facial recognition.

## Features ✨
- 📸 **Real-Time Video Streaming** via ESP32-CAM.
- 🔑 **Facial Recognition** for personalized access control.
- 🚨 **Motion Detection** with ultrasonic sensors and alerts.
- 🌐 **Remote Control** using MQTT commands for lock operation.
- 💡 **Dual Connectivity Indicators** for Wi-Fi and MQTT statuses.
- 🛠️ Fully customizable and extendable for smart home integration.

## System Components 🧩
- **ESP32-CAM**: Handles video streaming and servo motor control for the lock.
- **Raspberry Pi**: Acts as an MQTT broker and facial recognition processor.
- **Ultrasonic Sensor**: Detects motion and triggers alerts.
- **Servo Motor**: Controls the locking mechanism.
- **Buzzer**: Provides audible feedback for motion detection.
- **Python Scripts**: For face registration and recognition.

## Getting Started 🚀

### 1. Prerequisites 🛠️
- ESP32-CAM
- Raspberry Pi 4 (or any compatible board)
- Ultrasonic Sensor (HC-SR04)
- Servo Motor
- Buzzer
- Python (with OpenCV library installed)

### 2. Setup Steps ⚙️
1. Flash the ESP32-CAM with the provided code.
2. Deploy the MQTT broker on your Raspberry Pi.
3. Run the `register_faces.py` script to enroll faces.
4. Start the `facial_recognition.py` script for access control.

## How to Use 📚
1. **Power Up**: Connect the ESP32 and Raspberry Pi to power.
2. **Register Users**: Run the `register_faces.py` script to save authorized faces.
3. **Stream Video**: Access live footage via the ESP32-CAM IP.
4. **Control Lock**: Publish MQTT messages (`open` or `close`) to control the lock.

## Code Overview 🖥️

### ESP32-CAM Firmware 📡
Handles video streaming, motion detection, and MQTT integration for lock control.

### `register_faces.py` Script 📝
Captures and saves user faces into the system for authentication purposes.

### `facial_recognition.py` Script 🤖
Recognizes faces in real-time and provides access control feedback.

## Project Structure 📂
📁 SmartLockProject  
├── 📄 ESP32_Firmware.ino  
├── 📄 register_faces.py  
├── 📄 facial_recognition.py  
└── 📂 registered_faces  
