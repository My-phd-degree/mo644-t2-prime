#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]);
int prime_default(int n);
void sieve(int size);
int * qttPrimeNumbers;

int main(int argc, char *argv[]) {
  int n;
  int n_factor;
  int n_hi;
  int n_lo;
  int primes;
  double t;
  FILE *input;

  if (argc < 2) {
    fprintf(stderr, "Error: missing path to input file\n");
    return 1;
  }

  if ((input = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Error: could not open file\n");
    return 1;
  }

  n_lo = 1;
  n_factor = 2;

 // Do not change this line
  omp_set_num_threads(4);

  fscanf(input, "%d", &n_hi);
  n_hi = 1 << n_hi;

  printf("                    \n");
  printf("         N     Pi(N)\n");
  printf("\n");

  n = n_lo;

  t = omp_get_wtime();

  sieve(n_hi);
  while (n <= n_hi) {
    primes = prime_default(n);

    printf("  %8d  %8d\n", n, primes);

    n = n * n_factor;
  }
  free(qttPrimeNumbers);

  t = omp_get_wtime() - t;

  /*
    Terminate.
  */
  fprintf(stderr, "%lf\n", t);

  return 0;
}

void sieve (int size) {
  int i,
      j;
  int rootSqrt = (int) ceil(sqrt(size));
  qttPrimeNumbers = (int *) malloc((size + 1) * sizeof(int));
  bool bitset[size + 1];
  //sieve
  memset(bitset, true, size + 1);
  bitset[0] = bitset[1] = false;
//  #pragma omp parallel num_threads(4) default(none), shared(bitset, size, rootSqrt), private(i, j)
//  #pragma omp for schedule(static)
  #pragma omp parallel for num_threads(4) default(none), shared(bitset, size, rootSqrt), private(i, j)
  for (i = 2; i <= rootSqrt; ++i) 
    if (bitset[i] == true)
      for (j = i + i; j <= size; j += i) {
        bitset[j] = false;
      }
  //sum array
  qttPrimeNumbers[0] = 0;
  for (i = 1; i <= size; ++i) {
    qttPrimeNumbers[i] = qttPrimeNumbers[i - 1];
    if (bitset[i] == true) 
      ++qttPrimeNumbers[i];
  }
}

/*
  Purpose:
   counts primes.
  Licensing:
    This code is distributed under the GNU LGPL license.
  Modified:
    10 July 2010
  Author:
    John Burkardt
  Parameters:
    Input, the maximum number to check.
    Output, the number of prime numbers up to N.
*/
int prime_default(int n) {
  /*
  //setting
  int i;
  int j;
  int prime;
  int total = 0;

  for (i = 2; i <= n; i++) {
    prime = 1;

    for (j = 2; j < i; j++) {
      if (i % j == 0) {
        prime = 0;
        break;
      }
    }
    total = total + prime;
  }

  return total;
  */
  return qttPrimeNumbers[n];
}
