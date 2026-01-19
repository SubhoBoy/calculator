// #include <LiquidCrystal.h>
// LiquidCrystal lcd(2,3,4,5,6,7);

// void setup() {
//   // put your setup code here, to run once:
// lcd.begin(16,2);
// lcd.clear();
// lcd.print("Yoooo");
//         lcd.clear();
//         lcd.print("Button");
//         lcd.clear();
//         lcd.print("Button");
// pinMode(8,INPUT_PULLUP);
// pinMode(9,INPUT_PULLUP);
// pinMode(10,INPUT_PULLUP);
// pinMode(11,INPUT_PULLUP);
// pinMode(12,INPUT_PULLUP);
// pinMode(A0,OUTPUT);
// pinMode(A1,OUTPUT);
// pinMode(A2,OUTPUT);
// pinMode(A3,OUTPUT);
// pinMode(A4,OUTPUT);
// Serial.begin(9600);
// }

// void loop() {
//   int t = 5;
//   while(t--) {
//     analogWrite(A0,1023);
//     int n = 5;
//     while(n--) {
//       if (digitalRead(8+n)) {
//         // lcd.clear();
//         int num = t*5+n+1;
//         // lcd.print("Button");
//         Serial.println("Button "+String(num));
//         delay(100);
//         }
//     }
//     analogWrite(A0,0);
//   }
// }

#include <LiquidCrystal.h>

// LCD is connected to 2, 3, 4, 5, 6, 7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// --- PIN CONFIGURATION ---
// User: "Digital pins are my rows, Analog pins are my columns"
// We need 5 Rows and 5 Columns for a 5x5 Matrix

// Rows (Outputs): Digital 8, 9, 10, 11, 12
const int rowPins[5] = {8, 9, 10, 11, 12}; 

// Columns (Inputs): Analog A0, A1, A2, A3, A4
const int colPins[5] = {A0, A1, A2, A3, A4}; 

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Keypad Test");
  Serial.begin(9600);
  Serial.print("Keypad Test");

  // 1. Set ROWS as OUTPUTS and default them to HIGH
  for (int i = 0; i < 5; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH); 
  }

  // 2. Set COLUMNS as INPUTS with internal PULL-UP resistors
  // This is crucial: it keeps the pins HIGH when no button is pressed.
  for (int i = 0; i < 5; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
}

void loop() {
  // Loop through each ROW
  for (int r = 0; r < 5; r++) {
    
    // ACTIVATE Row: Pull it LOW
    digitalWrite(rowPins[r], LOW);
    
    // Check all COLUMNS
    for (int c = 0; c < 5; c++) {
      
      // If a Column reads LOW, it means the button connecting
      // Row 'r' and Col 'c' is pressed.
      if (digitalRead(colPins[c]) == LOW) {
        
        // Calculate a unique button number (1 to 25)
        int buttonNum = (r * 5) + c + 1;
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Button Pressed:");
        lcd.setCursor(0, 1);
        lcd.print(buttonNum);
        
        Serial.print("Row: ");
        Serial.print(r);
        Serial.print(" Col: ");
        Serial.print(c);
        Serial.println(" Pressed");

        // Small delay to prevent spamming
        delay(200); 
        lcd.clear();
        lcd.print("Keypad Test");
      }
    }
    
    // DEACTIVATE Row: Set it back to HIGH before moving to the next row
    digitalWrite(rowPins[r], HIGH);
  }
}