Threading in C++ to do the Matrix Multiplication Problem using pthread library on Linux.

The program creates T worker threads to perform the operation (T will be passed to the program with the Linux command line). Where each of the threads
will perform part of the total matrix multiplication operation. 

There are two versions of the program (source code):

1. ThreadUnsafe, doesn't create a crtical section using mutex, therefore, a race condition will occur. 

2. ThreadSafe, create a crtical section using mutex when needed, therefore, no form of race condition will occur. 

Note : the code was compiled with the following command using C++ compiler:

g++ -std=c++11 ThreadSafe.cpp -lpthread -o t , and to pass the number of threads use ./t

-------------------------------------------------------------------------------------------------------------

ThreadUnsafe Result


![08](https://user-images.githubusercontent.com/91283864/214876178-1c57f0e8-56d3-40bf-addd-aa1cd5e39b3a.png)


-------------------------------------------------------------------------------------------------------------


ThreadSafe Result


![08](https://user-images.githubusercontent.com/91283864/214876282-5dcbd63e-3cdc-451f-9750-bcd89a3c8345.png)

