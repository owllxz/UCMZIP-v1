/*
	Autor: Luis Sepulveda Martinez - Rut: 19871495-k
	Autor: Omar Olivares Urrutia - Rut: 17.603.163-8
	Estructuras de datos utilizadas listas enlzadas(low_list.c) y arboles binarios(arbol.c).
	La forma de como se codifico el archivo .txt, se obtuvo la cantidad de caracteres el cual este tenia, luego se 
	volvio a recorrer dicho archivo utilizando el valor anteriormente extraido y se guardo en un arreglo, luego se procedio a contar la cantidad de apareciones
	de cada caracter y esos datos posteriormente se almacenaron en una lista enlazada, luego se procedio a confecionar el arbol de huffman, luego de una vez
	tener realizado el proceso de arbol de huffman, se procedio a obtener la codificacion de cada caracter, luego cada codificacion fue almacenada en un archivo .uzp en la segunda
	linea del archivo segun el orden correspondiente de aparicion de cada caracter en el archivo original, en la primera del archivo fueron guardados los datos necesarios para recrear el 
	arbol en un posterior proceso de decodificado.
	La forma de decodficar el archivo .uzp fue extrar toda la primera linea de dicho archivo y almacenando caracter y frecuencia, luego todos estos datos fueron almacenados en una lista 
	enlazada y se volvio a reutilizar el proceso de construcion del arbol, luego extraimos la segunda linea y la recorrimos junto con el arbol, 0 para izquierda y 1 para derecha y en caso de
	que este arbol apuntace a nulo por izquierda y derecha guardasemos el caracter en un achivo con extencion .txt
*/

//Hay comentarios que son anteriores a solucionar el problema, hay problemas que estan en los comentarios que ya fueron solucionados, como el caso de que no funcionaba cuando solo habia un caracter

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "low_list.c"
#include "arbol.c"

#define MAX_NAME 100

struct my{
	char cadena[6];
};
void abrir_archivo(char NAME_FILE[],char NAME_OUTFILE[]);
void verif_argumento(int cantidad_argumentos,char NAME_FILE[]);
void verif_extencion(char NAME_FILE[],int puntos_original);
void buscar_punto(char NAME_FILE[]);
void compresion(char NAME_FILE[], int puntos_original);
void descompresion(char NAME_FILE[], int puntos_original);
void contar_caracteres(FILE *archivo,char NAME_OUTFILE[]);
void copiar_caracteres(FILE *archivo, char arreglo[]);
void lista_frecuencia(char arreglo[], int cantidad_caracteres,char NAME_OUTFILE[]);
int frecuencia_caracter(char arreglo[], char c, int cantidad_caracteres);
void llenar_arbol(lista *list, char arreglo[],int cantidad_caracteres,char NAME_OUTFILE[]);
void buscar_c(arbol *a, char caracter, char almacenar, char bin[], int i, char fixed[]);
void asignar_codigo(lista *char_frec, arbol *arbol_actual, char arreglo[],int cantidad_caracteres,char NAME_OUTFILE[]);
void abrir_archivo_des(char NAME_FILE[],char NAME_OUTFILE[]);
void re_arbol(lista *dLista, int max_char, char NAME_FILE[],char NAME_OUTFILE[]);
void re_palabra(arbol *dArbol, int max_char, char NAME_FILE[],char NAME_OUTFILE[], lista *dLista);
void re_file(arbol *dArbol, char NAME_FILE[], char dArreglo[],char NAME_OUTFILE[], lista *dLista);

int main(int argc, char *argv[]){
	char NAME_FILE[MAX_NAME];
	memset(NAME_FILE, '\0', MAX_NAME);
	strcpy(NAME_FILE, argv[1]);

	verif_argumento(argc,NAME_FILE);
	return 0;
}
void abrir_archivo(char NAME_FILE[],char NAME_OUTFILE[]){
	FILE *archivo;
	archivo = fopen(NAME_FILE, "r");
	if(archivo == NULL){
		printf("Error al abrir el archivo\n");
	}
	contar_caracteres(archivo,NAME_OUTFILE);
	fclose(archivo);
}
void contar_caracteres(FILE *archivo,char NAME_OUTFILE[]){
	int cantidad_caracteres = 0;
	int cantidad_salto = 0;
	char aux;
	while(aux != EOF){
		aux = fgetc(archivo);
		if(aux != EOF){
			cantidad_caracteres++;
		}
		if(aux == '\n'){
			cantidad_salto++;
		}
	}
	rewind(archivo); //Coloca el apuntador al inicio del archivo
	char arreglo[cantidad_caracteres];
	memset(arreglo, '\0' , cantidad_caracteres);
	copiar_caracteres(archivo, arreglo);
	lista_frecuencia(arreglo, cantidad_caracteres,NAME_OUTFILE);
} 
void lista_frecuencia(char arreglo[], int cantidad_caracteres,char NAME_OUTFILE[]){
	int i = 0;
	lista *char_frec;
	char_frec = crear_lista();
	while(i < cantidad_caracteres){
		char c = arreglo[i];
		int z = buscar(char_frec,c);
		if(z == 0){ //Revisar se queda en el bucle
			if(c != 13){
				int frecuencia = frecuencia_caracter(arreglo,c,cantidad_caracteres);
				char_frec = agregar_inicio(char_frec, frecuencia, c);
			}
		}
		i++;
	}
	char_frec = ordenar_lista(char_frec);
	/*
	recorrer_lista(char_frec);
	ascii_code(char_frec);
	printf("\n");
	*/
	llenar_arbol(char_frec,arreglo,cantidad_caracteres,NAME_OUTFILE);
}
void llenar_arbol(lista *list, char arreglo[], int cantidad_caracteres,char NAME_OUTFILE[]){
	int len_lista = largo_lista(list);
	int i, primerCaso = 1;
	arbol *arbol_actual;
	arbol *arbol_anterior;
	arbol_actual = crear_arbol();
	arbol_anterior = crear_arbol();
	int pos = 0;
	i = 0;
	//THIS CASE ONLY WORK, IF LIST CONTATION MORE OF TWO ELEMENTS
	//This Case only work, if list contation zero or more of two elements, with one element drop the programa
	if(len_lista != 1){
		while( i < len_lista-1){
		if(primerCaso == 1){
			int suma_frecuencias = (numero(list,pos) + numero(list,pos+1));
			arbol_actual = agregar(&arbol_actual, suma_frecuencias, 0); // 0 = NULL CHARACTER
			arbol_actual = agregar(&arbol_actual, numero(list,pos), caracter(list,pos));
			arbol_actual = add_force(&arbol_actual, numero(list,pos+1), caracter(list,pos+1)); //Si es que no usamos este add_force y simplemente utilizamos el otro agregar, el segundo caracter se transforma en hijo del primero
			primerCaso = 0;
		}
		else{
			arbol_anterior = arbol_actual;
			arbol_actual = crear_arbol();
			int suma_frecuencias = raiz_value(arbol_anterior) + numero(list,pos+1);
			arbol_actual = agregar(&arbol_actual,suma_frecuencias,0);
			arbol_actual = agregar(&arbol_actual,numero(list,pos+1),caracter(list,pos+1));
			arbol_actual = agregar_derecha(arbol_actual,arbol_anterior);
		}
		pos = pos + 1;
		i++;
	    }	
	}
	else{
		int suma_frecuencias = (numero(list,0) + 0);
		arbol_actual = agregar(&arbol_actual, suma_frecuencias, 0);
		arbol_actual = agregar(&arbol_actual, numero(list,0), caracter(list,0));
		//printf("Error: Su archivo solo contiene un elemento.\n");
		//exit(0);
	}
	/*
	printf("------------------------------\n");
	preorder(arbol_actual);
	printf("------------------------------\n");
	printf("Hojas\n");
	muestra_hojas(arbol_actual);
	printf("------------------------------\n");
	printf("------------------------------\n");
	*/
	asignar_codigo(list, arbol_actual,arreglo,cantidad_caracteres,NAME_OUTFILE);
}
void asignar_codigo(lista *char_frec, arbol *arbol_actual, char arreglo[], int cantidad_caracteres,char NAME_OUTFILE[]){
	int i = 0;
	if(largo_lista(char_frec) != 1){
		while(i < largo_lista(char_frec)){
			char x = caracter(char_frec, i);
			char bin[nivel_arbol(arbol_actual)];
			memset(bin, '\0', nivel_arbol(arbol_actual));
			char fixed[nivel_arbol(arbol_actual)];
			memset(fixed, '\0', nivel_arbol(arbol_actual));
			buscar_c(arbol_actual,x,'3',bin,0,fixed);
			//printf("%s ------> %c\n", fixed, x);
			i++;
		}
	}
	//printf("------------------------------\n");
	//Guardar arbol en archivo
	//Lo que tenemos que hacer no es guardar el arbol en si, sino los datos necesarios para reconstuirlo posteriormente en el proceso de decodificado
	FILE *out;
	out = fopen(NAME_OUTFILE ,"w");
	for (i = 0; i < largo_lista(char_frec); ++i)
	{
		if(caracter(char_frec,i) == ' '){
			fputs("space",out);
			fputc(' ',out);	
			fprintf(out, "%d", numero(char_frec,i));
			fputc(' ',out);	
		}
		else if(caracter(char_frec,i) == '\n'){
			fputs("jump",out);	
			fputc(' ',out);	
			fprintf(out, "%d", numero(char_frec,i));
			fputc(' ',out);	
		}
		else if(caracter(char_frec,i) == 13){
			fputs("ret",out);	
			fputc(' ',out);	
			fprintf(out, "%d", numero(char_frec,i));
			fputc(' ',out);	
		}
		else if(caracter(char_frec,i) == 9){
			fputs("tabh",out);	
			fputc(' ',out);	
			fprintf(out, "%d", numero(char_frec,i));
			fputc(' ',out);	
		}
		else if(caracter(char_frec,i) == 11){
			fputs("tabv",out);	
			fputc(' ',out);	
			fprintf(out, "%d", numero(char_frec,i));
			fputc(' ',out);	
		}
		else if(caracter(char_frec,i) == 8){
			fputs("back",out);	
			fputc(' ',out);	
			fprintf(out, "%d", numero(char_frec,i));
			fputc(' ',out);	
		}
		else{
			char string[2];
			string[0] = caracter(char_frec,i);
			fputs(string,out);	
			fputc(' ',out);	
			fprintf(out, "%d", numero(char_frec,i));
			fputc(' ',out);
		}
	}
	//Fin de guardar arbol
	fputc('\n',out);
	if(largo_lista(char_frec) != 1){
		for (i = 0; i < cantidad_caracteres; ++i)
		{
			char c = arreglo[i];
			//char x = caracter(char_frec, i);
			char bin[nivel_arbol(arbol_actual)];
			memset(bin, '\0', nivel_arbol(arbol_actual));
			char fixed[nivel_arbol(arbol_actual)];
			memset(fixed, '\0', nivel_arbol(arbol_actual));
			buscar_c(arbol_actual,c,'3',bin,0,fixed);
			fputs(fixed, out);	
		}		
	}
	if(largo_lista(char_frec) == 1){
		fputc('1',out);
	}
	fclose(out);	
	printf("Fin proceso de codificado.\n");
}
//Debo aceptar que este proceso no es el mas optimo, y ademas estamos haciendo uso de los apuntadores los cuales no deberia usarse desde aqui
void buscar_c(arbol *a, char caracter, char almacenar, char bin[], int i, char fixed[]){

	if(a != NULL){
		if(a->caracter == caracter && a->izq == NULL & a->der ==NULL){
			bin[i] = almacenar;
			i = i + 1;
			strcpy(fixed,bin); //Guardamos en fixed solo lo que queremos guardar
		}
		if(almacenar == '0' && a -> izq != NULL){
			bin[i] = '0';
			i = i + 1;
		}
		if(almacenar == '1' && a->der != NULL){
			bin[i] = '1';
			i = i + 1;
		}
		buscar_c(a->izq,caracter,'0',bin,i,fixed);
		buscar_c(a->der,caracter,'1',bin,i,fixed);
	}
}
int frecuencia_caracter(char arreglo[], char c, int cantidad_caracteres){
	int frecuencia = 0;
	int i = 0;
	while(i < cantidad_caracteres){
		if(arreglo[i] == c){
			frecuencia++;
		}
		i++;
	}
	return frecuencia;
}
void copiar_caracteres(FILE *archivo, char arreglo[]){
	char c;
	int i = 0;
	while(c != EOF){
		c = fgetc(archivo);
		arreglo[i] = c;
		i++;
	}
	fclose(archivo);
}
void verif_argumento(int cantidad_argumentos, char NAME_FILE[]){
	if(cantidad_argumentos > 1 && cantidad_argumentos == 2){
		buscar_punto(NAME_FILE);
	}
	else{
		printf("Error: Falta Nombre del archivo.\nSintaxis: UCMZIP <Nombre del archivo>\nVuelva a ejecutar el programa.\n");
	}
}
void buscar_punto(char NAME_FILE[]){
	int i;
	int puntos = 0;
	for (i = 0; i < strlen(NAME_FILE); ++i)
	{
		if(NAME_FILE[i] == '.'){
			puntos++;
		}
	}
	verif_extencion(NAME_FILE,puntos);
}
void verif_extencion(char NAME_FILE[],int puntos_original){
	int i;
	int puntos = 0;
	for (i = 0; i < strlen(NAME_FILE); ++i)
	{
		if(NAME_FILE[i] == '.'){
			puntos++;
		}
		if(NAME_FILE[i] == '.' && puntos == puntos_original){
			if(NAME_FILE[i+1] == 'u'){
				if(NAME_FILE[i+2] == 'z'){
					if(NAME_FILE[i+3] == 'p' && NAME_FILE[i+4] == '\0'){
						descompresion(NAME_FILE,puntos_original);
					}
					else{
						compresion(NAME_FILE,puntos_original);
					}
				}
				else{
					compresion(NAME_FILE,puntos_original);
				}
			}
			else{
				compresion(NAME_FILE,puntos_original);
			}
		}
	}
}
void abrir_archivo_des(char NAME_FILE[],char NAME_OUTFILE[]){
	FILE *dArchivo;
	dArchivo = fopen(NAME_FILE,"r");
	int i;
	i = 0;
	lista *aux;
	aux = crear_lista();
	char c;
	while(1){
		c = fgetc(dArchivo);
		if( feof(dArchivo)){
			break;
		}
		i++;
	}
	rewind(dArchivo);
	char cadena[i];
	lista *dLista;
	dLista = crear_lista();
	while(1){
		fscanf(dArchivo, "%s", cadena);
		if( feof(dArchivo)){
			break;
		}
		if((strcmp(cadena,"space")) == 0){
			char a = ' ';
			fscanf(dArchivo, "%s", cadena);
			if( feof(dArchivo)){
				break;
			}
			int b = atoi(cadena);
			dLista = agregar_inicio(dLista, b, a);
		}
		else if((strcmp(cadena,"jump")) == 0){
			char a = '\n';
			fscanf(dArchivo, "%s", cadena);
			if( feof(dArchivo)){
				break;
			}
			int b = atoi(cadena);
			dLista = agregar_inicio(dLista, b, a);
		}
		else{
			char a = cadena[0];
			fscanf(dArchivo, "%s", cadena);
			if( feof(dArchivo)){
				break;
			}
			int b = atoi(cadena);
			dLista = agregar_inicio(dLista, b, a);
		}
	}
	fclose(dArchivo);
	re_arbol(dLista, i, NAME_FILE,NAME_OUTFILE);
}
void re_arbol(lista *dLista, int max_char, char NAME_FILE[],char NAME_OUTFILE[]){
	int pos = (largo_lista(dLista)) - 1;
	int primerCaso = 1;
	int i = 0;
	int len_lista = largo_lista(dLista);
	arbol *dArbol;
	arbol *dArbolAnterior;
	dArbol = crear_arbol();
	dArbolAnterior = crear_arbol();
	if(len_lista != 1){
		while(i < len_lista-1){
		if(primerCaso == 1){
			int suma_frecuencias = (numero(dLista,pos) + numero(dLista,pos-1));
			dArbol = agregar(&dArbol, suma_frecuencias, 0);
			dArbol = agregar(&dArbol, numero(dLista,pos), caracter(dLista,pos));
			dArbol = add_force(&dArbol, numero(dLista,pos-1), caracter(dLista,pos-1));
			primerCaso = 0;
		}
		else{
			dArbolAnterior = dArbol;
			dArbol = crear_arbol();
			int suma_frecuencias = raiz_value(dArbolAnterior) + numero(dLista,pos-1);
			dArbol = agregar(&dArbol,suma_frecuencias,0);
			dArbol = agregar(&dArbol,numero(dLista,pos-1),caracter(dLista,pos-1));
			dArbol = agregar_derecha(dArbol,dArbolAnterior);
		}
		i++;
		pos = pos -1;
		}
	}
	else{
		int suma_frecuencias = (numero(dLista,0) + 0);
		dArbol = agregar(&dArbol, suma_frecuencias, 0);
		dArbol = agregar(&dArbol, numero(dLista,0), caracter(dLista,0));
	}
	/*
	recorrer_lista(dLista);
	printf("--------------------------------------------------------\n");
	preorder(dArbol);
	*/
	re_palabra(dArbol,max_char,NAME_FILE,NAME_OUTFILE,dLista);
}
void re_palabra(arbol *dArbol, int max_char, char NAME_FILE[],char NAME_OUTFILE[], lista *dLista){
	FILE *dArchivo;
	dArchivo = fopen(NAME_FILE,"r");
	char dArreglo[max_char];
	memset(dArreglo,'\0',max_char);
	char c;
	int jumpCase = 0;
	int pos = 0;
	while(1){
		c = fgetc(dArchivo);
		if( feof(dArchivo)){
			break;
		}
		else if(c == '\n'){
			jumpCase = 1;
		}
		else{
			if(jumpCase == 1){
				dArreglo[pos] = c;
				pos++;
			}
		}
	}
	fclose(dArchivo);
	//printf("%s\n", dArreglo);
	re_file(dArbol,NAME_FILE,dArreglo,NAME_OUTFILE,dLista);
}
void re_file(arbol *dArbol, char NAME_FILE[], char dArreglo[],char NAME_OUTFILE[], lista *dLista){
	arbol *dAux;
	dAux = crear_arbol();
	dAux = dArbol;
	FILE *outFile;
	outFile = fopen(NAME_OUTFILE, "w");
	int i;
	i = 0;
	if(largo_lista(dLista) != 1){
		while(i < strlen(dArreglo)){
			char c = dArreglo[i];
			if(c == '0'){
				dArbol = move_this(dArbol,0);
			}
			else if(c == '1'){
				dArbol = move_this(dArbol,1);
			}	
			if(esVacio(dArbol) == 1){
				if(valorApuntado(dArbol) == '\n'){
					fputc('\r',outFile);
					fputc('\n',outFile);
					//printf("Puesto Salto\n");
				}
				else{
					fputc(valorApuntado(dArbol),outFile);
				}
				dArbol = dAux;
			}
			i++;
		}		
	}
	else{
		dArbol = move_this(dArbol,1);
		if(esVacio(dArbol) == 1){
			if(valorApuntado(dArbol) == '\n'){
				fputc('\r',outFile);
				fputc('\n',outFile);
				//printf("Puesto Salto\n");
			}
			else{
				fputc(valorApuntado(dArbol),outFile);
			}
		}
	}
	fclose(outFile);
	printf("Fin proceso de decodificado.\n");
}
void compresion(char NAME_FILE[],int puntos_original){
	printf("Iniciando proceso de codificado.\n");
	int i;
	int puntos = 0;
	char NAME_OUTFILE[strlen(NAME_FILE)+3];
	memset(NAME_OUTFILE,'\0',strlen(NAME_FILE)+3);
	for (i = 0; i < strlen(NAME_FILE) && puntos != puntos_original; ++i)
	{
		if(NAME_FILE[i] == '.'){
			puntos = puntos + 1;
		}
		NAME_OUTFILE[i] = NAME_FILE[i];
	}
	NAME_OUTFILE[i] = 'u';
	NAME_OUTFILE[i+1] = 'z';
	NAME_OUTFILE[i+2] = 'p';
	printf("%s\n", NAME_OUTFILE);
	abrir_archivo(NAME_FILE,NAME_OUTFILE);
}
void descompresion(char NAME_FILE[],int puntos_original){
	printf("Iniciando proceso de decodificado.\n");
	int i;
	int puntos = 0;
	char NAME_OUTFILE[strlen(NAME_FILE)];
	memset(NAME_OUTFILE,'\0',strlen(NAME_FILE)+3);
	for (i = 0; i < strlen(NAME_FILE) && puntos != puntos_original; ++i)
	{
		if(NAME_FILE[i] == '.'){
			puntos = puntos + 1;
		}
		NAME_OUTFILE[i] = NAME_FILE[i];
	}
	NAME_OUTFILE[i] = 't';
	NAME_OUTFILE[i+1] = 'x';
	NAME_OUTFILE[i+2] = 't';
	printf("%s\n", NAME_OUTFILE);
	abrir_archivo_des(NAME_FILE,NAME_OUTFILE);	
}