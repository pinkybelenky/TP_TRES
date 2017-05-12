
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "constantes.h"
#include "ranking.h"




int main(int argc, char const *argv[])
{
	/* defino variables donde va a ir la posición de cada archivo dentro de la cla*/
	size_t pos_db_cla, pos_file_cla, pos_log_cla; 

	size_t cant_total_elem; /*se va a usar para la cantidad total de elmentos en la db cuando termina el programa*/

	size_t eof_flag;

	state_st estado;

	FILE* db, pf, log, db_temp;

	peli_t *db_struct, *file_struct;

	if (argc != CANT_TOTAL_PARAM_RANK)
	{
		fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR_CANT_PARAM_ERRONEA);
		return EXIT_FAILURE;
	}

	pos_db_cla = obtener_pos_cla("if",argc,argv);/*falta definir macro */
	pos_file_cla = obtener_pos_cla("f",argc,argv);/*falta definir macro */
	pos_log_cla = obtener_pos_cla("log",argc,argv);/*falta definir macro */

	if ((strcmp(argv[POS_COMMAND_CLA], "ALTA"))==0)/*falta definir macro para "ALTA"*/
	{
		realizar_alta();
	}
	else if ((strcmp(argv[POS_COMMAND_CLA], "BAJA"))==0)/*falta definir macro para "BAJA"*/
	{
		realizar_baja();
	}
	else if ((strcmp(argv[POS_COMMAND_CLA], "MODIFICAR"))==0)/*falta definir macro para "MODIFICAR"*/
	{
		realizar_mod();
	}
	else{
		fprintf(stderr, "%s:%s\n",ERROR, ERROR_INVALID_COMMAND);
		return EXIT_FAILURE;
	}
	return 0;
}


/*REALIZAR_ALTA*/
char db_temp_name[] = "db_temp.bin"; /*falta definir macro */

/*ABRO ARCHIVOS*/
if ((db = fopen(argv[pos_db_cla, "rb"]))== NULL)
{
	fprintf(stderr, "%s:%s\n",ERROR, ERROR_OPEN_DB);
	return EXIT_FAILURE;
}
if ((pf = fopen(argv[pos_file_cla, "rb"]))== NULL)
{
	fprintf(stderr, "%s:%s\n",ERROR, ERROR_OPEN_FILE);
	return EXIT_FAILURE;
}
if ((log = fopen(argv[pos_log_cla, "w"]))== NULL)
{
	fprintf(stderr, "%s:%s\n",ERROR, ERROR_OPEN_LOG);
	return EXIT_FAILURE;
}
if ((db_temp = fopen(db_temp_name, "wb"]))== NULL)
{
	fprintf(stderr, "%s:%s\n",ERROR, ERROR_INTERNO);
	return EXIT_FAILURE;
}
/*FIN ABRO ARCHIVOS*/



/*PIDO MEMORIA PARA AMBOS ESTRUCTURAS POR PRIMERA VEZ*/
if((db_struct = (peli_t*)calloc(CANT_ELEMENTOS_LEIDOS,sizeof(peli_t)))==NULL){
	fprintf(stderr, "%s:%s\n",ERROR,ERROR_MEMORIA);
	fclose(db_temp);
	fclose(log);
	fclose(pf);
	fclose(db);
	return EXIT_FAILURE;
}

if((file_struct = (peli_t*)calloc(CANT_ELEMENTOS_LEIDOS,sizeof(peli_t)))==NULL){
	fprintf(stderr, "%s:%s\n",ERROR,ERROR_MEMORIA);
	free(db_struct)
	fclose(db_temp);
	fclose(log);
	fclose(pf);
	fclose(db);
	return EXIT_FAILURE;
}

/*FIN PIDO MEMORIA PARA AMBOS ESTRUCTURAS POR PRIMERA VEZ*/

/* LEO UNA VEZ LA DB Y EL ARCHIVO*/
if ((fread(db_struct,sizeof(peli_t),CANT_ELEMENTOS_LEIDOS,db)) =! CANT_ELEMENTOS_LEIDOS)
{
	if(error(db))
	{
		fprintf(stderr, "%s:%s\n",ERROR,ERROR_LOAD);
		free(file_struct)
		free(db_struct)
		fclose(db_temp);
		fclose(log);
		fclose(pf);
		fclose(db);
		return EXIT_FAILURE;	
	}
	/* SI NO CARGO 1 ELEMENTO Y NO HUBO ERROR SIGNIFICA QUE NO HAY DATOS EN LA BASE
	ENTONCES SOLO HAY QUE COPIAR LOS BINARIOS DEL ARCHIVO EN LA BASE
}
	

/* FIN LEO UNA VEZ LA DB Y EL ARCHIVO*/




