#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "constantes.h"
#include "msg_esp.h"


typedef enum{
	ST_OK=1,
	ST_MEMORY_ERROR,
	ST_UNABLE_TO_READ_FROM_FILE,
	ST_ERROR_CONVERSION,
	ST_ERROR_NULL_PTR
} state_st;

typedef struct peliculas {
	size_t id;
    char titulo[200];
    char guion [200];
    char director[200];
    time_t fecha;
    double puntaje;
    size_t reviews;
    } peli_t;


typedef enum{
    READ_BOTH_FILES,
    READ_DB,
    READ_DB_ONLY,
    READ_FILE,
    READ_FILE_ONLY,
    NO_MORE_BYTES_TO_READ
} reading_st;

struct tm* ConvertirHora(char**,struct tm*,char**);
state_st str_trim(char*, char ,char*);
int indexof(char* str, char c);
state_st del_films_array (peli_t ** films, size_t *n);
state_st split(char*, char, char***,size_t*);
void handle_error(state_st);
state_st split(char*,char,char***,size_t*);
char* strdup(char*);
void del_str_array(char**,size_t*);


size_t obtener_pos_cla(char*,int argc,const char*argv[]);
state_st ask_mem(peli_t**);
void handle_state(state_st, peli_t*, peli_t*, FILE*, FILE*, FILE*);


void imprimir_peli(peli_t *peli,FILE*);
void free_and_close(peli_t*,peli_t*,FILE*,FILE*,FILE*);

state_st peli_cpy(peli_t*,peli_t*);

void free_and_close(peli_t* file_struct,peli_t* db_struct, FILE* log, FILE* db, FILE* pf){
    free(file_struct);
    free(db_struct);
    fclose(log);
    fclose(pf);
    fclose(db);
}

void handle_error(state_st st){
    switch (st) {
        case ST_OK:
            break;
        case ST_MEMORY_ERROR:
            fprintf(stderr, "%s:%s\n",ERROR,ERROR_MEMORIA);
            break;
        case ST_UNABLE_TO_READ_FROM_FILE:
            fprintf(stderr, "%s:%s\n",ERROR,ERROR_LOAD_DB);
            break;
        case ST_ERROR_CONVERSION:
            fprintf(stderr, "%s:%s\n",ERROR,ERROR_CONVER);
            break;
        case ST_ERROR_NULL_PTR:
            fprintf(stderr, "%s:%s\n",ERROR,ERROR_MEMORIA);
            break;
        default:
            printf("%s\n", "fff");
            break;
    }
}


state_st del_films_array (peli_t ** films, size_t *n){
  if(films==NULL){
    return ST_ERROR_NULL_PTR;   
  }
  free (*films);
  *films=NULL;
  *n=0;
  return ST_OK;
}


struct tm* ConvertirHora(char** str_time,struct tm* time_info_ptr,char** endptr){

    char time_info_year[CANT_CHAR_FECHA_ANIO], time_info_mon[CANT_CHAR_FECHA_MES],time_info_day[CANT_CHAR_FECHA_DIA];
    time_info_ptr->tm_year = (strtol(strncpy(time_info_year,*str_time,CANT_CHAR_FECHA_ANIO+1),endptr,10) -1900);
    time_info_ptr->tm_mon = (strtol(strncpy(time_info_mon,*(str_time +1),CANT_CHAR_FECHA_MES+1),endptr,10) -1);
    time_info_ptr->tm_mday = strtol(strncpy(time_info_day,*(str_time +2),CANT_CHAR_FECHA_DIA+1),endptr,10);
    return time_info_ptr;
}

state_st str_trim(char * str, char c,char* aux_time){
    char* str_trimmed;
    if ((str_trimmed = (char*)malloc(indexof(str,c)+1))==NULL)
    {
        return ST_MEMORY_ERROR;
    }
    strncpy(str_trimmed,str,indexof(str,c));
    aux_time= str_trimmed;
    return ST_OK;
}

int indexof(char* str, char c){
    int i;
    for (i = 0; str[i]!=c; ++i);
    return i;
}




size_t obtener_pos_cla(char* pos_cla,int argc, const char*argv[]){
    int i,res;
    for (i = 0; i < argc; ++i)
    {
        if (strcmp(pos_cla, argv[i]) == 0)
        {
            res = i+1;
        }
    }
    return res;
}

state_st ask_mem(peli_t** ptr){
    if((*ptr = (peli_t*)calloc(CANT_ELEMENTOS_LEIDOS,sizeof(peli_t)))==NULL)
    {
        return ST_MEMORY_ERROR;
    }
    return ST_OK;
}   

/*MANEJA EL ESTADO QUE DEVUELVEN LAS FUNCIONES ask_mem y demás */
void handle_state(state_st st, peli_t* db_struct,peli_t* file_struct,FILE* log,FILE* pf,FILE* db){
    switch (st) {
        case ST_OK:
            break;
        case ST_MEMORY_ERROR:
            fprintf(stderr, "%s:%s\n",ERROR,ERROR_MEMORIA);
            free(db_struct);
            free(file_struct);
            fclose(log);
            fclose(pf);
            fclose(db);
            break;
        case ST_UNABLE_TO_READ_FROM_FILE:
            fprintf(stderr, "%s:%s\n",ERROR,ERROR_LOAD_DB);
            free(file_struct);
            free(db_struct);
            fclose(log);
            fclose(pf);
            fclose(db);
            break;
        default:
            break;
    }
}


state_st split(char* str_fields,char delim, char*** str_array,size_t* L){
    char** strings;
    size_t n,i;
    char* campo,*aux,*line;
    char sdelim[2];

    if (str_fields == NULL || str_array == NULL || L==NULL)
    {
    return ST_ERROR_NULL_PTR;
    }

    for(n=0,i=0;str_fields[i];i++){
        if(str_fields[i] == delim){
            n++;
        }
    }
    n++;
    if (((strings = (char**)malloc(n*sizeof(char*)))==NULL))
    {
        return ST_MEMORY_ERROR;
    }
    if ((line=strdup(str_fields)) == NULL){
        free(strings);
        return ST_MEMORY_ERROR;
    }
    sdelim[0]=delim;
    sdelim[1]='\0';
    i = 0;
    aux = line;
    campo =strtok(aux,sdelim);
    do
    {
        if(( strings[i] = strdup(campo)) ==NULL ){
            printf("%s\n", strings[i]);
            free(line);
            del_str_array(strings,&n);
            *str_array = NULL;
            *L = 0;
            return ST_MEMORY_ERROR;
        }
        i++;
    } while ((campo =strtok(NULL,sdelim))!=NULL);

    *str_array = strings;
    *L = n;
    free(line);
    return ST_OK;
}

char* strdup(char* str){
    char* str2;
    if((str2 = (char*)malloc(sizeof(char) * (strlen(str)+1)))==NULL){
        return NULL;
    }
    strcpy(str2,str);
    return str2;

}

void del_str_array(char** strings, size_t *L){
    int i;

    for(i = 0; i< (*L);i++){
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
    strings = NULL;
    *L = 0;
}



void imprimir_peli(peli_t* peli, FILE * pf){
    /* GENERO STRING DE FECHA */
    struct tm* fecha_peli;
    char fecha[CANT_TOTAL_CHAR_FECHA] = "";
    char fecha_anio[CANT_CHAR_FECHA_ANIO];
    char fecha_mes[CANT_CHAR_FECHA_MES];
    char fecha_dia[CANT_CHAR_FECHA_DIA];
    char sep[2] = { SEPARADOR_FECHA,'\0'};
    fecha_peli = localtime(&(peli->fecha));
    strftime(fecha_anio, 10, "%Y", fecha_peli);
    strftime(fecha_mes, 10, "%m", fecha_peli);
    strftime(fecha_dia, 10, "%d", fecha_peli);

    strcat(fecha, fecha_anio);
    strcat(fecha, sep);
    strcat(fecha, fecha_mes);
    strcat(fecha, sep);
    strcat(fecha, fecha_dia);
    /*FIN GENERO STRING DE FECHA */
    fprintf(pf, "%lu%c%s%c%s%c%s%c%s%c%f%c%lu\n", peli->id, SEPARADOR_LINEAS,peli->titulo, SEPARADOR_LINEAS , peli->guion , SEPARADOR_LINEAS , peli->director, SEPARADOR_LINEAS , fecha ,  SEPARADOR_LINEAS, peli->puntaje ,  SEPARADOR_LINEAS, peli->reviews );
}


state_st peli_cpy(peli_t* dest,peli_t* src){
    if ((dest == NULL) || (src == NULL) )
    {
        return ST_ERROR_NULL_PTR;
    }

    dest->id = src->id;
    if ((strcpy(dest->titulo,src->titulo) == NULL) || (strcpy(dest->guion,src->guion) == NULL )|| (strcpy(dest->director,src->director)) == NULL )
    {
        return ST_MEMORY_ERROR;
    }
    dest->fecha = src->fecha;
    dest->puntaje = src->puntaje;
    dest->reviews = src->reviews;
    return ST_OK;
}