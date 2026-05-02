# AutoCad-Electrical
This system is designed to calculate the required number of lighting fixtures based on room dimensions and standard illumination levels. It also estimates electrical load, current, and suggests appropriate wiring size for residential applications.
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
  Serial.println("\nSelect Room Type:");
  Serial.println("1. Bedroom (100 lux)");
  Serial.println("2. Drawing Room (150 lux)");
  Serial.println("3. Dining Room (150 lux)");
  Serial.println("4. Kitchen (160 lux)");
  Serial.println("5. Toilet (100 lux)");
  Serial.println("6. Passage & stairs (80 lux)");
  Serial.println("7. Balcony (60 lux)");
  Serial.println("8. Outside (40 lux)\n");
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
      case 1: E_TARGET = 100; Serial.println("Bedroom selected (100 lux)\n"); break;
      case 2: E_TARGET = 150; Serial.println("Drawing Room selected (150 lux)\n"); break;
      case 3: E_TARGET = 150; Serial.println("Dining Room selected (150 lux)\n"); break;
      case 4: E_TARGET = 160; Serial.println("Kitchen selected (160 lux)\n"); break;
      case 5: E_TARGET = 100; Serial.println("Toilet selected (100 lux)\n"); break;
      case 6: E_TARGET = 80; Serial.println("Passage & stairs selected (80 lux)\n"); break;
      case 7: E_TARGET = 60; Serial.println("Balcony selected (60 lux)\n"); break;
      case 8: E_TARGET = 40; Serial.println("Outside selected (40 lux)\n"); break;
      default: Serial.println("Invalid! Enter 1-8:\n"); return;
    }
    
    Serial.println("Enter Length (X'Y\"):");
    inputState = 0;
  }
  else if (inputState == 0) {
    while (Serial.available() == 0) {}
    length_ft = Serial.parseInt();
    if (length_ft > 0) {
      inputState = 1;
    }
  }
  else if (inputState == 1) {
    while (Serial.available() == 0) {}
    length_in = Serial.parseFloat();
    if (length_in >= 0 && length_in <= 11.999) {
      length_m = (length_ft * 0.3048) + (length_in * 0.0254);
      Serial.println("\nEnter Width (X'Y\"):");
      inputState = 2;
    } else {
      Serial.println("Invalid! Enter inches (0 to 11.999):");
    }
  }
  else if (inputState == 2) {
    while (Serial.available() == 0) {}
    width_ft = Serial.parseInt();
    if (width_ft > 0) {
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

float calculateUF(float ri) {
  if (ri <= ri_values[0]) {
    float slope = (uf_values[1] - uf_values[0]) / (ri_values[1] - ri_values[0]);
    float uf = uf_values[0] - (ri_values[0] - ri) * slope;
    if (uf < 0.19) uf = 0.19;
    return uf;
  }
  
  if (ri >= ri_values[tableSize-1]) {
    float slope = (uf_values[tableSize-1] - uf_values[tableSize-2]) / (ri_values[tableSize-1] - ri_values[tableSize-2]);
    float uf = uf_values[tableSize-1] + (ri - ri_values[tableSize-1]) * slope;
    if (uf > 0.85) uf = 0.85;
    return uf;
  }
  
  for (int i = 0; i < tableSize-1; i++) {
    if (ri >= ri_values[i] && ri <= ri_values[i+1]) {
      float uf = uf_values[i] + (ri - ri_values[i]) * (uf_values[i+1] - uf_values[i]) / (ri_values[i+1] - ri_values[i]);
      return uf;
    }
  }
  return 0.30;
}

void calculateAndShow() {
  float hm = 2.0;
  float ri = (length_m * width_m) / (hm * (length_m + width_m));
  float uf = calculateUF(ri);
 
  float area = length_m * width_m;
  float N_float = (E_TARGET * area) / (n * F * uf * LLF);
  int N_int = (int)N_float;
  if (N_float > N_int) N_int++;
  
  float N_rounded = (float)((int)(N_float * 100 + 0.5)) / 100;
  
  float area_sqft = area * 10.764;
  
  int fans = 0;
  int exhaust_fans = 0;
  
  if (roomType == 5) {
    exhaust_fans = 1;
  } else if (roomType == 4) {
    exhaust_fans = 2;
  } else if (roomType == 6 || roomType == 7 || roomType == 8) {
    fans = 0;
    exhaust_fans = 0;
  } else {
    fans = (int)(area_sqft / 100.0);
    if ((area_sqft / 100.0) > fans) fans++;
  }
  
  int bulbs = 0;
  int tubelight = 0;
  
  if (roomType == 4 || roomType == 5 || roomType == 7 || roomType == 8) {
    bulbs = N_int;
    tubelight = 0;
  } else {
    if (N_float <= 1.5) {
      tubelight = 1;
      bulbs = 0;
    } else if (N_float <= 2.5) {
      tubelight = 1;
      bulbs = 2;
    } else if (N_float <= 3.5) {
      tubelight = 2;
      bulbs = 2;
    } else {
      tubelight = (int)(N_float / 2 + 0.5);
      bulbs = (int)(N_float / 2 + 0.5);
    }
  }
  
  Serial.println("\n----------------------------------------");
  Serial.print("Length = ");
  Serial.print(length_ft);
  Serial.print("' ");
  Serial.print(length_in);
  Serial.print("\" = ");
  Serial.print(length_m, 2);
  Serial.println(" m");
  
  Serial.print("Width = ");
  Serial.print(width_ft);
  Serial.print("' ");
  Serial.print(width_in);
  Serial.print("\" = ");
  Serial.print(width_m, 2);
  Serial.println(" m");
  
  Serial.print("Area = ");
  Serial.print(area, 2);
  Serial.println(" m²");
  
  Serial.print("Room Index = ");
  Serial.println(ri, 3);
  
  Serial.print("UF = ");
  Serial.println(uf, 4);
  
  Serial.print("Illuminance = ");
  Serial.print(E_TARGET);
  Serial.println(" lux");
  
  Serial.print("N = ");
  Serial.println(N_rounded, 2);
  
  if (roomType == 4 || roomType == 5 || roomType == 7 || roomType == 8) {
    Serial.print("Light: ");
    Serial.print(bulbs);
    Serial.println(" bulbs");
  } else {
    Serial.print("Light: ");
    Serial.print(tubelight);
    Serial.print(" tubelight + ");
    Serial.print(bulbs);
    Serial.println(" bulbs");
  }
  
  if (roomType == 5) {
    Serial.println("Fan: 1 exhaust fan");
  } else if (roomType == 4) {
    Serial.println("Fan: 2 exhaust fans");
  } else if (roomType == 6 || roomType == 7 || roomType == 8) {
    Serial.println("Fan: none");
  } else {
    Serial.print("Fan: ");
    Serial.print(fans);
    Serial.println(" ceiling fans");
  }
  Serial.println("----------------------------------------");
}
```

---

## 👨‍💻 Author

MD Emon
EEE Student | Robotics & VLSI Enthusiast

---

## ⭐ Support

If you like this project, give it a ⭐ on GitHub!
