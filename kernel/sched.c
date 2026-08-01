#include <sys/signal.h>
#include <sched.h>
#include <traps.h>
#include <printk.h>

#include <stdint.h>
#include <stddef.h>

struct task_struct tasks[64] = {0};
struct task_struct* current = &tasks[0];
struct task_struct* next = &tasks[0];

void blank()
{
    while (1)__asm__ volatile ("hlt");
}

struct task_struct* initTask(long pid, long eip)
{
    struct task_struct* task = &tasks[pid];
    task->pid = pid;
    task->alarm = 0;
    task->signal = 0;
    task->state = Ready;
    task->registers.eax = 0;
    task->registers.ecx = 0;
    task->registers.ebx = 0;
    task->registers.edx = 0;
    task->registers.esi = 0;
    task->registers.edi = 0;
    uintptr_t* stack_top = 0x3FF00 + (pid * 8192) + 8192;
    stack_top--;
    *stack_top = (long)blank;
    task->registers.esp = stack_top;
    task->registers.ebp = 0;
    task->registers.cs = 0x08;
    task->registers.eip = eip;
    task->registers.eflags = 0x202;

    /*
        * A diferencia de la multitarea por hardware usando TSS (Task State Structure), no modificamos la GDT dinámicamente para agregar el contexto de la tarea (el TSS del proceso) en un descriptor dentro de la GDT
    */

    return task;

}

void saveContext(struct interrupt_frame* frame)
{
    current->registers.eax = frame->eax;
    current->registers.ecx = frame->ecx;
    current->registers.ebx = frame->ebx;
    current->registers.edx = frame->edx;
    current->registers.esi = frame->esi;
    current->registers.edi = frame->edi;
    current->registers.esp = frame->esp;
    current->registers.eip = frame->eip;
    current->registers.eflags = frame->eflags;
}

int exec(struct task_struct* task)
{
    __asm__ volatile (
        "movl %6, %%esp\n"
        "pushl %9\n"
        "pushl %8\n"
        "pushl %7\n"
        "movl %0, %%eax\n"
        "movl %1, %%ecx\n"
        "movl %2, %%ebx\n"
        "movl %3, %%edx\n"
        "movl %4, %%esi\n"
        "movl %5, %%edi\n"
        "iret"
        :: "m"(task->registers.eax), "m"(task->registers.ecx), "m"(task->registers.ebx), "m"(task->registers.edx), "m"(task->registers.esi), "m"(task->registers.edi), "m"(task->registers.esp), "m"(task->registers.eip), "m"(task->registers.cs), "m"(task->registers.eflags)
    );
    return 0;
}

void schedule()
{
    if (tasks[current->pid+1].registers.eip != 0)
    {
        //printk("PID inicial = %i\n", current->pid);
        next = &tasks[current->pid+1];

    } else
    {
        //printk("PID inicial = %i\nHeisenberg", current->pid);
        for (long i = 0; i < 64; ++i)
        {
            if (tasks[i].registers.eip != 0)
            {
                next = &tasks[i];
                //printk("PID elegido = %i\nEIP: %i\n", next->pid, next->registers.eip);
                break;
            }
        }
    }

    if (current->signal != 0)
    {
        psig();
    }

    current->state = Ready;
    current = next;
    current->state = Running;

    exec(current);
}