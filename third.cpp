#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#define SIZE_I 2
#define SIZE_J 2
float X[SIZE_I][SIZE_J];
float S[SIZE_I][SIZE_J];
int count = 0; 
struct DATA_ {
double x;
int i;
int z;
};
typedef struct DATA_ DATA;
pthread_mutex_t lock;
double f(double x) { return x*x; }
void *calc_thr (void *arg) {
DATA* a = (DATA*) arg;
X[a->i][a->z] = f(a->x);
pthread_mutex_lock(&lock);
count ++;
pthread_mutex_unlock(&lock);
delete a;
return NULL;
}
void *input_thr(void *arg) {
DATA* a = (DATA*) arg;
printf("S[%d][%d]:", a->i, a->z);
scanf("%f", &S[a->i][a->z]);
delete a;
return NULL;
}
int main() {
pthread_t thr[ SIZE_I * SIZE_J ];
pthread_mutex_init(&lock, NULL);
DATA *arg;
for (int i=0;i<SIZE_I; i++) {
for (int z=0; z<SIZE_J; z++) {
arg = new DATA;
arg->i = i;
arg->z = z;
pthread_create (&thr[ i* SIZE_J + z ], NULL, input_thr, (void *)arg);
}
}
for(int i = 0; i < SIZE_I*SIZE_J; i++) pthread_join (thr[i], NULL);
pthread_t thread;
printf("Start calculation\n");
for (int i=0;i<SIZE_I; i++) {
for (int z=0; z<SIZE_J; z++) {
arg = new DATA;
arg->i = i;
arg->z = z;
arg->x = S[i][z];
pthread_create (&thread, NULL, calc_thr, (void *)arg);
pthread_detach(thread);
}
do {
sleep(1);
printf("finished %d threads.\n", count);
} while ( count < SIZE_I*SIZE_J);
for (int i=0;i<SIZE_I; i++) {
for (int z=0; z<SIZE_J; z++) {
printf("X[%d][%d] = %f\t", i, z, X[i][z]);
}
printf("\n");
}
pthread_mutex_destroy(&lock);
return 0;
}
}