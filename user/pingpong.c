#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/sem.h"

int pick_sem(int value)
{
    int opened = 0;
    int sem = 0;

    while (sem < maxsem && !opened)
    {
        opened = sem_open(sem, value);
        if (!opened)
        {
            ++sem;
        }
    }

    if (!opened)
    {
        return -1;
    }

    return sem;
}

// pkiller: pingpong 4 &; pingpong 5 &;
int main(int argc, char *argv[])
{
    int sem1 = -1, sem2 = -1;

    sem1 = pick_sem(1);
    sem2 = pick_sem(0);

    if (sem1 == -1)
    {
        printf("ERROR: pingpong no pudo abrir los semáforos: 0\n");

        if (sem2 != -1)
        {
            sem_close(sem2);
        }

        return 0;
    }

    if (sem2 == -1)
    {
        printf("ERROR: pingpong no pudo abrir los semáforos: 1\n");

        if (sem1 != -1)
        {
            sem_close(sem1);
        }

        return 0;
    }

    if (argc != 2 || !atoi(argv[1])) // atoi retorna 0 si no logra handlear los caracteres correctos
    {
        printf("ERROR: Se esperaba recibir un único valor mayor o igual que 0\n");
        return 0;
    }

    int times = atoi(argv[1]);

    if (fork() == 0)
    {
        for (unsigned int i = 0u; i < times; ++i)
        {
            sem_down(sem1);
            printf("ping\n");
            sem_up(sem2);
        }
    }
    else
    {
        if (fork() == 0)
        {
            for (unsigned int i = 0u; i < times; ++i)
            {
                sem_down(sem2);
                printf("\tpong\n");
                sem_up(sem1);
            }
        }
        else
        {
            wait(0);
            wait(0);
            sem_close(sem2);
            sem_close(sem1);
        }
    }

    return 1;
}
