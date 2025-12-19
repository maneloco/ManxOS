#include "display.h"

void clear_screen(){
    // Se apunta al principio de la memoria de vídeo como un array de strings
    struct vga_char* screen = (struct vga_char*) VIDEO_ADDRESS;
    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++) {
        screen[i].character = ' ';
        screen[i].color = WHITE;
    }
}

void kprint_at(char *message, int col, int row){
    struct vga_char* screen = (struct vga_char*) VIDEO_ADDRESS;

    int i = row * MAX_COLS + col;
    int j = 0;
    while(message[j] != 0) {
        screen[i].character = message[j];
        screen[i].color = WHITE;
        j++;
        i++;
    }
}

void kprint(char *message){
    kprint_at(message, 0, 0); // Luego hay que cambiar esto con una variable global para saber donde está el puntero.
}

