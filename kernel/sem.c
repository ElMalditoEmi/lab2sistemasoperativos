#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

int sem_open(int sem, int value)
{
    if (value < 0 || value > 1)
    {
        printf("Error: el primer argumento de sem_open debe ser 0 o 1\n"); // CAMBIAR
        // exit(1);
        return 0;
    }

    if (sem > maxsem)
    {
        printf("Error: se crearon más semaforos que los permitidos\n"
               "La cantidad permitida es de 1 a %d\n",
               maxsem);
        // exit(1);
        return 0;
    }

    // Lo que sigue no estoy segura de que está bien
    // Se maneja igual si un semaforo se inicia bloqueado o desbloqueado???
    initlock(&in_use[sem].lock, "newlock"); // Iniciar el spin
    acquire(&in_use[sem].lock);             // Y hacer que ningún otro hilo se meta al proceso
    in_use[sem].count = value;             // Realizo los cambios a que faltan
    release(&in_use[sem].lock);             // Desbloqueo el spin



    return 1
}