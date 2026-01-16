#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 100

// Stack utils
typedef struct { char data[MAX][10]; int top; } S_Str;
typedef struct { double data[MAX]; int top; } S_Dbl;

void push_s(S_Str *s, char *v) { strcpy(s->data[++s->top], v); }
char* pop_s(S_Str *s) { return s->data[s->top--]; }
char* peek_s(S_Str *s) { return s->data[s->top]; }

void push_d(S_Dbl *s, double v) { s->data[++s->top] = v; }
double pop_d(S_Dbl *s) { return s->data[s->top--]; }

int prec(char *op) { // [cite: 112]
    if (!strcmp(op, "^")) return 4;
    if (!strcmp(op, "*") || !strcmp(op, "/")) return 3;
    if (!strcmp(op, "+") || !strcmp(op, "-")) return 2;
    return 0;
}

int is_func(char *t) {
    return !strcmp(t,"sin") || !strcmp(t,"cos") || !strcmp(t,"ln");
}

// --- Shunting Yard Algorithm --- [cite: 67-68]
void shunt(char *expr, char output[MAX][10], int *out_len) {
    S_Str ops = { .top = -1 };
    char *token = strtok(expr, " "); // Tokenize by space for simplicity
    *out_len = 0;

    while (token) {
        if (isdigit(token[0]) || (token[0]=='-' && isdigit(token[1]))) {
            strcpy(output[(*out_len)++], token); // Number -> Output
        } else if (is_func(token) || !strcmp(token, "(")) {
            push_s(&ops, token);                 // Func/( -> Stack
        } else if (!strcmp(token, ")")) {
            while (strcmp(peek_s(&ops), "("))    // Pop until (
                strcpy(output[(*out_len)++], pop_s(&ops));
            pop_s(&ops); // Pop (
            if (ops.top != -1 && is_func(peek_s(&ops))) 
                strcpy(output[(*out_len)++], pop_s(&ops)); // Pop func
        } else { // Operator
            while (ops.top != -1 && prec(peek_s(&ops)) >= prec(token))
                strcpy(output[(*out_len)++], pop_s(&ops));
            push_s(&ops, token);
        }
        token = strtok(NULL, " ");
    }
    while (ops.top != -1) strcpy(output[(*out_len)++], pop_s(&ops));
}

//--- RPN Evaluator --- [cite: 127]
double eval(char rpn[MAX][10], int len) {
    S_Dbl val = { .top = -1 };
    for (int i = 0; i < len; i++) {
        char *t = rpn[i];
        if (isdigit(t[0]) || (t[0]=='-' && isdigit(t[1]))) {
            push_d(&val, atof(t));
        } else if (is_func(t)) {
            double a = pop_d(&val);
            if (!strcmp(t, "sin")) push_d(&val, m_sin(a));
            else if (!strcmp(t, "cos")) push_d(&val, m_cos(a));
            else if (!strcmp(t, "ln"))  push_d(&val, m_ln(a));
        } else {
            double b = pop_d(&val);
            double a = pop_d(&val); // Binary ops pop 2
            if (!strcmp(t, "+")) push_d(&val, a + b);
            else if (!strcmp(t, "-")) push_d(&val, a - b);
            else if (!strcmp(t, "*")) push_d(&val, a * b);
            else if (!strcmp(t, "/")) push_d(&val, a / b);
            else if (!strcmp(t, "^")) push_d(&val, m_pow(a, b));
        }
    }
    return pop_d(&val);
}
