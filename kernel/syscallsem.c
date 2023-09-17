#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sem.h"

// Recordar que todas las funciones deben devolver 0 en caso de error
// Por eso comenté los exit(1) porque no estaba segura cual era la forma
// correcta de informar el error

uint64
sys_sem_open()
{
    int on_off, name;
    argint(0, &name);
    argint(1, &on_off);
    
    return sem_open(name, on_off);
}

uint64
sys_sem_close()
{
    // Hay que hacer algo más acá porque sería lo mismo que bloquear
    int name;
    argint(0, &name);
    
    return sem_close(name);
}

// De las que siguen se pueden mejorar los if`s pero es para que por
// ahora se vean bien las cosas y me entienda yo misma

uint64
sys_sem_up()
{
    int name;
    argint(0, &name);

    return sem_up(name);
}

uint64
sys_sem_down()
{
    int name;
    argint(0, &name);

    return sem_down(name);
}
