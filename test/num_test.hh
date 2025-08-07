// NUM TEST

#include "../gl/num.hh"

namespace num_test {

// Comparators
void test1(){
  nat a(100);
  int n = a.to_int();
  assert(n == 100, "to_int failure");

  nat b = 120;
  n = b.to_int();
  assert(n == 120, "operator= failure");

  nat c(100);
  assert(a == c, "operator== failure");
  assert(!(a == b), "operator== failure");

  assert(a != b, "operator!= failure");
  assert(!(a != c), "operator!= failure");

  assert(a < b, "operator< failure");
  assert(!(a < c), "operator< failure");
  assert(!(b < a), "operator< failure");

  assert(a <= b, "operator<= failure");
  assert(a <= c, "operator<= failure");
  assert(!(b <= a), "operator<= failure");

  assert(b > a, "operator> failure");
  assert(!(a > b), "operator> failure");
  assert(!(a > c), "operator> failure");

  assert(b >= a, "operator>= failure");
  assert(!(a >= b), "operator>= failure");
  assert(a >= c, "operator>= failure"); }

// Bitwise
void test2(){
  nat a(100);
  a <<= 64;
  assert(a.data.size() == 2, "operator<< failure");
  a >>= 64;
  assert(a == 100, "operator>> failure");

  a = 100;
  a <<= 1000;
  assert(a.data.size() == 16, "operator<< failure");
  a >>= 500;
  assert(a.data.size() == 8, "operator>> failure"); }

// Addition
void test3(){
  nat a(100), b(200);
  assert(a + b == 300, "operator+ failure");

  llu n = 0;
  a = b = ~n;
  nat c = a + b;
  assert(c.data.size() == 2, "operator+ failure");

  a = 100, b = 200, c = 300;
  a <<= 1000, b <<= 1000, c <<= 1000;
  assert(a + b == c, "operator+ failure"); }

// Subtraction
void test4(){
  nat a(300), b(100);
  assert(a - b == 200, "operator- failure");

  a = 1;
  a <<= 64;
  llu n = 0;
  b = ~n;
  nat c = a - b;
  assert(a - b == 1, "operator- failure"); }

// Multiplication
void test5(){
  nat a(100), b(100);
  assert(a * b == 10000, "operator* failure");

  a = 1;
  a <<= 32;
  b = a * a;
  assert(b.data.size() == 2, "operator* failure");
  b >>= 32;
  assert(a == b, "operator* failure"); }

// Division & modulus
void test6(){
  nat a(200), b(10);
  assert(a / b == 20, "operator/ failure");

  b = 12;
  assert(a / b == 16, "operator/ failure");

  a = b = 1;
  a <<= 64;
  b <<= 32;
  assert(a / b == b, "operator/ failure");

  a = 64, b = 10;
  assert(a % b == 4, "operator% failure");
}

// Test all
void super_test(){
  int T = 1000000;
  nat n = 100;
  int n2, n3, nr;
  while(T--){
    printf("T=%d  n.size = %d\n", T, n.data.size());
    bool check = true;
    if(n > INT_MAX) check = false;
    if(check) n2 = n.to_int();
    int o = rand() % 4;
    int r = rand() % 1000 + 1;
    if(o == 0) n += r, nr = n2 + r;
    else if(o == 1 && n > r) n -= r, nr = n2 - r;
    else if(o == 2) n *= r, nr = n2 * r;
    else n /= r, nr = n2 / r;
    if(n > INT_MAX) check = false;
    if(check) n3 = n.to_int();
    if(check) assert(n3 == nr, "super_test failure"); } }

void test_all(){
  printf("\nTesting num:\n");
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  super_test();
  printf("All tests pass :)\n"); } }
