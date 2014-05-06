#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define FLOAT float
// Constante de la funcion de onda
#define c sqrt(0.01/40)
#define L 100.0

void initCondition( FLOAT * u, FLOAT * x0, int n_x, FLOAT dx );

void updateFunc( FLOAT * u_past, FLOAT *u_present, FLOAT *x0, int n_x, FLOAT r );

int main(int argc, char **argv){

  // Archivo para imprimir las evolucion de u
  FILE *data = fopen("Wave_Eq.dat", "w");
  
  // Numero de iteraciones en espacio y tiempo y sus respectivos dt, dx
  int n_x = atoi(argv[1]);
  int n_t = atoi(argv[2]);
  //printf("%d \n", n_x );
  FLOAT dx = L/(n_x-1);
  //printf("%f %d \n", L, n_x );
  FLOAT dt = 2.0;
  // Parámetro de estabilidad (debe ser menor que 1)
  FLOAT r = c*(dt/dx);
  printf("%f \n", r);
  
  // Vector de posicion
  FLOAT *x0 = malloc(n_x*sizeof(FLOAT));
  int j;
  for( j = 0; j < n_x; j++){
    x0[j] = j*dx;
  }

  // Vector actual de la funcion de onda
  FLOAT *present_u = malloc(n_x *sizeof(FLOAT));
  int i;
  present_u = malloc(n_x*sizeof(FLOAT));
  
  // Condicion inicial
  initCondition(present_u,x0,n_x,dx);

  // Condiciones de frontera
  present_u[0] = 0;
  present_u[n_x-1] = 0;
  
  // Vector futuro de la funcion de onda
  FLOAT *future_u = malloc(n_x *sizeof(FLOAT));
  future_u = malloc(n_x*sizeof(FLOAT));

  // Condiciones de frontera
  future_u[0] = 0;
  future_u[n_x-1] = 0;

  // Calcula el primer valor de future_u
  for( i = 1; i < n_x-1; i++){
    future_u[i] = present_u[i] + (r*r/2.0) * (present_u[i+1] - 2.0 * present_u[i] + present_u[i-1]);
  }

  // Imprime la condicion inicial ( present_ u )
  for( i = 0; i < n_x-1; i++){
    fprintf(data, "%f ", (float) present_u[i]);
  }
  fprintf(data, "%f\n", (float) present_u[i]);

  // Calcula el siguiente valor de present_u y future_u
  for( j= 0; j < n_t; j++){
    updateFunc(present_u,future_u,x0,n_x,r);
    // Imprimir la funcion u
    for( i = 0; i < n_x-1; i++){
      fprintf(data, "%f ", (float) present_u[i]);
    }
    fprintf(data, "%f\n", (float) present_u[i]);
   }
  return 0;
}


/*
 * Inicializa las condiciones iniciales
 */
void initCondition( FLOAT * u, FLOAT * x0, int n_x, FLOAT dx ){
  
  int i;
  for( i = 0; i < 0.8*L/dx; i++){
    u[i] = 1.25*x0[i]/L;
  }
  //printf("%d\n", i );
  for( i = i; i < n_x; i++ ){
    u[i] = 5 - 5*x0[i]/L;
  }
}

void updateFunc( FLOAT * u_past, FLOAT *u_present, FLOAT *x0, int n_x, FLOAT r ){
  // Copia los valores (sin apuntar) del vector u_present = u_future
  FLOAT *u_future = malloc(n_x*sizeof(FLOAT));
  u_future[0] = 0;
  u_future[n_x-1] = 0;
  int i;
  for( i = 1; i < n_x-1 ; i++){
    //printf("%f\n", upast[i]);
    u_future[i] = (2.0*(1.0-r*r))*u_present[i] - u_past[i] + (r*r)*(u_present[i+1] +  u_present[i-1]);
  }
  memcpy( u_past, u_present, n_x*sizeof(FLOAT));
  memcpy( u_present, u_future, n_x*sizeof(FLOAT));
  free(u_future);
}
