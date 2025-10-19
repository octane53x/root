// THREAD TEST

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <thread>

using std::thread;

void fn(int n){
  for(int i = 0; i < 10000000; ++i)
    int r = rand() * rand();
}

int main(){
  clock_t t0 = clock();
  for(int i = 0; i < 100000000; ++i)
    int r = rand() * rand();
  clock_t tf = clock();
  printf("No threads: %lf sec\n", (double)(tf-t0)/CLOCKS_PER_SEC);

  t0 = clock();
  thread* thds[10];
  for(int i = 1; i <= 10; ++i)
    thds[i] = new thread(fn, i);
  for(int i = 1; i <= 10; ++i)
    thds[i]->join();
  tf = clock();
  printf("10 threads: %lf sec\n", (double)(tf-t0)/CLOCKS_PER_SEC);

  return 0;
}
