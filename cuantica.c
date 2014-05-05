#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//Mètodo que copia un vector en otro

void copy(float *u_past, float *u_new, int n_points)
 {
   int i;
   for(i=0;i<n_points;i++)
     {
       u_new[i]=u_past[i];
     }
 }


//Método que separa el espacio en memoria

float *get_memory(int n_points)
 {
   float *x;
   if(!(x=malloc(sizeof(float)*n_points)))
     {
	printf("problem with memory allocation");
	exit(1);
     }
   return x;
 }

//Método Principal

int main(int argc, char **argv)
 {
   //Declaración de Constantes
   
   //Número de iteraciones en el tiempo
   int n_time=atoi(argv[1]);
   
   //Divisiones del espacio
   int n_points=1000;
   float x_min=0.0;
   float x_max=12.0;
   
   //Parámetros de la ecuacion de onda inicial
   float sigma=0.05;
   float k=16*3.14159;

   float deltax=(x_max-x_min)/n_points;
   float deltat=0.0005;

   float alpha=deltat/(2*pow(deltax,2));

   //Declaración de Variables
   
   //Contadores
   int i;
   int j;
   
   //Espacio
   float *x;
   
   //Potencial
   float *V;

   //Partes reales e imaginarias a lo largo del tiempo
   float *R_initial;
   float *I_initial;

   float *R_present;
   float *I_present;

   float *R_future;
   float *I_future;

   //Densidad de probabilidad
   float *density;

   //Alocacion de espacio en memoria
   x=get_memory(n_points);
   V=get_memory(n_points);

   R_initial=get_memory(n_points);
   I_initial=get_memory(n_points);

   R_present=get_memory(n_points);
   I_present=get_memory(n_points);

   R_future=get_memory(n_points);
   I_future=get_memory(n_points);

   density=get_memory(n_points);

   //Declaración de condiciones iniciales y de frontera

   x[0]=x_min;
   x[n_points-1]=x_max;

   //Llenamos las divisiones en el tiempo y con eso definimos el potencial a lo largo de la posición y las funciones iniciales
   for(i=1;i<n_points-1;i++)
    {
      x[i]=x[i-1]+deltax;

    }

   for(i=0;i<n_points;i++)
    {
      V[i]=pow(x[i],2)/2;

      R_initial[i]=exp(-0.5*pow((x[i]-5)/sigma,2))*cos(k*x[i]);
      I_initial[i]=exp(-0.5*pow((x[i]-5)/sigma,2))*sin(k*x[i]); 
    }
     
     
   //Redefinimos nuestras funciones para el siguiente paso 
   copy(R_initial,R_present,n_points);
   copy(I_initial,I_present,n_points);

   //Hacemos las N iteraciones a lo largo del tiempo
   for(j=0;j<n_time;j++)
    {
     for(i=1;i<n_points-1;i++) 
     {
       R_future[i]=R_present[i]-2*((alpha*(I_present[i+1]+I_present[i-1]))-(2*I_present[i]*(alpha+(deltat*V[i]))));
       I_future[i]=I_present[i]+2*((alpha*(R_present[i+1]+R_present[i-1]))-(2*R_present[i]*(alpha+(V[i]*deltat))));  
     }
      density[i]=pow(R_present[i],2)+pow(I_present[i],2);
      copy(R_future,R_present,n_points);
      copy(I_future,I_present,n_points);
    }
   
   //Ahora exportamos los datos a un archivo
   FILE *in =fopen("datos.dat","w");
      for(i=0;i<n_points;i++)
	{
	  fprintf(in,"%f %f \n",x[i],density[i]);
	}
      fclose(in);

   
   return 0;
 }
