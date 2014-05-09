#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// Definiciones

// Rango, numero de puntos y diferencial ---> espacial
#define nx 20000
#define x_max -70.0
#define x_min 70.0
#define dx (x_max-x_min)/nx

// Constantes de la funcion de onda inicial
#define sigma 0.05
#define k 16*3.14159

// diferencial ---> temporal
#define dt dx*dx/4.0

// diferencial total del algoritmo
#define alpha dt/(2*dx*dx)

// Precision de los datos
#define FLOAT double

// La frecuencia con la que se imprimen los datos
#define imprx 1000
#define impr 100

void initCondition( FLOAT * Re_p, FLOAT *Im_p, FLOAT * x0, int n_x );
FLOAT V( FLOAT x );
void updateFunc( FLOAT * X, FLOAT *Y, FLOAT *x0, int n_x, FLOAT sign );


int main( int argc, char** argv ){
  
  printf("%f %f\n", alpha , dt);

  //---------------------------
  // Declaración de Constantes
  //---------------------------
   
  //Número de iteraciones en el tiempo
  int nt = atoi(argv[1]);
  
  // Archivo de texto de evolucion 
  FILE *in = fopen("datos.dat","w");
  
  // Contadores
  int i;
  int j;
  
  // Espacio
  FLOAT *x0 = malloc(nx*sizeof(FLOAT));

  // Funcion de onda actual
  FLOAT *Im_p = malloc(nx*sizeof(FLOAT));
  FLOAT *Re_p = malloc(nx*sizeof(FLOAT));

  //-------------------------------
  // Solucion de la ED
  //-------------------------------
  
  // Llena el vector de posicion
  for( i = 0; i < nx; i++ ){
    x0[i] = x_min + i*dx;
  }

  // Condicion inicial ( y de frontera )
  initCondition( Re_p, Im_p, x0, nx );
  
  // Iteracion sobre el tiempo (se tiene que doblar el numero de pasos debido a la definicion de Re e Im en enteros y semienteros de tiempo)
  for( j = 0; j < 2*nt; j++ ){
    //printf("%d\n", j);
    // Actualiza los vectores dependiendo del step de tiempo
    if( j%2 == 0 ){
      // Copia RE
      FLOAT* tempR = malloc(nx*sizeof(FLOAT));
      memcpy( tempR , Re_p, nx*sizeof(FLOAT) );
      // Calcula el siguiente paso de tiempo para Re
      updateFunc( Re_p, Im_p, x0, nx, 1.0 ); 
      // imprime ro (IM) lolol
      FLOAT ro = 0;
      if( j%impr == 0 ){
	fprintf(in, "%f %f ", (float) x0[0], 0.0); 
      }
      for( i = 1; i < nx-1; i += (int) (nx-1)/imprx ){
	ro = Im_p[i]*Im_p[i] + tempR[i]*Re_p[i];
	if( j%impr == 0 ){
	  //printf("%f\n", ro, (float) ro);
	  fprintf(in, "%f %f ", (float) x0[i], (float) ro); 
	}   
      }
      if( j%impr == 0 ){
	fprintf(in, "%f %f\n", (float) x0[nx-1], 0.0); 
      }
      // Liberar memoria de la copia
      free( tempR );
    }
    else{
      // Copia IM
      FLOAT* tempI = malloc(nx*sizeof(FLOAT));
      memcpy( tempI , Im_p, nx*sizeof(FLOAT) );
      // Calcula el siguiente paso de tiempo para Im
      updateFunc( Im_p, Re_p, x0, nx, -1.0 ); 
      // imprime ro (RE) lolol
      FLOAT ro = 0;
      if( j%impr == 0 ){
	fprintf(in, "%f %f ", (float) x0[0], 0.0); 
      }
      for( i = 1; i < nx-1;  i += (int) (nx-1)/imprx ){
	ro = Re_p[i]*Re_p[i] + tempI[i]*Im_p[i];
	if( j%impr == 0 ){
	  fprintf(in, "%f %f ", (float) x0[i], (float) ro); 
	}
      }   
      if( j%impr == 0 ){
	fprintf(in, "%f %f\n", (float) x0[nx-1], 0.0); 
      }
      // Liberar memoria de la copia
      free( tempI );
    }
  }
  return 0;
}


/*
 * Inicializa las condiciones iniciales
 */
void initCondition( FLOAT * Re_p, FLOAT *Im_p, FLOAT * x0, int n_x ){
  
  int i;
  Re_p[0] = 0; 
  Re_p[n_x-1] = 0;
  Im_p[0] = 0; 
  Im_p[n_x-1] = 0;
  for( i = 1; i < n_x-1 ; i++){
    Re_p[i] = exp(-0.5*pow((x0[i]-5)/sigma,2))*cos(k*x0[i]);
    Im_p[i] = exp(-0.5*pow((x0[i]-5)/sigma,2))*sin(k*x0[i]);
  }
}

/*
 * Retorna el potencial en un punto espacial dado
 */
 FLOAT V( FLOAT x ){
   return 0.5*(x*x);
 }


/*
 * Calcula el siguiente step de tiempo en la ecuacion diferencial
 */ 
void updateFunc( FLOAT * X, FLOAT *Y, FLOAT *x0, int n_x, FLOAT sign ){
  // Copia los valores (sin apuntar) del vector u_present = u_future
  FLOAT *X_f = malloc(n_x*sizeof(FLOAT));
  X_f[0] = 0;
  X_f[n_x-1] = 0;
  int i;
  for( i = 1; i < n_x-1 ; i++){
    X_f[i] = X[i] + -sign*2*( alpha*( Y[i+1] + Y[i-1]) - 2*( alpha + dt*V(x0[i]) )*Y[i] );
    //printf("%f\n", X[i]);
  }
  memcpy( X, X_f, n_x*sizeof(FLOAT));
  free(X_f);
}
 
