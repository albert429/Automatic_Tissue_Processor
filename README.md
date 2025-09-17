
# 🧪 Automatic Tissue Processor (ATP) – Embedded System  

## 📖 Overview  
The **Automatic Tissue Processor (ATP)** is a medical laboratory device used in **pathology labs** to process biopsy tissues for further examination.  
This project implements a **cost-effective embedded solution** using a PIC16F877A microcontroller, **RTC module**, **LCD**, **keypad interface**, and **EEPROM-based configuration**.  

It features:  
- Real-time clock tracking and scheduling of processing cycles.  
- Recovery mechanism after unexpected **power outages**.  
- Configurable time schedules for each tissue beaker.  
- User interaction via a **4x3 matrix keypad** and **LCD menus**.  

---

## ⚙️ Hardware Components  
- **PIC16F877A** Microcontroller  
- **16x2 LCD Display**  
- **4x3 Keypad Matrix**  
- **RTC (DS1307 / compatible)** via I2C  
- **EEPROM** (internal) for saving settings & recovery states  
- **Relay / Motor drivers** for mechanical control  
- **Power outage detection circuit** (connected to `POWER_OUT_PIN`)  

---

## 🖥️ Software Features  
1. **Real-Time Clock Management**  
   - Uses I2C to interface with RTC (time & date tracking).  
   - Time displayed in `HH:MM:SS` format on LCD.  

2. **Menu System (via LCD + Keypad)**  
   - **Main Menu:** Start, Test, Time, Configure.  
   - **Start Mode:** Runs the processing sequence for selected beakers.  
   - **Test Mode:** Manual control for movement (UP, DOWN, Next, Home, etc.).  
   - **Time Mode:** View or Set system date & time.  
   - **Configure Mode:**  
     - Auto-start scheduling.  
     - Per-beaker processing time configuration.  
     - View saved configurations.  

3. **Power Outage Handling**  
   - Detects power failure using input pin.  
   - Saves current state (`beaker`, `remaining time`) into EEPROM.  
   - Restores and continues automatically after power recovery.  

4. **EEPROM Persistence**  
   - Stores **auto-start time**.  
   - Stores **per-beaker time configurations**.  
   - Saves **system state** in case of failure.  

---

## 🔧 File Structure  
```
/ATP-Firmware
 ├── main.c             # Main firmware with menus & control logic
 ├── lcd.c              # LCD driver
 ├── rtc.c              # RTC read/write functions
 ├── README.md          # Documentation
```

---

## 🚀 How It Works  
1. **Startup**  
   - System initializes LCD & RTC.  
   - Displays main menu.  

2. **Processing Cycle**  
   - User selects **Start** → Beaker A begins.  
   - System calculates **required time** for each beaker using current RTC values + EEPROM config.  
   - Moves sequentially through beakers (`A → B → ... → M`).  

3. **During Power Outage**  
   - `POWER_OUT_PIN` detects failure.  
   - State (current beaker, remaining time) is written to EEPROM.  
   - On recovery, system restores from saved state and continues.  

---

## 🛠️ Development Environment  
- **Compiler:** CCS C for PIC (MPLAB also supported with minor adjustments)  
- **Target MCU:** PIC16F877A (20 MHz crystal)  
- **Communication:** I2C (RTC), RS232 (debugging), GPIO (LCD & Keypad)  

---

## 📋 Example LCD Screens  

**Main Menu**  
```
1:Start       3:Time
2:Test        4:Configure
```

**Start Menu**  
```
Beaker (A) Time:12:34:56
CupB:12:40:00   #:STOP
```

**Error Handling**  
```
POWER ERROR!
IN:0
```

---

## ✅ Key Contributions  
- **Embedded Software Design** (bare-metal C on PIC16F877A).  
- **Hardware Integration** (LCD, Keypad, RTC, EEPROM).  
- **System Resilience** (Power-out recovery).  
- **User-friendly interface** for lab technicians.  
