// Name : Musawir Kasi
   
//Task : BULB Control System Using Rotary Encoder

//NOTE : I have performed this task on 16*2 LCD (1602A) because 20*4 was not avaliable


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>

// Define LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address and size for 1602a

// Define Encoder Pins
Encoder rotaryEncoder(2, 3);
const int encoderSwitch = 4;  // Encoder button pin

// Define Relay Pins
const int relay1 = 8;
const int relay2 = 9;
const int relay3 = 10;

// Bulb States
bool bulbState[3] = {false, false, false};  // Initial state: OFF

// Variables for menu navigation
int menuIndex = 0;
long lastPosition = -999;

void setup() {
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.backlight();

  // Set Relay pins as OUTPUT
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  // Initialize Relays to OFF state
  digitalWrite(relay1, HIGH); // Relay HIGH means OFF for most modules
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);

  // Encoder switch
  pinMode(encoderSwitch, INPUT_PULLUP);

  // Display initial menu
  displayMenu();
}

void loop() {
  // Read rotary encoder position
  long newPosition = rotaryEncoder.read() / 4;  // Adjust based on encoder steps

  if (newPosition != lastPosition) {
    menuIndex = newPosition % 3;  // Wrap around the menu options
    displayMenu();
    lastPosition = newPosition;
  }

  // Check if encoder button is pressed
  if (digitalRead(encoderSwitch) == LOW) {
    toggleBulb(menuIndex);
    displayMenu();  // Update the menu after toggling
    delay(300);     // Debounce delay
  }
}

void displayMenu() {
  lcd.clear();

  if (menuIndex == 0 || menuIndex == 1) {
    // Show BULB 1 and BULB 2
    lcd.setCursor(0, 0);
    lcd.print("BULB 1: ");
    lcd.print(bulbState[0] ? "ON " : "OFF");

    lcd.setCursor(0, 1);
    lcd.print("BULB 2: ");
    lcd.print(bulbState[1] ? "ON " : "OFF");

    // Show selector
    lcd.setCursor(14, menuIndex);
    lcd.print("<-");
  } else if (menuIndex == 2) {
    // Show BULB 2 and BULB 3
    lcd.setCursor(0, 0);
    lcd.print("BULB 2: ");
    lcd.print(bulbState[1] ? "ON " : "OFF");

    lcd.setCursor(0, 1);
    lcd.print("BULB 3: ");
    lcd.print(bulbState[2] ? "ON " : "OFF");

    // Show selector
    lcd.setCursor(14, menuIndex - 1);
    lcd.print("<-");
  }
}

void toggleBulb(int index) {
  bulbState[index] = !bulbState[index];

  // Toggle relays
  if (index == 0) {
    digitalWrite(relay1, bulbState[0] ? LOW : HIGH);  // LOW = ON, HIGH = OFF
  } else if (index == 1) {
    digitalWrite(relay2, bulbState[1] ? LOW : HIGH);
  } else if (index == 2) {
    digitalWrite(relay3, bulbState[2] ? LOW : HIGH);
  }
}
