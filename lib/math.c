#include "../include/math.h"

double quadrato(double x) {
    return x * x;
}

double radice(double x) {
    if (x < 0) return -1;
    double guess = x;
    double epsilon = 0.000001;
    while ((guess * guess - x) > epsilon || (x - guess * guess) > epsilon) {
        guess = 0.5 * (guess + x / guess);
    }
    return guess;
}

double pow(double base, int esponente) {
    double risultato = 1.0;
    
    for (int i; i < esponente; i++){
        risultato *= base;
    }
    return risultato;

}

double seno(double x) {
    // Convert degrees to radians
    x = DEG_TO_RAD(x);
    
    double term = x;
    double sum = x;
    double num = x;
    double fact = 1.0;
    int sign = 1;

    for (int i = 3; i <= 15; i += 2) {
        num *= x * x;
        fact *= i * (i - 1);
        sign = -sign;
        term = sign * num / fact;
        sum += term;
    }
    return sum;
}

double coseno(double x) {
    // Convert degrees to radians
    x = DEG_TO_RAD(x);
    
    double term = 1;
    double sum = 1;
    double num = 1;
    double fact = 1.0;
    int sign = 1;

    for (int i = 2; i <= 14; i += 2) {
        num *= x * x;
        fact *= i * (i - 1);
        sign = -sign;
        term = sign * num / fact;
        sum += term;
    }
    return sum;
}