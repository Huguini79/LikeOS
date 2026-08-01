#include <sys/signal.h>
#include <sched.h>
#include <printk.h>

void signal(long signal, void* handler)
{
    current->sigactions[signal].handler = handler;
}

void sendsig(struct task_struct* task, long signal)
{
    task->signal |= 1U << (signal);
}

void psig()
{
    if (current->signal & (1U << SIGALRM))
    {
        if (current->sigactions[SIGALRM].handler == 0)
        {
            printk("\n* SIGALRM FOR PROCESS WITH PID=%i *\n", current->pid);
            current->signal &= ~(1U << SIGALRM);
    
        } else
        {
            void (*handler)(void) = (void(*)(void))current->sigactions[SIGALRM].handler;
            handler(); 
        }
    }
}