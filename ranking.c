
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
	size_t used_size,alloc_size;
	size_t eof_flag;

	state_st estado;

	FILE* db, *pf, *log;

	peli_t *db_struct, *file_struct, *db_temp, *aux;

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
if ((log = fopen(argv[pos_log_cla, "a"]))== NULL)
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



/*PIDO MEMORIA PARA AMBOS ESTRUCTURAS*/
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

/*PIDO MEMORIA PARA EL ARREGLO DE LA NUEVA DB*/
if ((db_temp=(peli_t*)calloc(INIT_CHOP,sizeof(peli_t)))==NULL){
	    fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR);
		free(file_struct)
		free(db_struct)
		fclose(db_temp);
		fclose(log);
		fclose(pf);
		fclose(db);
	    return EXIT_FAILURE;
	}

used_size=0;
alloc_size=INIT_CHOP;


eof_flag = 0; /*Falta definir macro para 1*/
estado = READ_BOTH_FILES;

while (!eof_flag){
	switch(estado){
		case READ_BOTH_FILES:
			if ((fread(db_struct,sizeof(peli_t),CANT_ELEMENTOS_LEIDOS,db)) =! CANT_ELEMENTOS_LEIDOS)
				{
					if(error(db))
					{
						fprintf(stderr, "%s:%s\n",ERROR,ERROR_LOAD);
						free(file_struct)
						free(db_struct)
						fclose(log);
						fclose(pf);
						fclose(db);
						return EXIT_FAILURE;	
					}
					else{/*SI no hay error significa que ya no hay más archivos en la db*/
						estado = READ_FILE_ONLY;
						break;
					}
				}

			if ((fread(file_struct,sizeof(peli_t),CANT_ELEMENTOS_LEIDOS,pf)) =! CANT_ELEMENTOS_LEIDOS)
			{
				if(error(pf))
				{
					fprintf(stderr, "%s:%s\n",ERROR,ERROR_LOAD);
					free_and_close(file_struct,db_struct,log,pf,db);
					return EXIT_FAILURE;	
				}
				else{
					/*SI no hay error significa que ya no hay más archivos en el archivo => cargó un archivo de db y leeo los que quedan*/
					if(used_size==alloc_size){
					    if ((aux=(peli_t*)realloc(db_temp,(alloc_size+CHOP_SIZE)*sizeof(peli_t))) == NULL){
					        fprintf(stderr, "%s:%s\n",ERROR,MSG_ERROR);
					        del_films_array(&db_temp,&used_size);
							free_and_close(file_struct,db_struct,log,pf,db);	
					        return EXIT_FAILURE;
					    }
					    alloc_size+=CHOP_SIZE;
					    db_temp=aux;
	      			}
	      			peli_cpy(&db_temp[used_size],db_struct); /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
	      			used_size++;
					estado = READ_DB_ONLY;
					break;
				}
			}
			if(used_size==alloc_size){
				    if ((aux=(peli_t*)realloc(db_temp,(alloc_size+CHOP_SIZE)*sizeof(peli_t))) == NULL){
				        fprintf(stderr, "%s:%s\n",ERROR,MSG_ERROR);
				        del_films_array(&db_temp,&used_size);
						free_and_close(file_struct,db_struct,log,pf,db);
				        return EXIT_FAILURE;
				    }
				    alloc_size+=CHOP_SIZE;
				    db_temp=aux;
      			}
			if (db_struct->id == file_struct->id){
				/* Si son iguales solo guardo el de la db y el otro lo mando al archivo de log*/
				fprintf(log, "%s\n", ERROR_ALTA);
				imprimir_peli(file_struct,log);
      			peli_cpy(&db_temp[used_size],db_struct); /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
      			used_size++;
      			estado = READ_BOTH_FILES;
      			break;

			}else if(db_struct->id < file_struct->id){
				/*guardo el de db y leo de nuevo db*/
      			peli_cpy(&db_temp[used_size],file_struct); /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
      			used_size++;
      			estado = READ_DB;
      			break;
			}

			else{
				/*guardo el del archivo y leo del archivo*/
      			peli_cpy(&db_temp[used_size],file_struct); /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
      			used_size++;
      			estado = READ_FILE;
      			break;
			}

		/* FIN CASE READ_BOTH_FILES */	
		case READ_DB:
			if ((fread(db_struct,sizeof(peli_t),CANT_ELEMENTOS_LEIDOS,db)) =! CANT_ELEMENTOS_LEIDOS)
			{
				if(error(db))
				{
					fprintf(stderr, "%s:%s\n",ERROR,ERROR_LOAD);
					free_and_close(file_struct,db_struct,log,pf,db);
					return EXIT_FAILURE;	
				}
				else{/*SI no hay error significa que ya no hay más archivos en la db*/
					estado = READ_FILE_ONLY;
					break;
				}
			}

			if(used_size==alloc_size){
				    if ((aux=(peli_t*)realloc(db_temp,(alloc_size+CHOP_SIZE)*sizeof(peli_t))) == NULL){
				        fprintf(stderr, "%s:%s\n",ERROR,MSG_ERROR);
				        del_films_array(&db_temp,&used_size);
						free_and_close(file_struct,db_struct,log,pf,db);
				        return EXIT_FAILURE;
				    }
				    alloc_size+=CHOP_SIZE;
				    db_temp=aux;
      			}
			if (db_struct->id == file_struct->id){
				/* Si son iguales solo guardo el de la db y el otro lo mando al archivo de log*/
				fprintf(log, "%s\n", ERROR_ALTA);
				imprimir_peli(file_struct,log);
      			peli_cpy(&db_temp[used_size],db_struct); /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
      			used_size++;
      			estado = READ_BOTH_FILES;
      			break;

			}else if(db_struct->id < file_struct->id){
				/*guardo el de db y leo de nuevo db*/
      			peli_cpy(&db_temp[used_size],file_struct); /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
      			used_size++;
      			estado = READ_DB;
      			break;
			}

			else{
				/*guardo el del archivo y leo del archivo*/
      			peli_cpy(&db_temp[used_size],file_struct); /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
      			used_size++;
      			estado = READ_FILE;
      			break;
			}


		/* FIN CASE READ_DB */	
		
		case READ_FILE:

		if ((fread(file_struct,sizeof(peli_t),CANT_ELEMENTOS_LEIDOS,pf)) =! CANT_ELEMENTOS_LEIDOS)
		{
			if(error(pf))
			{
				fprintf(stderr, "%s:%s\n",ERROR,ERROR_LOAD);
				free_and_close(file_struct,db_struct,log,pf,db);
				return EXIT_FAILURE;	
			}
			else{
				/*SI no hay error significa que ya no hay más archivos en el archivo => paso a leer de db*/
				estado = READ_DB_ONLY;
				break;
			}
		}
		if(used_size==alloc_size)
		{
		    if ((aux=(peli_t*)realloc(db_temp,(alloc_size+CHOP_SIZE)*sizeof(peli_t))) == NULL){
		        fprintf(stderr, "%s:%s\n",ERROR,MSG_ERROR);
		        del_films_array(&db_temp,&used_size);
				free_and_close(file_struct,db_struct,log,pf,db);
		        return EXIT_FAILURE;
		    }
		    alloc_size+=CHOP_SIZE;
		    db_temp=aux;
		}
		if (db_struct->id == file_struct->id){
				/* Si son iguales solo guardo el de la db y el otro lo mando al archivo de log*/
				fprintf(log, "%s\n", ERROR_ALTA);
				imprimir_peli(file_struct,log);
      			peli_cpy(&db_temp[used_size],db_struct); /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
      			used_size++;
      			estado = READ_BOTH_FILES;
      			break;

			}else if(db_struct->id < file_struct->id){
				/*guardo el de db y leo de nuevo db*/
      			peli_cpy(&db_temp[used_size],file_struct); /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
      			used_size++;
      			estado = READ_DB;
      			break;
			}

			else{
				/*guardo el del archivo y leo del archivo*/
      			peli_cpy(&db_temp[used_size],file_struct); /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
      			used_size++;
      			estado = READ_FILE;
      			break;
			}
		/* FIN CASE READ_FILE */
		
		case READ_DB_ONLY:
			if ((fread(db_struct,sizeof(peli_t),CANT_ELEMENTOS_LEIDOS,pf)) =! CANT_ELEMENTOS_LEIDOS)
			{
				if(error(pf))
				{
					fprintf(stderr, "%s:%s\n",ERROR,ERROR_LOAD);
					free_and_close(file_struct,db_struct,log,pf,db);
					return EXIT_FAILURE;	
				}
				else{
					/*SI no hay error significa que ya no hay más archivos en la db y en el archivo*/
					eof_flag = 1;
					break;
				}
			}
			if(used_size==alloc_size)
			{
			    if ((aux=(peli_t*)realloc(db_temp,(alloc_size+CHOP_SIZE)*sizeof(peli_t))) == NULL){
			        fprintf(stderr, "%s:%s\n",ERROR,MSG_ERROR);
			        del_films_array(&db_temp,&used_size);
					free_and_close(file_struct,db_struct,log,pf,db);
			        return EXIT_FAILURE;
			    }
			    alloc_size+=CHOP_SIZE;
			    db_temp=aux;
			}
			peli_cpy(&db_temp[used_size],db_struct); /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
  			used_size++;
  			estado = READ_DB_ONLY;
  			break;

		/* FIN CASE READ_DB_ONLY */
		
		case READ_FILE_ONLY:
		if ((fread(file_struct,sizeof(peli_t),CANT_ELEMENTOS_LEIDOS,pf)) =! CANT_ELEMENTOS_LEIDOS)
			{
				if(error(pf))
				{
					fprintf(stderr, "%s:%s\n",ERROR,ERROR_LOAD);
					free_and_close(file_struct,db_struct,log,pf,db);
					return EXIT_FAILURE;	
				}
				else{
					/*SI no hay error significa que ya no hay más archivos en la db y en el archivo*/
					eof_flag = 1;
					break;
				}
			}
			if(used_size==alloc_size)
			{
			    if ((aux=(peli_t*)realloc(db_temp,(alloc_size+CHOP_SIZE)*sizeof(peli_t))) == NULL){
			        fprintf(stderr, "%s:%s\n",ERROR,MSG_ERROR);
			        del_films_array(&db_temp,&used_size);
					free_and_close(file_struct,db_struct,log,pf,db);
			        return EXIT_FAILURE;
			    }
			    alloc_size+=CHOP_SIZE;
			    db_temp=aux;
			}
			peli_cpy(&db_temp[used_size],file_struct); /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
  			used_size++;
  			estado = READ_FILE_ONLY;
  			break;

		/* FIN CASE READ_FILE_ONLY */
	}
}

/* FIN ALTA*/


	

/* FIN LEO UNA VEZ LA DB Y EL ARCHIVO*/




