#include <console.h>
#include <pic.h>
#include <gdt.h>
#include <traps.h>
#include <printk.h>

void kernel_main()
{
    remapPIC();
    GdtInstall();
    TrapsInstall();
    ConsoleInstall();
    printk("Hola Mundo!");

    __asm__ volatile ("sti");
    
    while (1)
    {
        __asm__ volatile ("hlt");
    }
}