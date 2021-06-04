
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctime>
#include <sys/mman.h>
#include <cstring>


using namespace std;

bool flag = true;
sem_t semWrite, semRead;
int *ptr;

void *func(void* )
{
   int num = 0;
   while (flag){
       num = rand();
       cout << "Random number = " << num << endl;
       cout.flush();
       mempcpy(ptr, &num, sizeof (int));
       sem_post(&semWrite);
       sem_wait(&semRead);
       sleep(1);
   }
   return NULL;
}


int main()
{
    pthread_t thread;
    int fd = shm_open("/strg",O_RDWR, 0);
    ftruncate(fd,sizeof (int));
    ptr = (int*)mmap(0,sizeof (int), O_RDWR, MAP_SHARED, fd, 0);
    sem_open("/semRead", O_CREAT, 0644, 1);
    sem_open("/semWrite", O_CREAT, 0644, 1);
    pthread_create(&thread,NULL, func, NULL);
    getchar();
    flag = false;
    pthread_join(thread,NULL);
    sem_close(&semRead);
    sem_unlink("/semRead");
    sem_close(&semWrite);
    sem_unlink("/semWrite");
    munmap(ptr,sizeof (int));
    close(fd);
    shm_unlink("/fd");
    return 0;

}
