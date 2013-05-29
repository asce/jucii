#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define X 0
#define Y 1
#define R 0
#define I 1



int strpos(char *haystack, char *needle);
void get_args(int argc, char* argv[], long* pixel_size,float* c_value,
              float* lower_left_eval_space, float* upper_right_eval_space);
void setColor(int i,int j,char* im_pix_ref, float oi, float or,
              float incr,float inci,float ci,float cr);

/*
void setColorRow(int tx,int row,char im[][tx], float oi, float or,
                 float incr,float inci,float ci,float cr);
*/
void setRowColor(char* buff_row, int row_size, int row_number, float oi, float or,
                 float incr,float inci,float ci,float cr);

void createRawImage(int tx,int ty,char im[][tx]);




int strpos(char *haystack, char *needle)
{
  char *p = strstr(haystack, needle);
  if (p)
    return p - haystack;
  return -1; // Not found = -1.                                                               
}

void get_args(int argc, char* argv[], long* pixel_size,float* c_value,
              float* lower_left_eval_space, float* upper_right_eval_space){

  pixel_size[X] = 256;
  pixel_size[Y] = 256;

  c_value[R] = 0.285;
  c_value[I] = 0.01;

  lower_left_eval_space[R] = -1;
  lower_left_eval_space[I] = -1;

  upper_right_eval_space[R] = 1;
  upper_right_eval_space[I] = 1;

  int p,c,i,s;
  if(argc > 2){
    p = strpos(argv[1],"p")*2;
    c = strpos(argv[1],"c")*2;
    i = strpos(argv[1],"i")*2;
    s = strpos(argv[1],"s")*2;

    if( p > 0 && p+1 < argc){
      pixel_size[X] = atol(argv[p]);
      pixel_size[Y] = atol(argv[p+1]);
    }

    if(c > 0 && c+1 < argc){
      c_value[R] = atof(argv[c]);
      c_value[I] = atof(argv[c+1]);
    }

    if(i > 0 && i+1 < argc){
      lower_left_eval_space[R] = atof(argv[i]);
      lower_left_eval_space[I] = atof(argv[i+1]);
    }
    if(s > 0 && s+1 < argc){
      upper_right_eval_space[R] = atof(argv[s]);
      upper_right_eval_space[I] = atof(argv[s+1]);
    }

  }

  printf("Los valores de los parámetros son: \n");
  printf("Tamaño del pixel (Zoom): %ld %ld \n",pixel_size[X],pixel_size[Y]);
  printf("Valor de c: %f %f \n",c_value[R],c_value[I]);
  printf("Esquina inf. izq.: %f %f \n",lower_left_eval_space[R],lower_left_eval_space[I]);
  printf("Esquina inf. der.: %f %f \n",upper_right_eval_space[R],upper_right_eval_space[I]);

}

void setColor(int i,int j,char* im_pix_ref, float oi, float or,
              float incr,float inci,float ci,float cr){
  float zr,zi;
  float zrs,zis;
  unsigned char color = 0;// colorea y limita las iteraciones de puntos del conjunto    

  zi=(float)oi+(float)j*incr;
  zr=(float)or+(float)i*inci;
  zrs=zis=(float)0; // semilla                                                
  //color=0;                                                                  

  while (zrs+zis<(float)4 && color<256)
    {
      zrs=zr*zr;
      zis=zi*zi;
      zi=(float)2*zr*zi+ci;
      zr=zrs-zis+cr;
      color++;
    }
  *im_pix_ref=color-1; // asigno el color                                     
}
/*
void setColorRow(int tx,int row,char im[][tx], float oi, float or,
		 float incr,float inci,float ci,float cr){
  int i;
  for(i = 0; i < tx; i++)
    setColor(i,row,&im[row][i],oi,or,incr,inci,ci,cr);

}
*/
void setRowColor(char* buff_row, int row_size, int row_number, float oi, float or,
                 float incr,float inci,float ci,float cr){
  //tx =  row_size;
  int i;
  for(i = 0; i < row_size; i++)
    setColor(i,row_number,&buff_row[i],oi,or,incr,inci,ci,cr);

}



void createRawImage(int tx,int ty,char im[][tx]){
  int i;
  // paso los valores de la matriz al fichero Imagen                                          
  FILE *fim;
  fim=fopen("julia.raw","wb");
  if (!fim)
    {
      printf("No se puede abrir el fichero de salida.\n");
      exit(1);
    }
  for(i=ty-1;i>=0;i--)
    fwrite(im[i],sizeof(char),tx,fim);
  fclose(fim);

}
