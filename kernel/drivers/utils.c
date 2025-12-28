#include "utils.h"

// Copia bloques de memoria de un lugar a otro
// Fundamental para mover las filas de texto en el scroll de pantalla
void memory_copy(char* source, char* dest, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

// Rellena un bloque de memoria con un valor específico
// Útil para limpiar buffers o inicializar estructuras a cero
void memory_set(uint8_t* dest, uint8_t val, uint32_t len) {
    uint8_t* temp = (uint8_t*)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

// Calcula la longitud de una cadena de caracteres
int strlen(char* s) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

// Invierte una cadena "in-place" (modifica la original)
// Se usa principalmente después de convertir números a texto
void reverse(char* s) {
    int c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

// Convierte un número entero (con signo) a cadena ASCII
void int_to_ascii(int n, char* str) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    
    // Algoritmo: sacar el último dígito con % 10 repetidamente
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    // Como los dígitos salen al revés (123 -> "321"), invertimos
    reverse(str);
}

// Convierte un número a formato hexadecimal (ej: 0x1A)
// Vital para depurar direcciones de memoria
void hex_to_ascii(int n, char* str) {
    str[0] = '0';
    str[1] = 'x';
    int zeros = 0;
    int index = 2; // Escribimos después del "0x"

    int tmp;
    // Iteramos de 4 en 4 bits desde la izquierda (28 bits desplazamiento)
    for (int i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue; // Ignoramos ceros a la izquierda
        zeros = 1;
        if (tmp >= 0xA) str[index++] = tmp - 0xA + 'a';
        else str[index++] = tmp + '0';
    }

    // Último dígito (siempre se imprime, aunque sea 0)
    tmp = n & 0xF;
    if (tmp >= 0xA) str[index++] = tmp - 0xA + 'a';
    else str[index++] = tmp + '0';
    
    str[index] = '\0';
}
