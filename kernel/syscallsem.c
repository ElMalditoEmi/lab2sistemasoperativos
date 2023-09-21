#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sem.h"

// Recordar que todas las funciones deben devolver 0 en caso de error

uint64
sys_sem_open()
{
    int on_off, name;
    argint(0, &name);
    argint(1, &on_off);
    if (name >= maxsem)
    {
        panic("sem_id exceded");
    }
    return sem_open(name, on_off);
}

uint64
sys_sem_close()
{
    // Hay que hacer algo más acá porque sería lo mismo que bloquear
    int name;
    argint(0, &name);
     if (name >= maxsem)
    {
        panic("sem_id exceded");
    }
    return sem_close(name);
}

uint64
sys_sem_up()
{
    int name;
    argint(0, &name);
    if (name >= maxsem)
    {
        panic("sem_id exceded");
    }
    return sem_up(name);
}

uint64
sys_sem_down()
{
    int name;
    argint(0, &name);
    if (name >= maxsem)
    {
        panic("sem_id exceded");
    }
    return sem_down(name);
}
