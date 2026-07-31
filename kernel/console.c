#include <console.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define DEFAULT_FONT 0x0F

unsigned short* framebuffer;

struct vga
{
    long x;
    long y;
};

struct vga vga;

void ConsoleInstall()
{
    framebuffer = (unsigned short*)0xB8000;
    clear();
}

void put_c(char c)
{
    if (c == '\n')
    {
        vga.x = 0;
        vga.y++;
    
    } else
    {
        framebuffer[(vga.y * VGA_WIDTH) + vga.x] = DEFAULT_FONT << 8 | c;
        vga.x++;
    }

    if (vga.x >= VGA_WIDTH)
    {
        vga.x = 0;
        vga.y++;
    }

    if (vga.y >= VGA_HEIGHT)
    {
        clear();
    }
}

void put_cxy(char c, long x, long y)
{
    framebuffer[(y * VGA_WIDTH) + x] = DEFAULT_FONT << 8 | c;
}

void clear()
{
    vga.x = 0;
    vga.y = 0;

    for (long y = 0; y < VGA_HEIGHT; ++y)
    {
        for (long x = 0; x < VGA_WIDTH; ++x)
        {
            put_cxy(' ', x, y);
        }
    }
}