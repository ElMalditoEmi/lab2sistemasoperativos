#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sem.h"

struct semaphore
{
    int count; // Valor binario del semaforo
               // 0 == Bloqueado 1 == Libre
    struct spinlock lock;
};

struct semaphore in_use[maxsem];

int sem_open(int sem, int value)
{
    if (value < 0)
    {
        printf("ERROR: el primer argumento de sem_open debe ser mayor o igual que 0\n"); // CAMBIAR
        // exit(1);
        return 0;
    }

    if (sem > maxsem)
    {
        printf("ERROR: se crearon más semaforos que los permitidos\n"
               "La cantidad permitida es de 1 a %d\n",
               maxsem);
        // exit(1);
        return 0;
    }

    initlock(&(in_use[sem].lock), "newlock"); // Iniciar el spin
    acquire(&(in_use[sem].lock));             // Y hacer que ningún otro hilo inicialize el semaforo al mismo tiempo
    in_use[sem].count = value;                // Realizo los cambios a que faltan
    release(&(in_use[sem].lock));             // Desbloqueo el spin

    return 1;
}

int sem_close(int sem){

    acquire(&(in_use[sem].lock));
    in_use[sem].count = 0;
    release(&(in_use[sem].lock));

    return 1;
}

int sem_up(int sem)
{
    acquire(&(in_use[sem].lock));

    wakeup(&in_use[sem]); // Despierta los posibles procesos durmientes a causa del semaforo (como nunca es negativo, siempre que se le sume se deben liberar). En caso contrario es inocuo
    in_use[sem].count = in_use[sem].count + 1;

    release(&(in_use[sem].lock));

    return 1;
}

int sem_down(int sem)
{
    acquire(&(in_use[sem].lock));

    while (in_use[sem].count <= 0)
    {
        sleep(&(in_use[sem]), &(in_use[sem].lock));
    }
    in_use[sem].count = in_use[sem].count - 1;

    release(&(in_use[sem].lock));

    return 1;
}