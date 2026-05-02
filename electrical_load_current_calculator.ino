# Load & Current Calculator (Arduino)

An Arduino-based system to calculate **electrical load and current** for different room types.
This project also provides **wire size recommendations** based on calculated current.

---

## Project Overview

This calculator helps estimate:

* Total electrical load (Watt)
* Current consumption (Ampere)
* wire size for safe installation

It is useful for **basic electrical design, load estimation, and wiring planning**.

---

##  Features

*  Room type selection (Bedroom, Kitchen, etc.)
*  Auto appliance setup (fans, exhaust, sockets)
*  Manual input for lights and fans
*  Total load calculation
*  Current calculation using power formula
*  Wire size recommendation
*  Repeat calculation option

---

##  Formula Used

### Current Calculation

I = \frac{P}{V \times pf}

Where:

* **I** = Current (Ampere)
* **P** = Total Power (Watt)
* **V** = Voltage (220V)
* **pf** = Power factor (0.7 assumed)

---

##  Hardware Requirements

* Arduino UNO
* USB Cable
* PC/Laptop

---

##  Software Requirements

* Arduino IDE

---

##  How to Use

1. Upload the code to Arduino
2. Open Serial Monitor (9600 baud)
3. Select room type (1–8)
4. Enter number of appliances
5. View:

   * Total load (W)
   * Current (A)
   * Recommended wire size

---

##  Arduino Code

```cpp id="p9x2kf"
#include <math.h>

int roomType = 1;

#define BULB_W 20
#define TUBE_W 20
#define CEILING_FAN_W 100
#define EXHAUST_FAN_W 40
#define SOCKET_2PIN_W 100

int ceiling_fans = 0;
int exhaust_fans = 0;
int tubelight = 0;
int bulbs = 0;
int socket_2pin = 0;

int step = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("========================================");
  Serial.println("   LOAD & CURRENT CALCULATOR");
  Serial.println("========================================\n");
  Serial.println("Select Room Type:");
  Serial.println("1. Bedroom");
  Serial.println("2. Drawing Room");
  Serial.println("3. Dining Room");
  Serial.println("4. Kitchen");
  Serial.println("5. Toilet");
  Serial.println("6. Passage & stairs");
  Serial.println("7. Balcony");
  Serial.println("8. Outside\n");
  Serial.println("Enter 1-8:");
}

void loop() {
  if (step == 0) {
    while (Serial.available() == 0) {}
    roomType = Serial.parseInt();
    
    if (roomType >= 1 && roomType <= 8) {
      exhaust_fans = 0;
      socket_2pin = 0;
      ceiling_fans = 0;
      tubelight = 0;
      bulbs = 0;
      
      if (roomType == 4) {
        exhaust_fans = 2;
        socket_2pin = 1;
        Serial.println("\nKitchen selected - Auto: 2 Exhaust Fans, 1x 2pin Socket");
        Serial.println("\nEnter number of Tube Lights (0-10):");
        step = 2;
      } 
      else if (roomType == 5) {
        exhaust_fans = 1;
        Serial.println("\nToilet selected - Auto: 1 Exhaust Fan");
        Serial.println("\nEnter number of Tube Lights (0-10):");
        step = 2;
      }
      else if (roomType == 1 || roomType == 2 || roomType == 3) {
        socket_2pin = 2;
        Serial.println("\nBedroom/Drawing/Dining selected - Auto: 2x 2pin Socket");
        Serial.println("\nEnter number of Ceiling Fans (0-5):");
        step = 1;
      }
      else {
        Serial.println("\nPassage/Balcony/Outside selected - No auto fittings");
        Serial.println("\nEnter number of 2pin Sockets (0-5):");
        step = 5;
      }
    } else {
      Serial.println("Invalid! Enter 1-8:");
    }
  }
}
```

---

##  Output Includes

* Total Load (Watt)
* Current (Ampere)
* Wire Size Recommendation

---


