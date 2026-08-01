#ifndef SCHED_H
#define SCHED_H

#include <traps.h>

struct registers
{
    long eax;
    long ecx;
    long ebx;
    long edx;
    long esi;
    long edi;
    long ebp;
    long esp;
    long eflags;
    long eip;
    long cs;

} __attribute__((packed));

enum State
{
    Ready,
    Running,
    Blocked,
    Zombie,
    Sleep,
};

struct sigaction
{
    void* handler;
};

struct task_struct
{
    long pid;
    long alarm;
    long signal;
    enum State state;
    struct sigaction sigactions[32];
    struct registers registers;
};

extern struct task_struct tasks[64];
extern struct task_struct* current;

void saveContext(struct interrupt_frame* frame);
struct task_struct* initTask(long pid, long eip);
int exec(struct task_struct* task);
void schedule();

#endif