#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){
  // do stuff :)
    int semnum=running->syscall_args[0];

    SemDescriptor* des=SemDescriptorList_bySemnum(&running->sem_descriptors,semnum);

    if (! des){
        running->syscall_retvalue=DSOS_ESEMCLOSE;
        return;
     }
    
     des = (SemDescriptor*) List_detach(&running->sem_descriptors, (ListItem*) des);
     assert(des);

     Semaphore* sem=des->semaphore;

     SemDescriptorPtr* desptr=(SemDescriptorPtr*) List_detach(&sem->descriptors, (ListItem*)(des->ptr));

     assert(desptr);

     SemDescriptor_free(des);
     SemDescriptorPtr_free(desptr);

     if(sem->descriptors.size == 0){
         sem=(Semaphore*) List_detach(&semaphores_list, (ListItem*) sem);
         assert(sem);
         Semaphore_free(sem);
     }

     running->syscall_retvalue=0;
}
