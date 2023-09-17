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
    initlock(&(in_use[sem].lock), "newlock"); // Iniciar el spin
    acquire(&(in_use[sem].lock));             // Y hacer que ningún otro hilo se meta al proceso
    in_use[sem].count = value;             // Realizo los cambios a que faltan
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
    // Si el semaforo vale 1 o mas -->
    // la verdad que acá no sabia q iba un wakeup me copie d Emi jeje
    // Pero tiene sentido porque si antes quedó en 0 o menos entonces
    // quedaron procesos durmiendo que ahora cuando se active el semaforo
    // van a tener que levantarse para continuar
    /* if( in_use[name].count >= 1 ){
        //Aunque el aumento si o si es de a uno, así que con poner in_use[name].count ==1 bastaría no?
        wakeup(&in_use[name]);
        //De nuevo ese parámetro no lo tengo del todo claro;
        in_use[name].count = in_use[name].count + 1;
    }
    //Si el semaforo vale 0 o menos --> se puede cambiar el valor sin problemas
    else{
        in_use[name].count = in_use[name].count + 1;
    } */

    wakeup(&in_use[sem]); // Despierta los posibles procesos durmientes. En caso contrario es inocuo
    in_use[sem].count = in_use[sem].count + 1;

    release(&(in_use[sem].lock));

    return 1;
}

int sem_down(int sem)
{
    acquire(&(in_use[sem].lock));
    // Si el semaforo vale 1 0 más--> se puede cambiar el valor sin problemas
    /* if( in_use[name].count >=1 ){
        in_use[name].count = in_use[name].count - 1;
    }
    //Si el semaforo vale 0 o menos --> acá se tendría que esperar
    else{
        while( in_use[name].count <= 0 ){
            sleep(&in_use[name], in_use[name].lock);
            //No tengo claro que va en el primer valor de sleep acá tiré d chat gpt la dvd
        }
        in_use[name].count = in_use[name].count - 1;
    } */

    while (in_use[sem].count == 0)
    {
        sleep(&(in_use[sem]), &(in_use[sem].lock));
    }
    in_use[sem].count = in_use[sem].count - 1;

    release(&(in_use[sem].lock));

    return 1;
}