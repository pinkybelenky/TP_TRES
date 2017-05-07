#include <stdio.h>
#include <stdlib.h>


typedef struct peliculas {
	size_t id;
    char titulo[200];
    char guion [200];
    char director[200];
    time_t fecha;
    double puntaje;
    size_t reviews;
    } peli_t;


void ConvertirHora(char*,struct tm*);
char* str_trim(char *, char);
int indexof(char*, char);

int main(int argc, char const *argv[])
{
	if (argc != CANT_PARAM_CREAR_BASE) return EXIT_FAILURE;

	peli_t * films, * aux;
  	char * *csv_fields, **time_array,*endptr,line[MAX_STR];
 	size_t n,used_size,alooc_size;
	FILE *entrada, *salida;
	salida=fopen(argv[2],"wb");
	entrada=fopen(argv[1],"rt"); /*lo abrí porque no lo había abierto el patopatopato*/
	struct tm time_info;

	time_info.tm_sec = 0;
	time_info.tm_min = 0;
	time_info.tm_hour = 0;

	if ((films=(peli_t*)calloc(INIT_CHOP,sizeof(peli_t)))==NULL){
	    fprintf(stderr,"%s:%s\n",ERROR,MSG_ERROR);
	    fclose(entrada); /*no lo abrimos nunca se cierra igaul, así que lo abrí*/
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
	      del_person_array(&films,&used_size);
	      fclose (entrada);
	      fclose (salida);
	      return EXIT_FAILURE;
	    }

	    if(used_size==alloc_size){
	      if ((aux=(peli_t*)realloc(films,(alloc_size+CHOP_SIZE)*sizeof(peli_t))) == NULL){
	        fprintf(stderr, "%s:%s\n",ERROR,MSG_ERROR);
	        del_person_array(&films,&used_size);
	        del_str_array(&csv_fields,&n)
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
		    del_person_array(&films,&used_size);
		    del_str_array(&csv_fields,&n)
		    fclose (entrada);
		    fclose (salida);
		    return EXIT_FAILURE;
	  	} /*fin del if*/
		strcpy(films[used_size].titulo, csv_fields[TITLE_FIELD_POS]);
		strcpy(films[used_size].guion, csv_fields[SCRIPT_FIELD_POS]);
		strcpy(films[used_size].director, csv_fields[DIRECTOR_FIELD_POS]);
		films[used_size].puntaje = atof(csv_fields[SCORE_FIELD_POS]);
		films[used_size].fecha = mktime(ConvertirHora(csv_fields[TIME_FIELD_POS],&time_info));
		films[used_size].reviews = strtol(csv_fields[REVIEWS_FIELD_POS], &endptr,10); 
		if(*endptr!='\0'){
		    handle_error(st=ST_ERROR_CONVERSION); 
		    del_person_array(&films,&used_size);
		    del_str_array(&csv_fields,&n)
		    fclose (entrada);
		    fclose (salida);
		    return EXIT_FAILURE;
	  	} 

    }/*fin del while*/
    used_size++

	if(fwrite(films,sizeof(person_t),used_size,salida) != used_size){
	    fprintf(stderr, "%s:%s\n",ERROR,MSG_ERROR);
	    del_person_array(&films,&used_size);
	    del_str_array(&csv_fields,&n)
	    fclose (entrada);
	    fclose (salida);
	    return EXIT_FAILURE;
  	}

  	fclose(salida);
   	fclose(entrada);

   return EXIT_SUCCES;
}







status_t del_person_array (person_t ** films, size_t *n){
  if(films==NULL)
  return ST_ERRORNULL_PTR;

  free (*people);
  *people=NULL;
  *n=0;
  return ST_OK,
}


void ConvertirHora(char* str_time,struct tm* time_info_ptr){

	char time_info_year[4], time_info_mon[2],time_info_day[2];
	time_info_ptr->year = strtol(str_trim(str_time,'-'));
	time_info_ptr->mon = strtol(str_trim(str_time +5,'-'));
	time_info_ptr->mday = strtol(str_time +8);
}

char* str_trim(char * str, char c){
	char* str_trimmed;
	if ((str_trimmed = (char*)malloc(indexof(str,'-')+1))==NULL)
	{
		return EXIT_FAILURE;
	}
	strncpy(str_trimmed,str,indexof(str,'-'));
	str_trimmed[indexof(str,'-')+1] = '\0';
	return str_trimmed;
}

int indexof(char* str, char c){
	int i;
	for (i = 0; str[i]!=c; ++i);
	return i;
}