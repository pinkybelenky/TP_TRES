#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "msg_esp.h"


void realizar_alta(char*,char*,char*);
void realizar_baja(char*,char*,char*);
void realizar_mod(char*,char*,char*);



int main(int argc, char const *argv[])
{
	switch(argv[1]){
		case "ALTA":
			realizar_alta(argv[3],argv[5],argv[7]);
			break;
		case "BAJA":
			realizar_baja(argv[3],argv[5],argv[7]);
			break;
		case "MODIFICACION":
			realizar_mod(argv[3],argv[5],argv[7]);
			break;
	}
	return 0;
}


void realizar_alta(char* db,char* falta,char* flog){
	peli_t * peli_array, peli_alta,peli_res;
	pasar_db_a_array(db,peli_array);/* genera una cadena de estructuras a partir de la base de datos*/
	pasar_arch_alta_a_array(falta,peli_alta); /* genera una cadena de estructuras a partir del archivo csv*/
	generar_alta(db,peli_array,peli_alta,flog,peli_res);
	/* Una vez que se tienen los datos de la base de datos en una cadena de estructuras y los datos del csv en 
	otra cadena de estructuras, se ejecuta generar_alta. Está función define 2 enteros y los inicializa en 0. Ambos serán
	usados como indices para recorrer las cadenas.
	El pseudo código de la función generar_alta es el siguiente:
	

	int i = 0;
	int j = 0;
	while(i != largo_cadena_db && j!= largo_cadena_csv){
		if (i == largo_cadena_db){
			//Todavía hay elementos en el array de la cadena del csv
			realocar_memoria_de_peli_res()
			copiar_elemento_j_de_peli_alta_en_la_posicion_i+j_de_peli_res()
			j++;
		}else if {(j == largo_cadena_csv)
			//Todavía hay elementos en el array de la cadena del csv
			realocar_memoria_de_peli_res()
			copiar_elemento_i_de_peli_db_en_la_posicion_i+j_de_peli_res()
			i++;
		}else{
			realocar_memoria_de_peli_res()
			leer_elemento_i_de_peli_db();
			leer_elemento_j_de_peli_alta();
			comparar_ambos_elementos();
			if()(id_elementos_son_iguales())
				guardar_elemento_j_en_cadena_peli_log()
				copiar_elemento_de_peli_db_en_peli_res();
				i++;
				j++;
			}
			copiar_elemento_de_menor_id_en_peli_res_e_incrementar_el_indice_del_array_que_se_copio_()
		}	
	}
	abrir_db_en_modo_escritura_y_escribir_la_cadena_peli_res();
	abrir_flog_en_modo_append_y_escribir_la_cadena_peli_log();
	*/
}
void realizar_baja(char* db,char* fbaja,char* flog){
	peli_t * peli_array, peli_baja,peli_res;
	pasar_db_a_array(db,peli_array);/* genera una cadena de estructuras a partir de la base de datos*/
	pasar_arch_alta_a_array(fbaja,peli_baja); /* genera una cadena de estructuras a partir del archivo csv*/
	generar_alta(db,peli_array,peli_baja,flog,peli_res);
	/* Una vez que se tienen los datos de la base de datos en una cadena de estructuras y los datos del csv en 
	otra cadena de estructuras, se ejecuta generar_alta. Está función define 2 enteros y los inicializa en 0. Ambos serán
	usados como indices para recorrer las cadenas.
	El pseudo código de la función generar_alta es el siguiente:
	

	int i = 0;
	int j = 0;
	while(i != largo_cadena_db && j!= largo_cadena_csv){
		if (i == largo_cadena_db){
			//Todavía hay elementos en el array de la cadena del csv
			guardar_elemento_j_en_cadena_peli_log()
			j++;
		}else if {(j == largo_cadena_csv)
			//Todavía hay elementos en el array de la cadena del csv
			realocar_memoria_de_peli_res()
			copiar_elemento_i_de_peli_db_en_la_posicion_i+j_de_peli_res()
			i++;
		}else{
			realocar_memoria_de_peli_res()
			leer_elemento_i_de_peli_db();
			leer_elemento_j_de_peli_baja();
			comparar_ambos_elementos();
			if(id_elemento_peli_baja_<_id_elemento_peli_db()){
				guardar_elemento_j_en_cadena_peli_log()
				copiar_elemento_de_peli_db_en_peli_res();
				i++;
				j++;
			}else if(id_elemento_peli_baja_==_id_elemento_peli_db()){
				i++;
				j++;
			}else {
				copiar_elemento_de_peli_db_en_peli_res();
				i++;
			}
		}	
	}
	abrir_db_en_modo_escritura_y_escribir_la_cadena_peli_res();
	abrir_flog_en_modo_append_y_escribir_la_cadena_peli_log();
	*/
}
void realizar_mod(char* db,char* fmod,char* flog){
	peli_t * peli_array, peli_mod,peli_res;
	pasar_db_a_array(db,peli_array);/* genera una cadena de estructuras a partir de la base de datos*/
	pasar_arch_alta_a_array(fmod,peli_baja); /* genera una cadena de estructuras a partir del archivo csv*/
	generar_alta(db,peli_array,peli_mod,flog,peli_res);
	/* Una vez que se tienen los datos de la base de datos en una cadena de estructuras y los datos del csv en 
	otra cadena de estructuras, se ejecuta generar_alta. Está función define 2 enteros y los inicializa en 0. Ambos serán
	usados como indices para recorrer las cadenas.
	El pseudo código de la función generar_alta es el siguiente:
	

	int i = 0;
	int j = 0;
	while(i != largo_cadena_db && j!= largo_cadena_csv){
		if (i == largo_cadena_db){
			//Todavía hay elementos en el array de la cadena del csv
			guardar_elemento_j_en_cadena_peli_log()
			j++;
		}else if {(j == largo_cadena_csv)
			//Todavía hay elementos en el array de la cadena del csv
			realocar_memoria_de_peli_res()
			copiar_elemento_i_de_peli_db_en_la_posicion_i+j_de_peli_res()
			i++;
		}else{
			realocar_memoria_de_peli_res()
			leer_elemento_i_de_peli_db();
			leer_elemento_j_de_peli_baja();
			comparar_ambos_elementos();
			if(id_elemento_peli_baja_<_id_elemento_peli_db()){
				guardar_elemento_j_en_cadena_peli_log()
				copiar_elemento_de_peli_db_en_peli_res();
				i++;
				j++;
			}else if(id_elemento_peli_baja_==_id_elemento_peli_db()){
				guardar_elemento_j_en_cadena_peli_res()
				i++;
				j++;
			}else {
				copiar_elemento_de_peli_db_en_peli_res();
				i++;
			}
		}	
	}
	abrir_db_en_modo_escritura_y_escribir_la_cadena_peli_res();
	abrir_flog_en_modo_append_y_escribir_la_cadena_peli_log();
	*/
}
