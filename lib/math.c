#include "../include/math.h"

double quadrato(double x) {
    return x * x;
}

double radice(double x) {
    if (x < 0) return 0;
    double guess = x;
    double epsilon = 0.000001;
    while ((guess * guess - x) > epsilon || (x - guess * guess) > epsilon) {
        guess = 0.5 * (guess + x / guess);
    }
    return guess;
}

double seno(double x) {
    // Porta x nell'intervallo [0,360)
    while (x < 0) x += 360;
    while (x >= 360) x -= 360;

    if (x <= 90) return x / 90.0;
    if (x <= 180) return (180 - x) / 90.0;
    if (x <= 270) return (x - 180) / -90.0;
    return (360 - x) / -90.0;
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