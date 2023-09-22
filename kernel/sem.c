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
    int count;
    int opened;
    struct spinlock lock;
};

struct semaphore in_use[maxsem];

int sem_open(int sem, int value)
{
    if (value < 0)
    {
        printf("ERROR: el primer argumento de sem_open debe ser mayor o igual que 0\n"); // CAMBIAR
        return 0;
    }

    if (sem > maxsem)
    {
        printf("ERROR: se crearon más semaforos que los permitidos\n"
               "La cantidad permitida es de 1 a %d\n",
               maxsem);
        return 0;
    }

    acquire(&(in_use[sem].lock)); // Y hacer que ningún otro hilo inicialice el semaforo al mismo tiempo

   if (in_use[sem].opened != 0) // Si el semaforo ya estaba abierto devolver 0
    {
        release(&(in_use[sem].lock));
        return 0;
    }

    in_use[sem].opened = 1;
    in_use[sem].count = value; // Realizo los cambios a que faltan
    release(&(in_use[sem].lock)); // Desbloqueo el spin

    return 1;
}

int sem_close(int sem)
{
    acquire(&(in_use[sem].lock));
    if (in_use[sem].opened == 0)
    {
        release(&(in_use[sem].lock));
        return 0;
    }
    in_use[sem].count = 0;
    in_use[sem].opened = 0;
    release(&(in_use[sem].lock));

    return 1;
}

int sem_up(int sem)
{
    acquire(&(in_use[sem].lock));

    in_use[sem].count = in_use[sem].count + 1;
    wakeup(&(in_use[sem])); // Despierta los posibles procesos durmientes a causa del semaforo (como nunca es negativo, siempre que se le sume se deben liberar). En caso contrario es inocuo

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
    
    if(in_use[sem].count>=1){
        in_use[sem].count = in_use[sem].count - 1;
    }
    else{
        in_use[sem].count = 0;
    }

    release(&(in_use[sem].lock));

    return 1;
}

void init_all_sem()
{
    for (unsigned int i = 0u; i < maxsem; i++)
    {
        in_use[i].count = 0;
        in_use[i].opened = 0;
        initlock(&(in_use[i].lock), "newlock"); // Iniciar el spin
    }

}