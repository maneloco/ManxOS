#ifndef UTIL_H
#define UTIL_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

void memory_copy(char *source, char *dest, int n_bytes);

int strlen(char s[]);

void reverse(char s[]);

void int_to_ascii(int n, char str[]);

#endif
