#ifndef UTILS_H
#define UTILS_H

typedef unsigned int   uint32_t;
typedef          int   int32_t;
typedef unsigned short uint16_t;
typedef          short int16_t;
typedef unsigned char  uint8_t;
typedef          char  int8_t;

int strlen(char* string);
void reverse(char* string);
void int_to_ascii(int num, char* output);
void hex_to_ascii(int num, char* str);

void memory_copy(char* source, char* dest, int nbytes);
void memory_set(uint8_t* dest, uint8_t val, uint32_t len);

#endif
