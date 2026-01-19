#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include <LiquidCrystal.h>

// External declarations for functions from pars.c and func.c
// These will need to be linked during compilation.
extern void compute(const char* input, double start, double end, double step, char* output);
extern double m_sin(double x);
extern double m_cos(double x);
extern double m_tan(double x);
extern double m_ln(double x);
extern double m_pow(double x, double w);
extern double m_fact(double n);
extern double m_sqrt(double n);


// --- PIN CONFIGURATION (from Calc.ino) ---
const int rowPins[5] = {8, 9, 10, 11, 12}; 
const int colPins[5] = {A0, A1, A2, A3, A4}; 

// --- Keypad Mappings (from map.json) ---
// Mode 1
const char* keypad_mode_1[5][5] = {
  {"0", "1", "2", "3", "4"},
  {"5", "6", "7", "8", "9"},
  {"+", "-", "*", "/", "sin("},
  {"cos(", "tan(", "e^(", "ln(", "Clear"},
  {"Backspace", ".", "=", "Mode shift", "PI"} // Changed "π" to "PI" for easier C handling
};

// Mode 2
const char* keypad_mode_2[5][5] = {
  {"0", "1", "2", "3", "4"},
  {"5", "6", "7", "8", "9"},
  {"(", ")", "^", "fact(", "arcsin("},
  {"arccos(", "arctan(", "mod", "log10(", "Clear"},
  {"Backspace", ".", "=", "Mode shift", "PI"} // Changed "π" to "PI" for easier C handling
};

// --- Global Variables ---
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // LCD object (from Calc.ino)
int currentMode = 1; // Start in Mode 1
#define MAX_EXPRESSION_LEN 256 // Max length for expression buffer (from pars.c MAX)
char expressionBuffer[MAX_EXPRESSION_LEN];
int bufferIndex = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200; // milliseconds

// --- Function Prototypes ---
void input_setup();
void input_loop();
void update_lcd_display(const char* text);
void handle_key_press(int row, int col);

#endif // INPUT_H
