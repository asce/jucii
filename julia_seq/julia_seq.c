#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Julia.h"


int main(int argc, char* argv[]) {
	
        long pixel_size[2];
	float c_value[2];
	float lower_left_eval_space[2];
	float upper_right_eval_space[2];

	get_args(argc, argv, pixel_size, c_value,
		 lower_left_eval_space, upper_right_eval_space);

	long tx = pixel_size[X], ty = pixel_size[Y];
	// tamaño en pixel de la imagen (equivale al zoom)
	float cr = c_value[R], ci = c_value[I];		
	// Valor de c
	float or = lower_left_eval_space[R], oi = lower_left_eval_space[I];
	// Esq inferior izquierda del espacio a evaluar (encuadre)
	float dr = upper_right_eval_space[R], di = upper_right_eval_space[I];
	// Esq superior derecha del espacio a evaluar (encuadre)

	float tamr=dr-or;
	float tami=di-oi;
	float incr=tamr/(float)tx;
	float inci=tami/(float)ty;
	
	int i,j;
	unsigned char color;
	double secs;
	
	
	// inicializacion estatica de la imagen		
	char im[ty][tx];
		
	clock_t comienzo=clock();
	for (j=0;j<ty;j++)
	     for (i=0;i<tx;i++)
	       setColor(i,j,&im[j][i],oi,or,incr,inci,ci,cr);

	 printf("Tiempo Conjunto Julia: %li milisegundos\n",(clock()-comienzo)*1000/CLOCKS_PER_SEC);
	 createRawImage(tx,ty,im);
	 
	return EXIT_SUCCESS;
}


