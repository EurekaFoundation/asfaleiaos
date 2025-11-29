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

double pow(double base, int esponente) {
    double risultato = 1.0;

    if (esponente == 0) return 1.0;

    int e = esponente;
    int negative = 0;
    if (e < 0) { negative = 1; e = -e; }

    for (int i = 0; i < e; i++) {
        risultato *= base;
    }

    if (negative) {
        // handle division by zero naturally (returns +inf or NaN)
        return 1.0 / risultato;
    }
    return risultato;
}

double log(double x) {
    if (x <= 0.0) {
        // Il logaritmo per x <= 0 non è definito. Imposta errno come fa math.h
       // errno = EDOM; // Errore di dominio
        return 0.0 / 0.0; // Restituisce NaN (Not a Number)
    }

    // Normalizza x nell'intervallo [0.5, 1.5] usando le proprietà dei logaritmi
    int exponent = 0;
    while (x > 1.5) { // Se x è troppo grande, dividi per 2 e incrementa l'esponente
        x /= 2.0;
        exponent++;
    }
    while (x < 0.5) { // Se x è troppo piccolo, moltiplica per 2 e decrementa l'esponente
        x *= 2.0;
        exponent--;
    }

    // Applica la serie di Mercator per ln(1 + t) dove t = x - 1
    // La formula usata qui è per ln((1+y)/(1-y)) per una convergenza più rapida
    // o semplicemente ln(x) = 2 * ( (x-1)/(x+1) + 1/3 * ((x-1)/(x+1))^3 + ... )
    double t = (x - 1.0) / (x + 1.0);
    double t2 = t * t;
    double sum = t;
    double term = t;

    // Itera fino a quando i termini diventano molto piccoli (precisione double)
    for (int k = 1; k < 1000; k += 2) {
        term *= t2;
        sum += term / (double)k;
    }

    // Risultato finale: 2 * sum + (componente esponente * ln(2))
    return 2.0 * sum + (double)exponent * 0.69314718056;
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