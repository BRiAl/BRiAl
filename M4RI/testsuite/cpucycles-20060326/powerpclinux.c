#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

static long myround(double u)
{
  long result = u;
  while (result + 0.5 < u) result += 1;
  while (result - 0.5 > u) result -= 1;
  return result;
}

static long long microseconds(void)
{
  struct timeval t;
  gettimeofday(&t,(struct timezone *) 0);
  return t.tv_sec * (long long) 1000000 + t.tv_usec;
}

static long long timebase(void)
{
  unsigned long high;
  unsigned long low;
  unsigned long newhigh;
  unsigned long long result;
  asm volatile(
    "Lcpucycles:mftbu %0;mftb %1;mftbu %2;cmpw %0,%2;bne Lcpucycles"
    : "=r" (high), "=r" (low), "=r" (newhigh)
  );
  result = high;
  result <<= 32;
  result |= low;
  return result;
}

static double cpufrequency = 0;
static long tbcycles = 0;

static void init(void)
{
  FILE *f;
  int s;
  long long tb0; long long us0;
  long long tb1; long long us1;

  f = fopen("/proc/cpuinfo","r");
  if (!f) return 0;

  for (;;) {
    s = fscanf(f," clock : %lf MHz",&cpufrequency);
    if (s > 0) break;
    if (s == 0) s = fscanf(f,"%*[^\n]\n");
    if (s < 0) { cpufrequency = 0; break; }
  }

  fclose(f);
  if (!cpufrequency) return;
  cpufrequency *= 1000000.0;

  tb0 = timebase();
  us0 = microseconds();
  do {
    tb1 = timebase();
    us1 = microseconds();
  } while (us1 - us0 < 10000);
  if (tb1 <= tb0) return;

  tb1 -= tb0;
  us1 -= us0;
  tbcycles = myround((cpufrequency * 0.000001 * (double) us1) / (double) tb1);
}

long long cpucycles_powerpclinux(void)
{
  if (!tbcycles) init();
  return timebase() * tbcycles;
}

long long cpucycles_powerpclinux_persecond(void)
{
  if (!tbcycles) init();
  return cpufrequency;
}
