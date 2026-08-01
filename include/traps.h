#ifndef TRAPS_H
#define TRAPS_H

struct interrupt_frame
{
    long edi;
    long esi;
    long ebp;
    long reserved;
    long ebx;
    long edx;
    long ecx;
    long eax;
    long eip;
    long cs;
    long eflags;
    long esp;
    long ss;

} __attribute__((packed));

void TrapsInstall();

#endif