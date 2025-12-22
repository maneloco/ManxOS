#include "video.h"
#include "io.h"

int cursor_row = 0;
int cursor_col = 0;

void clear_screen(){
    character* video_memory = (character*) VIDEO_ADDRESS;
    for(int i = 0; i < MAX_ROWS*MAX_COLS; i++){
        video_memory[i].letra = ' ';
        video_memory[i].color = WHITE;
    }

    cursor_row = 0;
    cursor_col = 0;
}

void print(char* message){
    int i = 0;
    while (message[i] != '\0') {
        print_char(message[i]);
        i++;
    }
}

void print_char(char c) {
    character* video_memory = (character*) VIDEO_ADDRESS;

    if (c == '\n'){
        cursor_col = 0;
        cursor_row++;
    } else {
        int offset = cursor_row * MAX_COLS + cursor_col;
        video_memory[offset].letra = c;
        video_memory[offset].color = WHITE;
        cursor_col++;
    }
    
    if (cursor_col >= MAX_COLS) {
        cursor_col = 0;
        cursor_row++;
    }

    if (cursor_row >= MAX_ROWS) {
        scroll();
    }

    update_cursor_hardware();
}

void print_at(char* message, int row, int col){
    character* video_memory = (character*) VIDEO_ADDRESS;
    cursor_col = col;
    cursor_row = row;
    print(message);
}

void scroll() {
    character* video_memory = (character*) VIDEO_ADDRESS;
    
    for (int i = 1; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            int offset = (i * MAX_COLS) + j;
            video_memory[offset-MAX_COLS] = video_memory[offset];
        }
    }

    for (int j = 0; j < MAX_ROWS; j++) {
        video_memory[MAX_ROWS + j].letra = ' ';
        video_memory[MAX_ROWS + j].color = WHITE;
    }

    cursor_row = MAX_ROWS - 1;
}

void update_cursor_hardware() {
    // Calcula la posición del cursor y lo guarda en 8 bits sin signo
    unsigned short position = cursor_row * MAX_COLS + cursor_col;

    // Envía el byte alto (bits: 8-15) la posición
    outb(0x3D4, 0x0E); // Avisa al registro que va a enviar la parte alta
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));

    // Envía el byte bajo (bits: 0-7) la posición
    outb(0x3D4, 0x0F); // Avisa al registro de la VGA que va a enviar parte baja
    outb(0x3D5, (unsigned char)(position & 0xFF));
}

