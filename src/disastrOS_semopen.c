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
       // semnum non valido
       disastrOS_debug("id non valido");
       running->syscall_retvalue = DSOS_ESEMOPEN;
       return;
     }

     disastrOS_debug("cerco semaforo con id: %d (se esiste)\n", semnum);
    
     Semaphore* sem = SemaphoreList_byId(&semaphores_list, semnum);

     if(sem == NULL){
         
       // il semaforo non esiste e deve essere allocato
       disastrOS_debug("alloco semaforo con id: %d\n", semnum);
       sem = Semaphore_alloc(semnum, 1);

       //controllo se il semaforo è stato correttamente allocato
       if(sem == NULL) {
           
         disastrOS_debug("allocazione del semaforo fallita\n");
         running->syscall_retvalue = DSOS_ESEMOPEN;
         return;
       }
         
       disastrOS_debug("allocazione del semaforo riuscita\n");
         
       List_insert(&semaphores_list, semaphores_list.last, (ListItem*) sem);
     }
    
     disastrOS_debug("allocazione del semaforo riuscita\n");
    
     // alloco il descrittore del semaforo per il processo corrente

     SemDescriptor* des = SemDescriptor_alloc(running->last_sem_fd, sem, running);
     if(! des){
       running->syscall_retvalue = DSOS_ESEMNOFD;
       return;
     }
    
     disastrOS_debug("Descrittore correttamente creato\n");

     running->last_sem_fd++;

     SemDescriptorPtr* sem_descptr = SemDescriptorPtr_alloc(des);
     List_insert(&running->sem_descriptors, running->sem_descriptors.last, (ListItem*) des);

     //aggiungo descrittore alla struct del semaforo
     des->ptr=sem_descptr;
     List_insert(&sem->descriptors, sem->descriptors.last, (ListItem*) sem_descptr);

     running->syscall_retvalue = semnum;
    
     return;
    
}
