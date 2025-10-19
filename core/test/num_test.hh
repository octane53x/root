// NUM TEST

#include "../gl/num.hh"

namespace num_test {

// Comparators
void test1(){
  printf("  Comparators\n");

  // Basic tests
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
  assert(a >= c, "operator>= failure");

  // Small numbers
  int T = 10000;
  while(T--){
    int ni1 = rand(), ni2 = rand();
    nat n1 = ni1, n2 = ni2;
    if(ni1 < ni2){
      assert(!(n1 == n2), "operator== failure");
      assert(n1 != n2, "operator!= failure");
      assert(n1 < n2, "operator< failure");
      assert(n1 <= n2, "operator<= failure");
      assert(!(n1 > n2), "operator> failure");
      assert(!(n1 >= n2), "operator>= failure");
    }else if(ni1 > ni2){
      assert(!(n1 == n2), "operator== failure");
      assert(n1 != n2, "operator!= failure");
      assert(n1 > n2, "operator> failure");
      assert(n1 >= n2, "operator>= failure");
      assert(!(n1 < n2), "operator< failure");
      assert(!(n1 <= n2), "operator<= failure"); } } }

// Bitwise
void test2(){
  printf("  Bitwise\n");

  // Basic tests
  nat a(100);
  a <<= 64;
  assert(a.data.size() == 2, "operator<< failure");
  a >>= 64;
  assert(a == 100, "operator>> failure");

  a = 100;
  a <<= 1000;
  assert(a.data.size() == 16, "operator<< failure");
  a >>= 500;
  assert(a.data.size() == 8, "operator>> failure");

  // Small numbers
  int T = 10000;
  while(T--){
    int ni = rand() % 1000;
    int s = rand() % 20;
    nat n = ni;
    ni <<= s, n <<= s;
    assert(ni == n.to_int(), "operator<< failure");
    s = rand() % 20;
    ni >>= s, n >>= s;
    assert(ni == n.to_int(), "operator>> failure"); }

  // Shift up and back by the same amount and see if the number changes
  T = 10000;
  while(T--){
    int ni = rand();
    int s = rand() % 1000;
    nat n = ni;
    n <<= s, n >>= s;
    assert(n.to_int() == ni, "operator<< or operator>> failure"); } }

// Addition
void test3(){
  printf("  Addition\n");

  // Basic tests
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
  printf("  Subtraction\n");

  // Basic tests
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
  printf("  Multiplication\n");

  // Basic tests
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
  printf("  Division & modulus\n");

  // Basic tests
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

  // Check small numbers against native int division
  int T = 10000;
  while(T--){
    int ni = rand(), di = rand();
    nat n = ni, d = di;
    int ri = ni / di;
    nat r = n / d;
    assert(r.to_int() == ri, "operator/ failure"); }

  // Multiply then divide by the same amount and see if the number changes
  T = 10000;
  while(T--){
    printf("T%d\n", T);
    int size = rand() % 50 + 1;
    int bits_set = rand() % size / 2;
    nat n, n2, d;
    for(int i = 0; i < bits_set; ++i){
      int pos = rand() % size;
      nat bit = 1;
      n |= bit << pos; }
    printf("  n = ");
    n.print_bits();
    printf("\n");
    int di = rand();
    d = di;
    n2 = n;
    n2 *= d;
    n2 += rand() % di;
    n2 /= d;
    assert(n == n2, "operator/ failure"); }
}

// Test all
void super_test(){
  int T = 1000000;
  nat n = 100;
  int n2, n3, nr;
  while(T--){
    printf("T=%d  n.size = %d\n", T, n.data.size());
    printf("  n = ");
    n.print();
    printf("\n");

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
  //super_test();
  printf("All tests pass :)\n"); } }
