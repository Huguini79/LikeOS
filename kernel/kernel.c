#include <console.h>
#include <printk.h>

void kernel_main()
{
    ConsoleInstall();
    printk("Hola Mundo!");
}