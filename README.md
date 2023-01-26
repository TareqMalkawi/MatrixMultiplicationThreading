Threading in C/C++ to do the Matrix Multiplication Problem using pthread library in Linux.

There are two versions (source code)

1. ThreadUnsafe, doesn't create a crtical section using mutex, therefore, a race condition will occur. 

2. ThreadSafe, create a crtical section using mutex when needed, therefore, no form of race condition will occur. 

Note : the code was compiled with the following command using C++ compiler:

g++ -std=c++11 ThreadSafe.cpp -lpthread -o t

and to pass the number of threads use ./t


