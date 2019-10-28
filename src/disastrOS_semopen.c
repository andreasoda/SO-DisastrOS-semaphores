#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen(){
  // do stuff :)
     int semnum = running->syscall_args[0];

     if(semnum < 0){
       running->syscall_retvalue = DSOS_ESEMOPEN;
       return;
     }

     Semaphore* sem = SemaphoreList_byId(&semaphores_list, semnum);

     if(sem == NULL){

       sem = Semaphore_alloc(semnum, 1);

       if(sem == NULL) {
         running->syscall_retvalue = DSOS_ESEMOPEN;
         return;
       }

       List_insert(&semaphores_list, semaphores_list.last, (ListItem*) sem);
     }

     SemDescriptor* sem_des = SemDescriptor_alloc(running->last_sem_fd, sem, running);
     if(! sem_des){
       running->syscall_retvalue = DSOS_ESEMNOFD;
       return;
     }

     running->last_sem_fd++;

     SemDescriptorPtr* sem_descptr = SemDescriptorPtr_alloc(sem_des);
     List_insert(&running->sem_descriptors, running->sem_descriptors.last, (ListItem*) sem_des);

     des->ptr=sem_descptr;
     List_insert(&sem->descriptors, sem->descriptors.last, (ListItem*) sem_descptr);

     running->syscall_retvalue = semnum;
    
     return;
    
}
