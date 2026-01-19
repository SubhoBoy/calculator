#include "defines.h"
#include "timer.h"
#include "lcd.h"
#include <stdlib.h> // For atof
#include <stdio.h>  // For sprintf

int main(void) {
    timer_init();
    lcd_init();

    lcd_print("atof Test");
    delay_ms(1000);
    lcd_clear();

    double test_val = atof("1.23");

    char result_str[16];
    sprintf(result_str, "Val:%.6f", test_val);
    lcd_print(result_str);

    while (1) {
        // Loop forever to display the result
    }

    return 0;
}
