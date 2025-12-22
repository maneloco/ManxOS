#include "./drivers/video.h"

void main() {
    clear_screen();
    print("Kernel cargado con éxito.\n");
    print("Hola Mundo!");
    while(1);
}
