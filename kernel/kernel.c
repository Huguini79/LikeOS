#include <console.h>
#include <sched.h>
#include <pic.h>
#include <gdt.h>
#include <traps.h>
#include <printk.h>

void task3()
{
    printk("Task 3!\n");
    while (1) {}
}

void task2()
{
    printk("Task 2!\n");
    while (1) {}
}

void task1()
{
    printk("Task1!\n");
    while (1) {}
}

void kernel_main()
{
    remapPIC();
    GdtInstall();
    TrapsInstall();
    ConsoleInstall();
    printk("Kernel loaded with success\n\n");

    __asm__ volatile ("sti");

    struct task_struct* newTask = initTask(1, (long)task1);
    newTask->state = Ready;
    struct task_struct* newTask2 = initTask(2, (long)task2);
    newTask2->state = Ready;
    struct task_struct* newTask3 = initTask(3, (long)task3);
    newTask3->state = Ready;
    // exec(newTask);
    
    while (1)
    {
        __asm__ volatile ("sti");
    }
}