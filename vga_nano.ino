#include <VGAX.h>
#include "./fonts/bigfont.h"
#include "./fonts/smallfont.h"

VGAX vga;

// color constants
const int background = 0;
const int foreground1 = 1;
const int foreground2 = 2;
const int foreground3 = 3;

// placeholders
char str0[] = "3172W";
char str1[] = "50.7Hz";
char str2[] = "25.6\"C";
char str3[] = "3.5Bar";

// color threshold int
const int str0_th = 2000;
const int str1_th = 62;
const int str2_th = 45;
const int str3_th = 3;

// data input buffer
const char buffer_size = 20;
volatile int buffer_i = 0;
volatile bool completed = false;
char buffer[buffer_size];

void setup()
{
    vga.begin();
    vga.clear(background);
    Serial.begin(9600);
    Serial.println("...start");
}

void print_value(char text[], int color, int x, int y)
{
    vga.printSRAM((byte *)bigfont_data, bigfont_symbols, bigfont_height, 3, 1, text, x, y, color);
}

void print_label(char text[], int color, int x, int y)
{
    vga.printSRAM((byte *)smallfont_data, smallfont_symbols, smallfont_height, 3, 1, text, x, y, color);
}

void read_serial()
{
    while (Serial.available())
    {
        process_serial(Serial.read());
    }
}

void build_strings()
{
    // power
    str0[0] = buffer[0];
    str0[1] = buffer[1];
    str0[2] = buffer[2];
    str0[3] = buffer[3];

    // frequency
    str1[0] = buffer[4];
    str1[1] = buffer[5];
    str1[3] = buffer[6];

    // temperature
    str2[0] = buffer[7];
    str2[1] = buffer[8];
    str2[3] = buffer[9];

    // pressure
    str3[0] = buffer[10];
    str3[2] = buffer[11];
}

void process_serial(char c)
{
    // incomming frame:
    // 210056045623\r
    // power:2100
    // frequency:560/10
    // temperature:456/10
    // pressure:23/10
    Serial.write(c);

    if (c == '\r' && buffer_i > 1)
    {
        buffer[buffer_i] == 0;
        buffer_i = 0;
        completed = true;
        vga.clear(background);
        build_strings();
    }
    else
    {
        buffer[buffer_i] = c;
        buffer_i++;
        completed = false;
    }
    if (buffer_i >= buffer_size)
        buffer_i = 0;
}

int i = 0;
char str_i[10];

int to_int(char str[], int len)
{
    int sum = 0;
    for (int i = 0; i < len; i++)
    {
        sum = sum * 10 + (str[0] - 48);
    }
    return sum;
}

int compare(char str[], int len, int threshold, bool direct)
{
    int value = to_int(str, len);
    if (direct)
    {
        return value > threshold ? foreground1 : foreground2;
    }
    else
    {
        return value > threshold ? foreground2 : foreground1;
    }
}

void loop()
{

    print_label("potencia", foreground3, 0, 0);
    print_value(str0, compare(str0, 4, str0_th, false), 0, 5);

    print_label("frecuencia", foreground3, 66, 0);
    print_value(str1, compare(str1, 2, str1_th, true), 66, 5);

    print_label("temperatura", foreground3, 0, 31);
    print_value(str2, compare(str2, 2, str2_th, true), 0, 36);

    print_label("presion", foreground3, 66, 31);
    print_value(str3, compare(str3, 1, str3_th, false), 66, 36);

    read_serial();
}