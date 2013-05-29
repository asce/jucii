#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Julia.h"
#include "mpi.h"

#define ROW_NUMBER 0
#define ROW_CONTENT 1
#define FINISH 2

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
	int processes_number;
	int rank;
	//int p;

	int rows_number = ty;
	int row_size = tx;
	int row;
	int sent_rows = 0;
	int written_rows = 0;
	int source;
	
	MPI_Status status;
	// inicializacion estatica de la imagen		
	//char im[ty][tx];
	char** im;
	//char row_buff[row_size];
	char* row_buff; 

	mallocImage(&im,ty,tx);
	mallocImageRow(&row_buff,row_size);




	int row_to_write;
	int finished = 0;
	secs = MPI_Wtime();
	
	if(MPI_Init(&argc,&argv) != MPI_SUCCESS){
	  printf("Error al inicializar MPI\n");
	  exit(0);
	}

	MPI_Comm_size(MPI_COMM_WORLD,&(processes_number));
	MPI_Comm_rank(MPI_COMM_WORLD,&(rank));

	if(rank==0){//MASTER
	  //Reparto inicial de filas a los procesos
	  for(i = 1; i < processes_number && i < rows_number; i++ ){
	    row = i;
	    MPI_Send(&row,1,MPI_INT,i,ROW_NUMBER,MPI_COMM_WORLD);
	    sent_rows++;
	  }
	  //Recibir filas coloreadas y mandar nuevas tareas de filas a los procesos
	  while(written_rows < rows_number){
	    //Recibe numero de fila
	    MPI_Recv(&row,1,MPI_INT,MPI_ANY_SOURCE,ROW_NUMBER,MPI_COMM_WORLD,&status);
	    source = status.MPI_SOURCE;
	    row_to_write = row;
	    //Recibe fila de la fuente 'source'
	    MPI_Recv(row_buff,row_size,MPI_CHAR,source,ROW_CONTENT,MPI_COMM_WORLD,&status);
	    //Si quedan filas por enviar mando más filas
	    if(sent_rows < rows_number){
	      row = sent_rows;
	      MPI_Send(&row,1,MPI_INT,source,ROW_NUMBER,MPI_COMM_WORLD);
	      sent_rows++;
	    }
	    //Incluyo en la matrix de la imagen la fila recibida
	    memcpy(im[row_to_write],row_buff,sizeof(char)*row_size);
	    written_rows++;
	    //printf("Se ha escrito la fila %i.\n",row_to_write);
            //printf("Hemos escrito %i filas.\n",written_rows); 

	  }
          for(i = 1; i < processes_number; i++ ){
            row = -1;
            MPI_Send(&row,1,MPI_INT,i,FINISH,MPI_COMM_WORLD);
          }

	}else{//SLAVE
	  //Mientras no me llegue mensaje de fin voy coloreando y enviando las filas que me manden
	  while(finished == 0){
	    //printf("Proceso %i esperando tarea\n",rank);
	    MPI_Recv(&row,1,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
	    //printf("El proceso %i va a colorear la fila %i\n",rank,row);
	    if(status.MPI_TAG == FINISH){
	      finished = 1;
	      //printf("El proceso %i ha recibido señal de fin.\n",rank);
	    }else{//status.MPI_TAG == ROW_NUMBER
	      if(status.MPI_TAG != ROW_NUMBER){
		printf("WARNING: Se esperaba TAG ROW_NUMBER y se ha encontrado %i\n",
		       status.MPI_TAG);
		getchar();
	      }
	      setRowColor(row_buff,row_size,row,oi,or,incr,inci,ci,cr);
	      MPI_Send(&row,1,MPI_INT,0,ROW_NUMBER,MPI_COMM_WORLD);
	      MPI_Send(row_buff,row_size,MPI_CHAR,0,ROW_CONTENT,MPI_COMM_WORLD);
	    }
	  }
	}

		
	//for (j=0;j<ty;j++)
	//setRowColor(im[j],tx,j,oi,or,incr,inci,ci,cr);

	MPI_Finalize();

	if(rank==0){
	  secs = MPI_Wtime() - secs;
	  printf("Tiempo Conjunto Julia: %f milisegundos\n",secs*1000);
	  createRawImage(tx,ty,im);
	}

	freeImage(&im,ty);
	freeImageRow(&row_buff);


	return EXIT_SUCCESS;
}

