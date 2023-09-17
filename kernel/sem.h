#ifndef SEM_H
#define SEM_H

#define maxsem 10

int sem_open(int sem, int value);

int sem_close(int sem);

int sem_up(int sem);

int sem_down(int sem);

#endif