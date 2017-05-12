#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "constantes.h"
#include "msg_esp.h"
#include "ranking.h"




int main(int argc, char const *argv[])
{
	FILE *ptrbin;
	peli_t  *peli_imprimir;

	if (argc != CANT_PARAM_DECO)
	{
		fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR_CANT_PARAM_ERRONEA);
		return EXIT_FAILURE;
	}	
	
	
	if ((ptrbin = fopen(argv[FILE_DECO_POS],"rb")) == NULL);
	{
		fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR_DECO_ARCH_ENTRADA);
   		return EXIT_FAILURE;
	}

	if ((peli_imprimir = (peli_t*)calloc(CANT_PELI_A_IMPRIMIR,sizeof(peli_t))) == NULL)
	{
	    fprintf(stderr,"%s:%s\n",ERROR,ERROR_MEMORIA);
		fclose(ptrbin);
   		return EXIT_FAILURE;
	}

	while((fread(peli_imprimir,sizeof(peli_t),CANT_PELI_A_IMPRIMIR,ptrbin)) != CANT_PELI_A_IMPRIMIR){
		if(ferror(ptrbin)){
			fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR_LEER_BINARIO);
			fclose(ptrbin);
			free(peli_imprimir);
			return EXIT_FAILURE;
		}
		imprimir_peli(peli_imprimir,stdout);
	}
	return EXIT_SUCCESS;
}


