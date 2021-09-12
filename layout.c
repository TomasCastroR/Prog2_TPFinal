#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Largo del buffer donde almacena la informacion innecesaria
#define LARGO_BUFFER 100
/*determinarInicializacion: FILE* -> Bool
Recibe un archivo con el formato de entrada, si la cantidad de obstaculos random es mayor
a la mitad de casillas disponibles devuelve 1, sino 0*/
int determinarInicializacion(FILE *archivo){
    char buffer[LARGO_BUFFER];
    int dimension,obsFijos=0,obsRandom;
    fscanf(archivo,"%s\n%d\n",buffer,&dimension);
    fgets(buffer,LARGO_BUFFER,archivo);
    while(fgetc(archivo)=='('){
        fgets(buffer,LARGO_BUFFER,archivo);
        obsFijos++;
    }
    fgets(buffer,LARGO_BUFFER,archivo);
    fscanf(archivo,"%d\n",&obsRandom);
    return (obsRandom>(((dimension*dimension)-obsFijos-2)/2));
}
/*inicializarLaberinto: char** int int
Recibe un laberinto, su dimension y la condicion de seteo.
Si la condicion es 1, inicializa todas las casillas en '1'.Sino, en '0'*/
void inicializarLaberinto (char **laberinto,int dimension,int condicion){
    char caracter = condicion +'0';
    for(int i=0;i<dimension;++i){
        laberinto[i]=malloc(sizeof(char)*(dimension+1));
        laberinto[0][i]= caracter;
    }
    laberinto[0][dimension]='\0';
    for(int i=1; i<dimension;++i){
        strcpy(laberinto[i],laberinto[0]);
    }
}
/*inicializarLaberinto: char** int
Recibe una matriz char y su tamaño, libera el espacio de memoria de cada
puntero en la matriz, luego libera la matriz*/
void liberarMemoria (char **array,int dimension){
    for(int i=0;i<dimension;++i){
        free(array[i]);
    }
    free(array);
}
/*verificar: int int int char** char -> bool
Recibe una numero de fila, un numero de columna, un laberinto, su tamaño y
un caracter que representa espacio no ocupado por otro objeto.
Devuelve 1 si la posicion esta dentro del laberinto y no es una posicion ya ocupada,
en caso contrario 0*/
int verificar(int posX, int posY,int dimension, char **laberinto,char caracter){
    return (posX>0 && posY>0)&&(posX<=dimension&&posY<=dimension)&&(laberinto[posX-1][posY-1]==caracter);
}
/*obstaculosRandom: char** int int int int char*
Recibe un laberinto, su dimension, la condicion de seteo, la cantidad de obstaculos random a poner,
la cantidad de objetos fijos puestos y un numero en un array char para la seedrand.
Si la condicion es 1, coloca '0' (caminos libres)
Si la condicion es 0, coloca '1 (paredes)'*/
void obstaculosRandom(char **laberinto,int dimension,int condicion,int cantObsRandom,int cantObsFijos,char *randomSeed){
    int filaRandom,columnaRandom;
    char posDisponible = condicion +'0';
    srand(atoi(randomSeed));
    if(condicion){
        int caminosPuestos=0,caminos_a_poner=((dimension*dimension)-cantObsFijos-2)-cantObsRandom;
        while(caminosPuestos<caminos_a_poner){
            filaRandom = rand()%dimension;
            columnaRandom = rand()%dimension;
            while(laberinto[filaRandom][columnaRandom]!= posDisponible){
                filaRandom = rand()%dimension;
                columnaRandom = rand()%dimension;
            }
            laberinto[filaRandom][columnaRandom]= '0';
            caminosPuestos++;
        }
    }
    else{
        int obsRandomPuestos=0;
        while(obsRandomPuestos<cantObsRandom){
            filaRandom = rand()%dimension;
            columnaRandom = rand()%dimension;
            while(laberinto[filaRandom][columnaRandom]!= posDisponible){
                filaRandom = rand()%dimension;
                columnaRandom = rand()%dimension;
            }
            laberinto[filaRandom][columnaRandom]= '1';
            obsRandomPuestos++;

        }
    }
}
/*layoutLaberinto: FILE* char** int int char* -> bool
Recibe un archivo, un laberinto ya inicializado, la condicion en la que se inicializo,
su dimension y un numero en un array char.
A medida que lee el archivo, coloca los obstaculos fijos, que segun la condicion colocara '1' o '2' respectivamente,
la salida y el objetivo. Siempre verificando que los datos en la entrada sean validos.
En caso de no serlos, devuelve 0. Sino, 1*/
int layoutLaberinto (FILE *archivo,char **laberinto,int dimension,int condicion,char *randomSeed){
    int validez=1,cantObsFijos=0,fila,columna,obsRandom;
    char caminoLibre = condicion +'0',paredFija = condicion + '1',buffer[LARGO_BUFFER];

    fgets(buffer,LARGO_BUFFER,archivo);
    while(fgetc(archivo) == '('&&validez==1){
        fscanf(archivo,"%d,%d)\n",&fila,&columna);
        if(verificar(fila,columna,dimension,laberinto,caminoLibre)){
            laberinto[fila-1][columna-1]= paredFija;
            cantObsFijos++;
        }
        else validez=0;
    }
    if (validez){
        fgets(buffer,LARGO_BUFFER,archivo);
        fscanf(archivo,"%d\n",&obsRandom);
        if(obsRandom>((dimension*dimension)-cantObsFijos-2))validez=0;
        else{
            fgets(buffer,LARGO_BUFFER,archivo);
            fscanf(archivo,"(%d,%d)\n",&fila,&columna);
            if(verificar(fila,columna,dimension,laberinto,caminoLibre)){
                laberinto[fila-1][columna-1]='I';
                }
            else validez=0;
            if(validez){
                fgets(buffer,LARGO_BUFFER,archivo);
                fscanf(archivo,"(%d,%d)\n",&fila,&columna);
                if(verificar(fila,columna,dimension,laberinto,caminoLibre)){
                laberinto[fila-1][columna-1]='X';}
                else validez=0;
                if(validez){
                    obstaculosRandom(laberinto,dimension,condicion,obsRandom,cantObsFijos,randomSeed);
                    if(condicion){
                        int paredesCambiadas=0;
                        for(int i=0;i<dimension&&paredesCambiadas<cantObsFijos;i++){
                            for(int j=0;j<dimension&&paredesCambiadas<cantObsFijos;j++){
                                if(laberinto[i][j]=='2'){
                                    laberinto[i][j]='1';
                                    paredesCambiadas++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fclose(archivo);
    return validez;
}
/*escritura: char** int char*
Recibe un laberinto, su dimension y el nombre del archivo de salida.
Escribe en el archivo el laberinto fila por fila*/
void escritura (char **laberinto,int dimension,char fileSalida[]){
    FILE *archivoSalida = fopen(fileSalida,"w");
    for(int i=0;i<dimension;++i){
        fprintf(archivoSalida,"%s\n",laberinto[i]);
    }
    fclose(archivoSalida);
}
int main (int Argc,char *argumentos[]){
    FILE *Entrada = fopen(argumentos[1],"r");
    int condicion;
    condicion = determinarInicializacion(Entrada);
    rewind(Entrada);
    int dimension;
    char buffer[LARGO_BUFFER];

    fgets(buffer,LARGO_BUFFER,Entrada);
    fscanf(Entrada,"%d\n",&dimension);
    char **laberinto=(char**)malloc(sizeof(char*)*dimension);
    inicializarLaberinto(laberinto,dimension,condicion);
    if(layoutLaberinto(Entrada,laberinto,dimension,condicion,argumentos[3])){
        escritura(laberinto,dimension,argumentos[2]);
        liberarMemoria(laberinto,dimension);
    }
    else{
        printf("La entrada no es valida\n");
        liberarMemoria(laberinto,dimension);
    }

    return 0;
}
