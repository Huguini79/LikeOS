#include <printk.h>
#include <ioport.h>
#include <panic.h>

struct Idt
{
    unsigned short isr_low;
    unsigned short selector;
    unsigned char reserved;
    unsigned char type_attributes;
    unsigned short isr_high;

} __attribute__((packed));

struct Idtr
{
    unsigned short limit;
    long base;

} __attribute__((packed));

struct Idtr idtr;

struct Idt idt_table[256] __attribute__((aligned(8)));

extern void divide_by_zero_error_stub();
extern void keyboard_handler_stub();
extern void clock_handler_stub();

void setIdtDescriptor(long vector, unsigned char type_attributes, void* isr)
{
    struct Idt* Idt = &idt_table[vector];
    Idt->isr_low = (long)isr & 0xFFFF;
    Idt->selector = 0x08;
    Idt->reserved = 0;
    Idt->type_attributes = type_attributes;
    Idt->isr_high = (long)isr >> 16 & 0xFFFF;
}

void divide_by_zero_error()
{
    panic("* KERNEL PANIC: DIVIDE BY ZERO ERROR *");
}

void keyboard_handler()
{
    printk("Key pressed!");
    outb(0x20, 0x20);
}

volatile long ticks = 0;

void clock_handler()
{
    ticks += 10;
    outb(0x20, 0x20);
}

void TrapsInstall()
{
    setIdtDescriptor(0, 0x8E, divide_by_zero_error_stub);
    setIdtDescriptor(0x20, 0x8E, clock_handler_stub);
    setIdtDescriptor(0x21, 0x8E, keyboard_handler_stub);

    idtr.limit = sizeof(idt_table) - 1;
    idtr.base = (long)idt_table;

    __asm__ volatile ("lidt %0" :: "m"(idtr));
}