Threading in C/C++ to do the Matrix Multiplication Problem using pthread library in Linux.

There are two versions (source code):

1. ThreadUnsafe, doesn't create a crtical section using mutex, therefore, a race condition will occur. 

2. ThreadSafe, create a crtical section using mutex when needed, therefore, no form of race condition will occur. 

Note : the code was compiled with the following command using C++ compiler:

g++ -std=c++11 ThreadSafe.cpp -lpthread -o t

and to pass the number of threads use ./t


ThreadUnsafe Result

![08](https://user-images.githubusercontent.com/91283864/214875646-68e38d7d-948c-4bcc-bb9c-8a34d88068df.png)

-------------------------------------------------------------------------------------------------------------

ThreadSafe Result

![08](https://user-images.githubusercontent.com/91283864/214875791-08d633db-5252-42e8-9e8c-12475bad3646.png)
