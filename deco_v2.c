#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct peliculas {
	size_t id;
	char titulo[200];
	char guion [200];
	char director[200];
	time_t fecha;
	double puntaje;
	size_t reviews;
} peli_t;

void imprimir_peli(peli_t peli);

int main(int argc, char const *argv[])
{
	/* PASO LOS DATOS DEL BINARIO A UN ARRAY DE LA ESTRUCTURA. ESTO PROBABLEMENTE LO NECESITEMOS COMO FUNCIÖN MÄS ADELANTE*/
	FILE *ptrbin;
	peli_t  *peli_array;
	size_t used_size, alloc_size, i;

	if (argc != CANT_PARAM_DECO) return EXIT_FAILURE;
	

	if ((ptrbin = open(argv[1],"rb")) == NULL);
	{
		fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR_DECO_ARCH_ENTRADA);
   		return EXIT_FAILURE;
	}

	if ((peli_array = (peli_t*) calloc(INIT_CHOP, sizeof(peli_t))) == NULL)
	{
	    fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR);
		fclose(ptrbin);
   		return EXIT_FAILURE;
	}
	used_size = 0;
	alloc_size = INIT_CHOP;

	while(!feof(ptrbin)){
		if ((fread(peli_array + used_size, sizeof(peli_t), 1, ptrbin)) != 1)
		{	
			fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR_UNABLE_TO_READ_FROM_BINARY);
			fclose(ptrbin);
			free(peli_array)
	   		return EXIT_FAILURE
		}
		used_size++;
		if (used_size==alloc_size)
		{
			
			if ((peli_array = (peli_t*) realloc(peli_array,alloc_size + INIT_CHOP)) == NULL)
			{
			    fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR);
				fclose(ptrbin);
		   		return EXIT_FAILURE;
			}
			alloc_size =+ INIT_CHOP;
		}
	}/* FIN WHILE Y CARGADA ARRAY DE ESTRUCTURA CON DATOS DEL ARCHIVO BINARIO */

	/*IMPRIMIR ESTRUCTURAS POR STDOUT*/
	for (i = 0; i < used_size +1; ++i)
	{
		imprimir_peli(peli_array[i]);
	}
	return 0;
}


void imprimir_peli(peli_t peli){
	struct tm fecha_peli;
	char fecha[10] = "";
	char fecha_anio[4];
	char fecha_mes[2];
	char fecha_dia[2];
	char sep[2] = { SEPARADOR_FECHA,'\0'};
	fecha_peli = localtime(peli.fecha);
	strftime(fecha_anio, 10, "%Y", fecha_peli);
	strftime(fecha_mes, 10, "%m", fecha_peli);
	strftime(fecha_dia, 10, "%d", fecha_peli);

	strcat(fecha, fecha_anio);
	strcat(fecha, sep);
	strcat(fecha, fecha_mes);
	strcat(fecha, sep);
	strcat(fecha, fecha_dia);
 
	fprintf(STDOUT, "%lu%c%s%c%s%c%s%c%s%c%lf%c%lu\n", peli.id, SEPARADOR_LINEAS,peli.titulo, SEPARADOR_LINEAS , peli.guion , SEPARADOR_LINEAS , peli.director, SEPARADOR_LINEAS , peli.fecha ,  SEPARADOR_LINEAS, peli.puntaje ,  SEPARADOR_LINEAS, peli.reviews );
}

