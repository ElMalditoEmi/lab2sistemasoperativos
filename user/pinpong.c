#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
//Programa solo para probar q lo tomaba el xv6 
void pinpong(int a){
  
  sem_open(1,1);
  sem_up(1);
  sem_open(2,0);
  sem_down(2);
  sem_open(3,0);
  sem_up(3);
  sem_down(3);
  sem_open(12,0);
  exit(0);
}

int main(int argc, char *argv[]){
  pinpong(4);
  exit(0);
}