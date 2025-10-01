# RadarScope-ESP32

## ✨ Features
**Real-time Radar Visualization**  
Processing sketch renders a sweeping radar display with live blip detection.  

**ESP32 Human Detection Radar Interface**  
RD-03D radar module connected via UART, detects and tracks human targets only.  

**Distance & Angle Tracking**  
Targets are displayed as red blips with telemetry (distance in meters, angle in degrees).  

**Scalable Range Rings**  
Up to 5 meters range with calibrated rings and labels.  

**Modular Design**  
ESP32 handles radar data, Processing handles visualization.  

---

## 📂 Project Structure
RadarScope-ESP32/  
│── ProcessingRadar/  
│    └── RadarScope.pde       # Processing sketch for visualization  
│── ESP32_Firmware/  
│    └── radar_uart.ino       # ESP32 code for RD-03D data parsing & UART output  
└── README.txt  

---

## ⚙️ Requirements
**Hardware:** ESP32, RD-03D human presence radar module, USB cable  

**Software:**  
- Arduino IDE or PlatformIO (for ESP32 code)  
- Processing 4.x (for radar visualization)  

**Libraries:**  
- Arduino: `Arduino.h` (built-in)  
- Processing: `processing.serial.*`  

---

## 🔌 Wiring (ESP32 ↔ RD-03D)
- RX (GPIO16) ← TX of RD-03D  
- TX (GPIO17) → RX of RD-03D  
- GND ↔ GND  
- VCC → 5V  

---

## ▶️ Run
1. Flash the ESP32 with `radar_uart.ino`.  
2. Open Processing and run `RadarScope.pde`.  
3. Set the correct COM port in the code (`COM7` default).  
4. Watch human targets appear as radar blips in real time!  

---

## 📝 Notes
- Designed for **human detection only** — the RD-03D module filters non-human objects.  
- ESP32 task reads UART frames and decodes target distance/angle.  
- Processing GUI scales automatically to window size (default 800x420).  
- Targets out of range reset to `0`.  
- Optimized for **single target detection mode**.  

---

## 📡 About
RadarScope-ESP32 is a DIY real-time **human detection radar visualizer** using an ESP32 and RD-03D radar module.  
It combines embedded UART parsing with Processing graphics to create a retro-style sweeping radar screen focused on human presence tracking.  
