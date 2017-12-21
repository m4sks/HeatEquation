//
// Heat Equation.
// Numerical calculation by Neumann boundary condition.
// Quick-look .gif to view animation.
// Need to prepare c, gnuplot.
// Not need to prepare any folders.
//

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
//#include <time.h>

void initialize();
void calc(int x);
void data(int t);
void plot(int t, double xrange, double yrange);
void gif(int max, double xrange, double yrange, int divider);

#define D (5.0)
#define a (0)
#define b (0.5)
#define divX (200)
#define divT (3000)
#define deltaX (0.01)  // L = deltaX * divX
#define deltaT (0.01)  // T = deltaT * divT
double xrange = 1.2;
int divider = 3;

double u[divX];
double L = deltaX * (divX + 1);
char fname[32];
FILE *f;
FILE *gid;

int main() {
  system("mkdir -p data");
  system("mkdir -p graph");
  system("mkdir -p animation");
  initialize();
  gid = popen("gnuplot", "w");
  for (int i = 0; i < divT; i++) {
    calc(i);
    if (i % divider == 0) {
      data(i);
      plot(i, L, xrange);
    }
  }
  gif(divT - 1, L, xrange, divider);
  pclose(gid);

  /* for (int j = 0; j < divT; j++) {
    for (int i = 0; i < divX; i++) {
      printf("%d:%lf\t%lf\n", j, i * deltaX, u[i]);
    }
  } */
  return 0;
}

void initialize() {
  // srand48(time(0));
  for (int j = 0; j < divX; j++) {
    // u[i][j] = -1.0 + 2.0 * drand48();
    u[j] = sin(cos(j / 3.14)) / 2 + cos(sin(j/3.14*3/2)) / 3;
  }
  u[0] = a;
  u[divX - 1] = b;
}


void calc(int x) {
  u[0] = a;
  u[divX - 1] = b;
  for (int i = 1; i < divX - 1; i++) {
    u[i] = u[i] + D * (u[i + 1] - 2 * u[i] + u[i - 1]) * divT / (divX * divX);
  }
}


void data(int t) {
  sprintf(fname, "data/data%d.txt", t);
  f = fopen(fname,"w");
  for (int i = 0; i < divX; i++) {
    fprintf(f, "%lf\t%lf\n", i * deltaX, u[i]);
  }
  fclose(f);
}

void plot(int t, double xrange, double yrange) {
  fprintf(gid, "set terminal png\n");
  fprintf(gid, "set xrange [0:%lf] \n", xrange);
  fprintf(gid, "set yrange [-%lf:%lf] \n", yrange, yrange);
  fprintf(gid, "set output 'graph/graph%d.png' \n", t);
  fprintf(gid, "plot 'data/data%d.txt' w l \n", t);
  fflush(gid);
}

void gif(int max, double xrange, double yrange, int divider) {
  fprintf(gid, "set term gif animate delay 5 optimize size 640,480\n");
  // fprintf(gid, "set term gif transparent enh animate delay 25 size 640,480\n");
  fprintf(gid, "set output 'animation/animation_0to%d_divideby%d.gif'\n", max, divider);
  fprintf(gid, "set tmargin 0\n set bmargin 0\n set rmargin 0\n set lmargin 0\n");
  fprintf(gid, "set border 0\n");
  fprintf(gid, "set format x ''\n set format y ''\n");
  fprintf(gid, "unset xtics\n unset ytics\n");
  fprintf(gid, "unset xlabel\n unset ylabel\n");
  fprintf(gid, "set size ratio -1\n");
  fprintf(gid, "set autoscale xy\n");
  for (int i = 0; i <= max; i++) {
    if (i % divider == 0) {
      fprintf(gid, "plot 'graph/graph%d.png' binary filetype=png with rgbimage \n", i);
    }
  }
  fflush(gid);
}
