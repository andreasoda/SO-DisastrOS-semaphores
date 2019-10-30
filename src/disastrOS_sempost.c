#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semPost(){
  // do stuff :)
    
    int semnum = running->syscall_args[0];

    SemDescriptor* desc=SemDescriptorList_bySemnum(&running->sem_descriptors, semnum);

    if (! desc){
      disastrOS_debug("non ci sono semafori aperti con questo fd\n");
      running->syscall_retvalue=DSOS_ENOSEM;
      return;
    }

    Semaphore* sem=desc->semaphore;
    sem->count++;
    disastrOS_debug("incrementato valore semaforo\n");
 
    if(sem->count <= 0){
 
      SemDescriptorPtr* next_running_sem_desc_ptr = (SemDescriptorPtr*) List_detach(&sem->waiting_descriptors, sem->waiting_descriptors.first);

      PCB* next_running_PCB = next_running_sem_desc_ptr->descriptor->pcb;
      next_running_PCB->status=Ready;

      SemDescriptorPtr_free(next_running_sem_desc_ptr);

      List_insert(&ready_list, ready_list.first, (ListItem*) next_running_PCB);
    }
    running->syscall_retvalue = 0;
    
    return;
}
