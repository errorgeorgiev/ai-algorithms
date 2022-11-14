# N-queens problem solver
The programm is written using C++11. The naive-algorithm works okay for small numbers( < 1000), but will struggle if board size is 10 000. The optimised-algorithm is using caching and MinConflicts to solve the problem faster. Both implementations print the solution on the terminal. To compile and run:
```
g++ optimised-algorithm.cpp 
./a.out
```