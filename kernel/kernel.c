#include "utils/utils.h"
#include "display/display.h"

void main() {
clear_screen();
    kprint("Hola, soy display.h y funciono perfectamente");
    
    char buffer[10];
    int_to_ascii(1234, buffer);
    kprint_at(buffer, 0, 1);
}
