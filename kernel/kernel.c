void main() {
    unsigned char* video_memory = (unsigned char*) 0xb8000;
    video_memory[0] = 'A';
    video_memory[1] = 0x0f; // Para pintar de blanco
    while(1);
}
