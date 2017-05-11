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
	FILE *ptrbin;
	peli_t  *peli_imprimir;
	size_t used_size, alloc_size, i;

	if (argc != CANT_PARAM_DECO)
	{
		fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR_CANT_PARAM_ERRONEA);
		return EXIT_FAILURE;
	}	
	
	
	if ((ptrbin = open(argv[FILE_DECO_POS],"rb")) == NULL);
	{
		fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR_DECO_ARCH_ENTRADA);
   		return EXIT_FAILURE;
	}

	if ((peli_imprimir = (peli_t*)calloc(CANT_PELI_A_IMPRIMIR,sizeof(peli_t))) == NULL)
	{
	    fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR_ALOCAR_MEMORIA);
		fclose(ptrbin);
   		return EXIT_FAILURE;
	}

	while((fread(peli_imprimir,sizeof(peli_t),CANT_PELI_A_IMPRIMIR,ptrbin)) != CANT_PELI_A_IMPRIMIR){
		if(ferror(ptrbin)){
			fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR_LEER_BINARIO);
			fclose(ptrbin);
			free(peli_imprimir);
			return EXIT_FAILURE
		}
		imprimir_peli(peli_imprimir);
	}
	return EXIT_SUCCESS;
}


void imprimir_peli(peli_t peli){
	/* GENERO STRING DE FECHA */
	struct tm fecha_peli;
	char fecha[CANT_TOTAL_CHAR_FECHA] = "";
	char fecha_anio[CANT_CHAR_FECHA_ANIO];
	char fecha_mes[CANT_CHAR_FECHA_MES];
	char fecha_dia[CANT_CHAR_FECHA_DIA];
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
 	/*FIN GENERO STRING DE FECHA */
	fprintf(STDOUT, "%lu%c%s%c%s%c%s%c%s%c%lf%c%lu\n", peli.id, SEPARADOR_LINEAS,peli.titulo, SEPARADOR_LINEAS , peli.guion , SEPARADOR_LINEAS , peli.director, SEPARADOR_LINEAS , peli.fecha ,  SEPARADOR_LINEAS, peli.puntaje ,  SEPARADOR_LINEAS, peli.reviews );
}

