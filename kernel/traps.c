#include <printk.h>
#include <ioport.h>
#include <panic.h>
#include <traps.h>
#include <sched.h>

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
extern void double_fault_stub();
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

void double_fault()
{
    panic("* KERNEL PANIC: DOUBLE FAULT *");
}

void keyboard_handler()
{
    printk("Key pressed!");
    outb(0x20, 0x20);
}

volatile long ticks = 0;

void clock_handler(struct interrupt_frame* frame)
{
    outb(0x20, 0x20);
    ticks += 10;
    printk("CPU TICKS: %i\n", ticks);
    saveContext(frame);
    schedule();
}

void debug()
{
	panic("* KERNEL PANIC - DEBUG *");
}

void nmi()
{
	panic("* KERNEL PANIC - NMI *");
}

void int3()
{
	panic("* KERNEL PANIC - INT3 *");
}

void overflow()
{
	panic("* KERNEL PANIC - OVERFLOW *");
}

void bounds()
{
	panic("* KERNEL PANIC - BOUNDS *");
}

void invalid_op()
{
	panic("* KERNEL PANIC - INVALID OP *");
}

void device_not_available()
{
	panic("* KERNEL PANIC - DEVICE NOT AVAILABLE *");
}

void coprocessor_segment_overrun()
{
	panic("* KERNEL PANIC - Coprocessor Segment Overrun *");
}


void invalid_tss()
{
	panic("* KERNEL PANIC - Invalid TSS *");
}

void segment_not_present()
{
	panic("* KERNEL PANIC - Segment not present *");
}

void stack_segment()
{
	panic("* KERNEL PANIC - Stack Segment *");
}

void general_protection()
{
	panic("* KERNEL PANIC - General Protection Fault #GP *");
}

void page_fault()
{
	panic("* KERNEL PANIC - Page Fault #PF *");
}

void reserved()
{
	panic("* KERNEL PANIC - Reserved *");
}

void coprocessor_error()
{
	panic("* KERNEL PANIC - Coprocessor error *");
}

void TrapsInstall()
{
    setIdtDescriptor(0, 0x8E, divide_by_zero_error_stub);
    setIdtDescriptor(0x01, 0x8E, debug);
	setIdtDescriptor(0x02, 0x8E, nmi);
	setIdtDescriptor(0x03, 0x8E, int3);
	setIdtDescriptor(0x04, 0x8E, overflow);
	setIdtDescriptor(0x05, 0x8E, bounds);
	setIdtDescriptor(0x06, 0x8E, invalid_op);
	setIdtDescriptor(0x07, 0x8E, device_not_available);
    setIdtDescriptor(0x08, 0x8E, double_fault);
	setIdtDescriptor(0x09, 0x8E, coprocessor_segment_overrun);
	setIdtDescriptor(0x10, 0x8E, invalid_tss);
	setIdtDescriptor(0x11, 0x8E, segment_not_present);
	setIdtDescriptor(0x12, 0x8E, stack_segment);
	setIdtDescriptor(0x13, 0x8E, general_protection);
	setIdtDescriptor(0x14, 0x8E, page_fault);
	setIdtDescriptor(0x15, 0x8E, reserved);
	setIdtDescriptor(0x16, 0x8E, coprocessor_error);
    setIdtDescriptor(0x20, 0x8E, clock_handler_stub);
    setIdtDescriptor(0x21, 0x8E, keyboard_handler_stub);

    idtr.limit = sizeof(idt_table) - 1;
    idtr.base = (long)idt_table;

    __asm__ volatile ("lidt %0" :: "m"(idtr));
}