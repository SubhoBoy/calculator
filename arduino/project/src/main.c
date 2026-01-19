#include "defines.h"
#include "timer.h"
#include "lcd.h"
#include "pars.h" // Only pars.h is needed for eval_rpn
#include <stdio.h> // For sprintf

int main(void) {
    timer_init();
    lcd_init();

    lcd_print("Eval RPN Test");
    delay_ms(1000);
    lcd_clear();

    // Hardcoded RPN for "1 1 +"
    char rpn_test[3][10] = {"1", "1", "+"};
    int n_test = 3;
    double var_val_test = 0; // Not used in this simple test

    double result = eval_rpn(rpn_test, n_test, var_val_test);

    char result_str[16];
    sprintf(result_str, "Res:%.6f", result);
    lcd_print(result_str);

    while (1) {
        // Loop forever to display the result
    }

    return 0;
}