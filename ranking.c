
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "constantes.h"
#include "ranking.h"




int main(int argc, char const *argv[])
{

	/* defino variables donde va a ir la posición de cada archivo dentro de la cla*/
	size_t pos_db_cla, pos_file_cla, pos_log_cla; 

	size_t used_size,alloc_size;

	state_st st;
	reading_st estado;

	FILE* db, *pf, *log;

	peli_t *db_struct, *file_struct, *db_temp, *aux;
	/*VERIFICO QUE LA CANTIDAD DE PARAMETROS SEA LA CORRECTA*/
	if (argc != CANT_TOTAL_PARAM_RANK)
	{
		fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR_CANT_PARAM_ERRONEA);
		return EXIT_FAILURE;
	}

	/*OBTENGO LAS POSICIONES DE LOS ARCHIVOS DENTRO DE LA CLA*/
	pos_db_cla = obtener_pos_cla("if",argc,argv);/*falta definir macro */
	pos_file_cla = obtener_pos_cla("f",argc,argv);/*falta definir macro */
	pos_log_cla = obtener_pos_cla("log",argc,argv);/*falta definir macro */

	/*ABRO ARCHIVOS*/
	if ((db = fopen(argv[pos_db_cla], "rb"))== NULL)
	{
		fprintf(stderr, "%s:%s\n",ERROR, ERROR_OPEN_DB);
		return EXIT_FAILURE;
	}
	if ((pf = fopen(argv[pos_file_cla], "rb"))== NULL)
	{
		fprintf(stderr, "%s:%s\n",ERROR, ERROR_OPEN_FILE);
		return EXIT_FAILURE;
	}
	if ((log = fopen(argv[pos_log_cla], "a"))== NULL)
	{
		fprintf(stderr, "%s:%s\n",ERROR, ERROR_OPEN_LOG);
		return EXIT_FAILURE;
	}
	/*FIN ABRO ARCHIVOS*/

	/*PIDO MEMORIA PARA AMBOS ESTRUCTURAS*/
	if((db_struct = (peli_t*)calloc(CANT_ELEMENTOS_LEIDOS,sizeof(peli_t)))==NULL){
		fprintf(stderr, "%s:%s\n",ERROR,ERROR_MEMORIA);
		fclose(log);
		fclose(pf);
		fclose(db);
		return EXIT_FAILURE;
	}

	if((file_struct = (peli_t*)calloc(CANT_ELEMENTOS_LEIDOS,sizeof(peli_t)))==NULL){
		fprintf(stderr, "%s:%s\n",ERROR,ERROR_MEMORIA);
		free_and_close(file_struct,db_struct,log,pf,db);
		return EXIT_FAILURE;
	}

	/*FIN PIDO MEMORIA PARA AMBOS ESTRUCTURAS POR PRIMERA VEZ*/

	

	if ((strcmp(argv[POS_COMMAND_CLA], "ALTA"))==0)/*falta definir macro para "ALTA"*/
	{
		

	/***********************************************************************************************/
	/***********************************************************************************************/
	/**************************************REALIZAR_ALTA********************************************/
	/***********************************************************************************************/
	/***********************************************************************************************/


	/*PIDO MEMORIA PARA EL ARREGLO DE LA NUEVA DB*/
	if ((db_temp=(peli_t*)calloc(INIT_CHOP,sizeof(peli_t)))==NULL){
		    fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR);
			free_and_close(file_struct,db_struct,log,pf,db);
		    return EXIT_FAILURE;
		}

	used_size=0;
	alloc_size=INIT_CHOP;

	estado = READ_BOTH_FILES;

	while (1){
		switch(estado){
			case READ_BOTH_FILES:
				if ((fread(db_struct,sizeof(peli_t),CANT_ELEMENTOS_LEIDOS,db)) != CANT_ELEMENTOS_LEIDOS)
					{		
						if(ferror(db))
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

				if ((fread(file_struct,sizeof(peli_t),CANT_ELEMENTOS_LEIDOS,pf)) != CANT_ELEMENTOS_LEIDOS)
				{	
					if(ferror(pf))
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
		      			if ((st = peli_cpy(&db_temp[used_size],db_struct) != ST_OK)) /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
		      			{
		      				handle_state(st,db_struct,file_struct,log,pf,db);
		      				return EXIT_FAILURE;
		      			}

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
					if ((st = peli_cpy(&db_temp[used_size],db_struct) != ST_OK)) /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
	      			{
	      				handle_state(st,db_struct,file_struct,log,pf,db);
	      				return EXIT_FAILURE;
	      			}
					imprimir_peli(file_struct,log);
	      			used_size++;
	      			estado = READ_BOTH_FILES;
	      			break;

				}else if(db_struct->id < file_struct->id){

					/*guardo el de db y leo de nuevo db*/
	      			if ((st = peli_cpy(&db_temp[used_size],db_struct) != ST_OK)) /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
	      			{
	      				handle_state(st,db_struct,file_struct,log,pf,db);
	      				return EXIT_FAILURE;
	      			}

	      			used_size++;
	      			estado = READ_DB;
	      			break;
				}

				else{
					/*guardo el del archivo y leo del archivo*/
	      			if ((st = peli_cpy(&db_temp[used_size],file_struct) != ST_OK)) /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
	      			{
	      				handle_state(st,db_struct,file_struct,log,pf,db);
	      				return EXIT_FAILURE;
	      			}	      			
	      			used_size++;
	      			estado = READ_FILE;
	      			break;
				}

			/* FIN CASE READ_BOTH_FILES */	
			case READ_DB:
				if ((fread(db_struct,sizeof(peli_t),CANT_ELEMENTOS_LEIDOS,db)) != CANT_ELEMENTOS_LEIDOS)
				{
					if(ferror(db))
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
	      			if ((st = peli_cpy(&db_temp[used_size],db_struct) != ST_OK)) /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
	      			{
	      				handle_state(st,db_struct,file_struct,log,pf,db);
	      				return EXIT_FAILURE;
	      			}
	      			used_size++;
	      			estado = READ_BOTH_FILES;
	      			break;

				}else if(db_struct->id < file_struct->id){
					/*guardo el de db y leo de nuevo db*/
	      			if ((st = peli_cpy(&db_temp[used_size],db_struct) != ST_OK)) /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
	      			{
	      				handle_state(st,db_struct,file_struct,log,pf,db);
	      				return EXIT_FAILURE;
	      			}
	      			used_size++;
	      			estado = READ_DB;
	      			break;
				}

				else{
					/*guardo el del archivo y leo del archivo*/
	      			if ((st = peli_cpy(&db_temp[used_size],file_struct) != ST_OK)) /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
	      			{
	      				handle_state(st,db_struct,file_struct,log,pf,db);
	      				return EXIT_FAILURE;
	      			}
	      			used_size++;
	      			estado = READ_FILE;
	      			break;
				}


			/* FIN CASE READ_DB */	
			
			case READ_FILE:

				if ((fread(file_struct,sizeof(peli_t),CANT_ELEMENTOS_LEIDOS,pf)) != CANT_ELEMENTOS_LEIDOS)
				{
					if(ferror(pf))
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
		      			if ((st = peli_cpy(&db_temp[used_size],db_struct) != ST_OK)) /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
		      			{
		      				handle_state(st,db_struct,file_struct,log,pf,db);
		      				return EXIT_FAILURE;
		      			}
		      			used_size++;
		      			estado = READ_BOTH_FILES;
		      			break;

					}else if(db_struct->id < file_struct->id){
						/*guardo el de db y leo de nuevo db*/
		      			if ((st = peli_cpy(&db_temp[used_size],db_struct) != ST_OK)) /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
		      			{
		      				handle_state(st,db_struct,file_struct,log,pf,db);
		      				return EXIT_FAILURE;
		      			}
		      			used_size++;
		      			estado = READ_DB;
		      			break;
					}

					else{
						/*guardo el del archivo y leo del archivo*/
		      			if ((st = peli_cpy(&db_temp[used_size],file_struct) != ST_OK)) /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
		      			{
		      				handle_state(st,db_struct,file_struct,log,pf,db);
		      				return EXIT_FAILURE;
		      			}
		      			used_size++;
		      			estado = READ_FILE;
		      			break;
					}
				/* FIN CASE READ_FILE */
			
			case READ_DB_ONLY:
			printf("%s\n", "READ_DB_ONLY");
				if ((st = peli_cpy(&db_temp[used_size],db_struct) != ST_OK)) /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
      			{
      				handle_state(st,db_struct,file_struct,log,pf,db);
      				return EXIT_FAILURE;
      			}
      			printf("%d\n",db_temp[used_size].id );
	  			used_size++;
   				
				if ((fread(db_struct,sizeof(peli_t),CANT_ELEMENTOS_LEIDOS,pf)) != CANT_ELEMENTOS_LEIDOS)
				{	
					if(ferror(pf))
					{
						fprintf(stderr, "%s:%s\n",ERROR,ERROR_LOAD);
						free_and_close(file_struct,db_struct,log,pf,db);
						return EXIT_FAILURE;	
					}
					else{printf("%s\n", "bbbb");
						/*SI no hay error significa que ya no hay más archivos en la db y en el archivo*/
						estado = NO_MORE_BYTES_TO_READ;
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
	  			estado = READ_DB_ONLY;
	  			break;

			/* FIN CASE READ_DB_ONLY */
			
			case READ_FILE_ONLY:
			printf("%s\n", "READ_FILE_ONLY");
				if ((st = peli_cpy(&db_temp[used_size],file_struct) != ST_OK)) /*como un strcpy pero de peli_t (no hace falta pedir memoria)*/
      			{
      				handle_state(st,db_struct,file_struct,log,pf,db);
      				return EXIT_FAILURE;
      			}
	  			used_size++;
				if ((fread(file_struct,sizeof(peli_t),CANT_ELEMENTOS_LEIDOS,pf)) != CANT_ELEMENTOS_LEIDOS)
				{
					if(ferror(pf))
					{
						fprintf(stderr, "%s:%s\n",ERROR,ERROR_LOAD);
						free_and_close(file_struct,db_struct,log,pf,db);
						return EXIT_FAILURE;	
					}
					else{
						/*SI no hay error significa que ya no hay más archivos en la db y en el archivo*/
						estado = NO_MORE_BYTES_TO_READ;
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
				
	  			estado = READ_FILE_ONLY;
	  			break;
	  		case NO_MORE_BYTES_TO_READ:
	  		/* CIERRO DB, LA ABRO EN MODO ESCRITURA Y GUARDO db_temp*/
	  			printf("%s\n", "AAA");
	  			fclose(db);
	  			if ((db = fopen(argv[pos_db_cla], "wb"))== NULL)
				{
					fprintf(stderr, "%s:%s\n",ERROR, ERROR_OPEN_DB);
					del_films_array(&db_temp,&used_size);
					free(db_struct);
					free(file_struct);
					fclose(log);
					fclose(pf);
					return EXIT_FAILURE;
				}
				if ((fwrite(db_temp, sizeof(peli_t),used_size,db)) != used_size)
				{
					fprintf(stderr, "%s:%s\n",ERROR, ERROR_WRITE_FILE);
					del_films_array(&db_temp,&used_size);
					free_and_close(file_struct,db_struct,log,pf,db);
					return EXIT_FAILURE;
				}
				free_and_close(file_struct,db_struct,log,pf,db);

				return EXIT_SUCCESS;

			/* FIN CASE READ_FILE_ONLY */
		}
	}
	/***********************************************************************************************/
	/***********************************************************************************************/
	/****************************************** FIN ALTA********************************************/
	/***********************************************************************************************/
	/***********************************************************************************************/



	}
	else if ((strcmp(argv[POS_COMMAND_CLA], "BAJA"))==0)/*falta definir macro para "BAJA"*/
	{
		/*realizar_baja();*/
	}
	else if ((strcmp(argv[POS_COMMAND_CLA], "MODIFICAR"))==0)/*falta definir macro para "MODIFICAR"*/
	{
		/*realizar_mod();*/
	}
	else{
		fprintf(stderr, "%s:%s\n",ERROR, ERROR_INVALID_COMMAND);
		free_and_close(file_struct,db_struct,log,pf,db);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}





