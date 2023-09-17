#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/sem.h"

int main(int argc, char *argv[])
{
    sem_open(0, 1);
    int times = atoi(argv[1]);

    if (fork() == 0)
    {
        for (unsigned int i = 0u; i < times; ++i)
        {
            sem_down(0);
            printf("Ping\n");
            sem_up(0);
        }
    }
    else
    {
        if (fork() == 0)
        {
            for (unsigned int i = 0u; i < times; ++i)
            {
                sem_down(0);
                printf("\tPong\n");
                sem_up(0);
            }
        }
        else
        {
            wait(0);
            wait(0);
        }
    }

    return 0;
}