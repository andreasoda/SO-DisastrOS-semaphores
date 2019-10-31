# SO-DisastrOS-semaphores


The aim of the project is to provide an implementation of semaphores to control access to a common resource by multiple processes in disastrOS, the didactic operative system created by Prof. Giorgio Grisetti in the scope of the course Sistemi Operativi

Instructions

to compile run

make

then

./disastrOS_test

to start the test.

Usage

To open a semaphore in disastrOS, call disastrOS_semOpen(int semnum). It creates a system semaphore with global id semnum (must be >= 0) if it does not exist in the system and opens it for the calling process, putting it in the list of the opened semaphores of the process. If the semaphore with the given id already exists, this function just opens it for the process. If everything went fine it returns an integer representing the descriptor of the semaphore, otherwise an error code (see section below).

Both disastrOS_wait(int semfd) and disastrOS_semPost(int semfd) are used to change the value of the counter of the semaphore with descriptor semfd (the process should have opened the semaphore before calling these functions), respectively by decreasing and increasing it. On success they return 0, an error code otherwise.

Semaphores are closed by calling disastrOS_semClose(int semfd) by passing an integer representig the descriptor. When all processes using a certain semaphore closed it, it is automatically unlinked.

