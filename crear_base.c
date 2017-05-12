#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "constantes.h"
#include "msg_esp.h"
#include "ranking.h"


int main(int argc, char const *argv[])
{
	
	state_st st;
	peli_t *films, *aux;
  	char **csv_fields,*endptr,line[MAX_STR];
 	size_t n,used_size,alloc_size;
	FILE *entrada, *salida;
	struct tm time_info;

	salida=fopen(argv[POS_CREAR_BASE_DB],"wb");
	entrada=fopen(argv[POS_CREAR_BASE_CSV],"rt");
	

	time_info.tm_sec = 0;
	time_info.tm_min = 0;
	time_info.tm_hour = 0;

	if (argc != CANT_PARAM_CREAR_BASE) return EXIT_FAILURE;

	if ((films=(peli_t*)calloc(INIT_CHOP,sizeof(peli_t)))==NULL){
	    fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR);
	    fclose(entrada); 
	    fclose(salida);
	    return EXIT_FAILURE;
	}

	used_size=0;
	alloc_size=INIT_CHOP;

	while (fgets(line,MAX_STR,entrada) != NULL){

	    if ((endptr=strrchr(line,'\n')) != NULL)
	    *endptr='\0';

	    if ((st=split(line,',',&csv_fields,&n)) != ST_OK){
	      handle_error(st);
	      del_films_array(&films,&used_size);
	      fclose (entrada);
	      fclose (salida);
	      return EXIT_FAILURE;
	    }

	    if(used_size==alloc_size){
	      if ((aux=(peli_t*)realloc(films,(alloc_size+CHOP_SIZE)*sizeof(peli_t))) == NULL){
	        fprintf(stderr, "%s:%s\n",ERROR,MSG_ERROR);
	        del_films_array(&films,&used_size);
	        del_str_array(&csv_fields,&n);
	        fclose (entrada);
	        fclose (salida);
	        return EXIT_FAILURE;
	      }
	      alloc_size+=CHOP_SIZE;
	      films=aux;
      	}

      	/* PASO TODOS LOS DATOS DE csv_fields a el lugar de films */
	    films[used_size].id =strtol(csv_fields[ID_FIELD_POS], &endptr,10);
	    if(*endptr!='\0'){
		    handle_error(st=ST_ERROR_CONVERSION); /* Todo este bloque es para checkear que la conversion salió bien*/  
		    del_films_array(&films,&used_size);
		    del_str_array(&csv_fields,&n);
		    fclose (entrada);
		    fclose (salida);
		    return EXIT_FAILURE;
	  	} /*fin del if*/
		strcpy(films[used_size].titulo, csv_fields[TITLE_FIELD_POS]);
		strcpy(films[used_size].guion, csv_fields[SCRIPT_FIELD_POS]);
		strcpy(films[used_size].director, csv_fields[DIRECTOR_FIELD_POS]);
		films[used_size].puntaje = atof(csv_fields[SCORE_FIELD_POS]);
		films[used_size].fecha = mktime(ConvertirHora(csv_fields[TIME_FIELD_POS],&time_info, &endptr));
		films[used_size].reviews = strtol(csv_fields[REVIEWS_FIELD_POS], &endptr,10); 
		if(*endptr!='\0'){
		    handle_error(st=ST_ERROR_CONVERSION); 
		    del_films_array(&films,&used_size);
		    del_str_array(&csv_fields,&n);
		    fclose (entrada);
		    fclose (salida);
		    return EXIT_FAILURE;
	  	} 

    }/*fin del while*/
    used_size++;

	if(fwrite(films,sizeof(peli_t),used_size,salida) != used_size){
	    fprintf(stderr, "%s:%s\n",ERROR,MSG_ERROR);
	    del_films_array(&films,&used_size);
	    del_str_array(&csv_fields,&n);
	    fclose (entrada);
	    fclose (salida);
	    return EXIT_FAILURE;
  	}

  	fclose(salida);
   	fclose(entrada);

   return EXIT_SUCCESS;
}


