#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if !defined(__i386__)
#error "Compile with ix86-elf compiler"
#endif

/* Hardware text mode colour constants */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color){
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str){
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

void terminal_initialize(void){
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY,
    VGA_COLOR_BLACK);
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++){
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color){
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y){
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

/* 
    For internal usage in terminal scrolling, takes the formatted
    VGA entries as stored in terminal_buffer as opposed to the
    above which takes raw char and color args and formats them
    before entry into the buffer.
 */
void terminal_putoldentryat(uint16_t entry, size_t x, size_t y){
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = entry;
}

void terminal_scroll(void){
    terminal_column = 0;
    terminal_row = 0;
    for (size_t y = 1; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++){
            const size_t index = y * VGA_WIDTH + x;
            //terminal_buffer[index] = vga_entry(' ', terminal_color);
            uint16_t prev_entry = terminal_buffer[index];
            terminal_putoldentryat(prev_entry, terminal_column, terminal_row);
            if (++terminal_column == VGA_WIDTH){
                terminal_column = 0;
                if (++terminal_row == VGA_HEIGHT)
                    terminal_row = 0;
            }
        }
    }
}

void terminal_putchar(char c){
    /* Handle newline */
    if (c == '\n'){
        if (terminal_row + 1 == VGA_HEIGHT) {
            terminal_column = 0;
            terminal_scroll();
        } else {
            terminal_row++;
            terminal_column = 0;
        }
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH){
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_scroll();
    }
}

void terminal_write(const char* data, size_t size){
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char* data){
    terminal_write(data, strlen(data));
}

/* Stole this from a class I took last year */
void itoa(char *buf, int base, int d){
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;

    if (base == 'd' && d < 0) {
        *p++ = '-';
        buf++;
        ud = -d;
    }
    else if (base == 'x') {
        divisor = 16;
    }

    do {
        int remainder = ud % divisor;
        *p++ = (remainder < 10) 
                    ? remainder + '0' 
                    : remainder + 'a' - 10;
    } while (ud /= divisor);

    *p = '\0';

    p1 = buf;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void test_terminal(void){
    for (int i = 0; i < 30; i++){
        char istr;
        itoa(&istr, 10, i);
        terminal_writestring(&istr);
        terminal_writestring("\n");
    }
}

void kernel_main(void){
    terminal_initialize();
    test_terminal();
    //for (int i = 0; i < 25; i++) {
    //    terminal_writestring("Wuss poppin jit \n");
    //}
    //for (int j = 0; j < 10; j++)
    //    terminal_writestring("okay and this out da bounds \n");
}