/*funcion split*/
t_estado split (const char *S/*string*/, char delim/*simbolo*/, char*** str_array, size_t *L)
{ 
  char **strings;
  size_t n,i; /*numero de sepradores*/
  char * campo;
  char* aux;
  char sdelim[2];
  char * line; /*la agregue, no se si esta bien, asumo que si*/

  if (s == NULL || str_array == NULL || L=NULL)
  {
    return ERROR_NULL_PTR;
  }

  for(n=1,i=0,S[i],i++)
  {
      if(S[i]==delim)
      n++;

      if (strings=(char**)malloc(n*sizeof(char*)) == NULL){
        return ERROR_NO_MEMORIA;
      }
      if ((line=strdup(S)) == NULL){ /*puede ser que haga falta declarar a line antes de igualarlo a algo*/
        free(strings);
        return ERROR_NO_MEMORIA;
      }
      sdelim[0]=delim;
      sdelim[1]='\0';
  }

  for (i=0,aux=line;(campo=strtok(aux,sdelim)) != NULL;aux=NULL,i++){
    if((strings[i]=strdup(campo)) == NULL){
      free(line);
      del_str_array(strings,i)
      *str_array=NULL;
      *L=0
    }
    return ERROR_NO_MEMORIA;
  }
  free(line);
  *L=i;
  *str_array=strings;
  return ST_OK;
}
