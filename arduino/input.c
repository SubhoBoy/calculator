#include "input.h"
#include <string.h> // For strcat, strcpy, strlen
#include <stdio.h>  // For sprintf

// --- Helper function to update LCD display ---
void update_lcd_display(const char* text) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(text);
}

// --- Setup function for input handling ---
void input_setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Calculator Ready");
  Serial.begin(9600);
  Serial.println("Calculator Ready");

  // Configure row pins as OUTPUT and set HIGH
  for (int i = 0; i < 5; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH); 
  }

  // Configure column pins as INPUT_PULLUP
  for (int i = 0; i < 5; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }

  // Initialize expression buffer
  expressionBuffer[0] = '\0'; 
  bufferIndex = 0;
}

// --- Handle individual key presses ---
void handle_key_press(int row, int col) {
  const char* key;
  if (currentMode == 1) {
    key = keypad_mode_1[row][col];
  } else {
    key = keypad_mode_2[row][col];
  }

  Serial.print("Key Pressed: ");
  Serial.println(key);

  if (strcmp(key, "Clear") == 0) {
    expressionBuffer[0] = '\0';
    bufferIndex = 0;
    update_lcd_display("Cleared");
  } else if (strcmp(key, "Backspace") == 0) {
    if (bufferIndex > 0) {
      expressionBuffer[--bufferIndex] = '\0';
      update_lcd_display(expressionBuffer);
    } else {
      update_lcd_display("Empty");
    }
  } else if (strcmp(key, "Mode shift") == 0) {
    currentMode = (currentMode == 1) ? 2 : 1;
    char modeMsg[16];
    sprintf(modeMsg, "Mode %d", currentMode);
    update_lcd_display(modeMsg);
  } else if (strcmp(key, "=") == 0) {
    if (bufferIndex > 0) {
      char resultBuffer[MAX_EXPRESSION_LEN]; // Buffer for the result
      // Replace "PI" with actual value for computation
      char tempExpression[MAX_EXPRESSION_LEN];
      strcpy(tempExpression, expressionBuffer);
      char* p = tempExpression;
      while ((p = strstr(p, "PI")) != NULL) {
          memmove(p + 2, p + 2, strlen(p + 2) + 1); // Shift remaining string
          strncpy(p, "3.14159", 7); // Insert PI value
          p += 7; // Move past the inserted value
      }

      compute(tempExpression, 0, 0, 0, resultBuffer); // Call parser's compute function
      update_lcd_display(resultBuffer);
      Serial.print("Result: ");
      Serial.println(resultBuffer);
      // After displaying result, you might want to clear the buffer or keep the result
      // For now, let's clear it for a new input.
      expressionBuffer[0] = '\0';
      bufferIndex = 0;
    } else {
      update_lcd_display("No input");
    }
  } else {
    // Append character to buffer if space is available
    if (bufferIndex < MAX_EXPRESSION_LEN - 1) {
      // Handle multi-character keys like "sin("
      int keyLen = strlen(key);
      if (bufferIndex + keyLen < MAX_EXPRESSION_LEN) {
        strcpy(&expressionBuffer[bufferIndex], key);
        bufferIndex += keyLen;
        update_lcd_display(expressionBuffer);
      } else {
        update_lcd_display("Buffer Full!");
      }
    } else {
      update_lcd_display("Buffer Full!");
    }
  }
}

// --- Main loop function for input handling ---
void input_loop() {
  // Loop through each ROW
  for (int r = 0; r < 5; r++) {
    digitalWrite(rowPins[r], LOW); // Activate Row

    // Check all COLUMNS
    for (int c = 0; c < 5; c++) {
      if (digitalRead(colPins[c]) == LOW) { // Button pressed
        if ((millis() - lastDebounceTime) > debounceDelay) {
          handle_key_press(r, c);
          lastDebounceTime = millis();
        }
      }
    }
    digitalWrite(rowPins[r], HIGH); // Deactivate Row
  }
}
