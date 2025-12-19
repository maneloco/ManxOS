#include "utils.h";

void memory_copy(char *source, char *dest, int n_bytes){
    for (int i = 0; i < n_bytes; i++){
        *(dest + i) = *(source + i);
    }
}

int srtlen(char s[]){
    int len = 0;
    while (s[len] != '\0') ++len;
    return len;
}

void reverse(char s[]){
    int c, i, j;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j++){
        c = s[i]
        s[i] = s[j]
        s[j] = c;
    }
}

void int_to_ascii(int n, char str[]){
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n/=10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}
