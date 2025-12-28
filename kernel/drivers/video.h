#ifndef VIDEO_H
#define VIDEO_H

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE 0x0f

typedef struct{
    char letra;
    char color;
}__attribute__((packed)) character;

void clear_screen();
void print_at(char* message, int row, int col);
void print(char* message);
void print_char(char c);
void scroll();
void update_cursor_hardware();
void print_int(int n);
void print_hex(int n);


#endif
