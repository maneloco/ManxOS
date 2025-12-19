#ifndef DISPLAY_H
#define DISPLAY_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

struct vga_char {
    uint8_t character; // Carácter ASCII
    uint8_t color;
}__attribute__((packed));

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define WHITE 0x07

void clear_screen();
void kprint_at(char *message, int col, int row);
void kprint(char *message);

#endif
