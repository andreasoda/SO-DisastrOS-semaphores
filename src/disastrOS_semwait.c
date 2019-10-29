#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait(){
  // do stuff :)
   int semnum = running->syscall_args[0];

   SemDescriptor* desc=SemDescriptorList_bySemnum(&running->sem_descriptors, semnum);
    
   if (! desc){
     running->syscall_retvalue=DSOS_ENOSEM;
     return;
   }

   Semaphore* sem=desc->semaphore;
   sem->count--;

   if(sem->count < 0){
     running->status=Waiting;

     SemDescriptorPtr* descptr = SemDescriptorPtr_alloc(desc);
     List_insert(&sem->waiting_descriptors, sem->waiting_descriptors.last, (ListItem*) descptr);

     PCB* next_running = (PCB*) List_detach(&ready_list, ready_list.first);
     running=next_running;
   }
    running->syscall_retvalue = 0;
    
    return;
}
