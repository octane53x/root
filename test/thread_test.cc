// THREAD TEST

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <thread>

#define NTHD 4
#define COMP 1000000000

using std::thread;

void fn(int n){
  for(int i = 0; i < COMP/NTHD; ++i)
    int r = 365435675 * 284930594;
}

int main(){
  clock_t t0 = clock();
  for(int i = 0; i < COMP; ++i)
    int r = 365435675 * 284930594;
  clock_t tf = clock();
  printf("No threads: %lf sec\n", (double)(tf-t0)/CLOCKS_PER_SEC);

  t0 = clock();
  thread* thds[NTHD];
  for(int i = 1; i <= NTHD; ++i){
    printf("Creating thread %d\n", i);
    thds[i] = new thread(fn, i);
    printf("Created thread %d\n", i);
  }
  for(int i = 1; i <= NTHD; ++i){
    printf("Joining thread %d\n", i);
    thds[i]->join();
    printf("Joined thread %d\n", i);
  }
  tf = clock();
  printf("%d threads: %lf sec\n", NTHD, (double)(tf-t0)/CLOCKS_PER_SEC);

  return 0;
}
