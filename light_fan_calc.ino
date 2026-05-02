# 💡 Light and Fan Calculator (Arduino)

An Arduino-based system to calculate the required number of **lights and fans** for different room types using the **Lumen Method**.

---

## 📌 Overview

This project helps design efficient room lighting by calculating:

* 💡 Required number of lights
* 🌬️ Required number of ceiling/exhaust fans

It uses engineering concepts like **Room Index (RI)** and **Utilization Factor (UF)**.

---

## 📐 Formula Used

N = \frac{E \times A}{n \times F \times UF \times LLF}

---

## ⚙️ Features

* Multiple room types (Bedroom, Kitchen, etc.)
* Input in feet & inches
* Automatic unit conversion
* UF interpolation from table
* Smart light suggestion (bulb/tubelight)
* Fan & exhaust fan calculation
* Serial Monitor interface

---

## 🛠️ Hardware

* Arduino UNO
* USB Cable

---

## 💻 Software

* Arduino IDE

---

## 🚀 How to Use

1. Upload the code to Arduino
2. Open Serial Monitor (9600 baud)
3. Select room type (1–8)
4. Enter room dimensions
5. Get results instantly

---

## 💻 Arduino Code

```cpp
#include <math.h>

#define LLF 0.9
#define n 1
#define F 1800

float ri_values[] = {0.75, 1.00, 1.25, 1.50, 2.00, 2.50, 3.00, 4.00, 5.00};
float uf_values[] = {0.30, 0.37, 0.41, 0.45, 0.52, 0.57, 0.60, 0.65, 0.69};
int tableSize = 9;

float length_m = 0, width_m = 0;
int inputState = -2;  
int length_ft = 0, width_ft = 0;
float length_in = 0, width_in = 0;
float E_TARGET = 100;
int roomType = 1;

void showMenu() {
  Serial.println("\n========================================");
  Serial.println("   LIGHTING DESIGN CALCULATOR");
  Serial.println("========================================");
  Serial.println();
  Serial.println("Select Room Type:");
  Serial.println("1. Bedroom (100 lux)");
  Serial.println("2. Drawing Room (150 lux)");
  Serial.println("3. Dining Room (150 lux)");
  Serial.println("4. Kitchen (160 lux)");
  Serial.println("5. Toilet (100 lux)");
  Serial.println("6. Passage & stairs (80 lux)");
  Serial.println("7. Balcony (60 lux)");
  Serial.println("8. Outside (40 lux)");
  Serial.println();
  Serial.println("Enter 1-8:");
}

void setup() {
  Serial.begin(9600);
  showMenu();
}

void loop() {
  if (inputState == -2) {
    while (Serial.available() == 0) {}
    roomType = Serial.parseInt();
    
    switch(roomType) {
      case 1: E_TARGET = 100; Serial.println("Bedroom selected (100 lux)"); break;
      case 2: E_TARGET = 150; Serial.println("Drawing Room selected (150 lux)"); break;
      case 3: E_TARGET = 150; Serial.println("Dining Room selected (150 lux)"); break;
      case 4: E_TARGET = 160; Serial.println("Kitchen selected (160 lux)"); break;
      case 5: E_TARGET = 100; Serial.println("Toilet selected (100 lux)"); break;
      case 6: E_TARGET = 80; Serial.println("Passage & stairs selected (80 lux)"); break;
      case 7: E_TARGET = 60; Serial.println("Balcony selected (60 lux)"); break;
      case 8: E_TARGET = 40; Serial.println("Outside selected (40 lux)"); break;
      default: Serial.println("Invalid! Enter 1-8:"); return;
    }
    
    Serial.println("\nEnter Length (feet):");
    inputState = 0;
  }
  else if (inputState == 0) {
    while (Serial.available() == 0) {}
    length_ft = Serial.parseInt();
    if (length_ft > 0) {
      Serial.println("Enter Length (inches):");
      inputState = 1;
    }
  }
  else if (inputState == 1) {
    while (Serial.available() == 0) {}
    length_in = Serial.parseFloat();
    if (length_in >= 0 && length_in <= 11.999) {
      length_m = (length_ft * 0.3048) + (length_in * 0.0254);
      Serial.println("\nEnter Width (feet):");
      inputState = 2;
    } else {
      Serial.println("Invalid! Enter inches (0 to 11.999):");
    }
  }
  else if (inputState == 2) {
    while (Serial.available() == 0) {}
    width_ft = Serial.parseInt();
    if (width_ft > 0) {
      Serial.println("Enter Width (inches):");
      inputState = 3;
    }
  }
  else if (inputState == 3) {
    while (Serial.available() == 0) {}
    width_in = Serial.parseFloat();
    if (width_in >= 0 && width_in <= 11.999) {
      width_m = (width_ft * 0.3048) + (width_in * 0.0254);
      calculateAndShow();
      
      length_ft = width_ft = 0;
      length_in = width_in = 0;
      length_m = width_m = 0;
      
      Serial.println("\nEnter 39 to calculate again:");
      inputState = -3;
    } else {
      Serial.println("Invalid! Enter inches (0 to 11.999):");
    }
  }
  else if (inputState == -3) {
    while (Serial.available() == 0) {}
    int val = Serial.parseInt();
    if (val == 39) {
      showMenu();
      inputState = -2;
    } else {
      Serial.println("Enter 39 to calculate again:");
    }
  }
}
```

---

## 👨‍💻 Author

MD Emon
EEE Student 

---

## ⭐ Support

If you like this project, give it a ⭐ on GitHub!
