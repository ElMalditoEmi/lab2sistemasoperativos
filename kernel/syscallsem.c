#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#define maxsem 10
// Recordar que todas las funciones deben devolver 0 en caso de error
// Por eso comenté los exit(1) porque no estaba segura cual era la forma
// correcta de informar el error
struct semaphore
{
    int count; // Valor binario del semaforo
               // 0 == Bloqueado 1 == Libre
    struct spinlock lock;
};

struct semaphore in_use[maxsem];

uint64
sys_sem_open()
{
    int on_off, name;
    argint(0, &name);
    argint(1, &on_off);
    if (on_off < 0 || on_off > 1)
    {
        printf("Error: el primer argumento de sem_open debe ser 0 o 1\n"); // CAMBIAR
        // exit(1);
        return 0;
    }

    if (name > maxsem)
    {
        printf("Error: se crearon más semaforos que los permitidos\n"
               "La cantidad permitida es de 1 a %d\n",
               maxsem);
        // exit(1);
        return 0;
    }

    // Lo que sigue no estoy segura de que está bien
    // Se maneja igual si un semaforo se inicia bloqueado o desbloqueado???
    initlock(&in_use[name].lock, "newlock"); // Iniciar el spin
    acquire(&in_use[name].lock);             // Y hacer que ningún otro hilo se meta al proceso
    in_use[name].count = on_off;             // Realizo los cambios a que faltan
    release(&in_use[name].lock);             // Desbloqueo el spin

    return 1;
}

uint64
sys_sem_close()
{
    // Hay que hacer algo más acá porque sería lo mismo que bloquear
    int name;
    argint(0, &name);
    acquire(&in_use[name].lock);
    in_use[name].count = 0;
    release(&in_use[name].lock);
    return 1;
}

// De las que siguen se pueden mejorar los if`s pero es para que por
// ahora se vean bien las cosas y me entienda yo misma

uint64
sys_sem_up()
{
    int name;
    argint(0, &name);

    acquire(&in_use[name].lock);
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

    wakeup(&in_use[name]); // Despierta los posibles procesos durmientes. En caso contrario es inocuo
    in_use[name].count = in_use[name].count + 1;

    release(&in_use[name].lock);

    return 1;
}

uint64
sys_sem_down()
{
    int name;
    argint(0, &name);

    acquire(&in_use[name].lock);
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

    while (in_use[name].count == 0)
    {
        sleep(&in_use[name], &in_use[name].lock);
    }
    in_use[name].count = in_use[name].count - 1;

    release(&in_use[name].lock);
    return 1;
}
