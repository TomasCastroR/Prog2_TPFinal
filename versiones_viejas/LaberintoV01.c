#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define LargoBuffer 100 

void InicializarLab (char *Laberinto[],int Dimension){
    for(int i=0;i<Dimension;++i){
        Laberinto[i]=malloc(sizeof(int)*Dimension+1);
        Laberinto[0][i]='0';
    }
    Laberinto[0][Dimension]='\0';
    for(int i=1; i<Dimension;++i){
        strcpy(Laberinto[i],Laberinto[0]);//ACA SE ROMPE PARA DIMENSION +20000
    }
}

void LiberarMemoria (char *Array[],int Dimension){
    for(int i=0;i<Dimension;++i){
        free(Array[i]);
    }
    free(Array);
}

int Verificar(int PosX, int PosY,int Dimension, char *Laberinto[]){
    return (PosX>0 && PosY>0)&&(PosX<=Dimension&&PosY<=Dimension)&&(Laberinto[PosX-1][PosY-1]=='0');
}

void Swapear(int Libres[],int Pos,int Dimension,int Tamano,int CantSwapeados){
    int Swap;

    Swap=Libres[(Tamano-1-CantSwapeados)];
    Libres[(Tamano-1-CantSwapeados)]=Libres[Pos];
    Libres[Pos]=Swap;
}

void ObstaculosRandom (FILE *Archivo,char *Laberinto[],int CantObsRandom,int Dimension){
    int Tamano=Dimension*Dimension,Pos=0;
    int *PosLibres=(int*)malloc(sizeof(int)*Tamano);
    for(int i=0;i<Dimension;i++){
        for(int j=0;j<Dimension;j++){
            if(Laberinto[i][j] == '0'){//ACA SE ROMPE PARA DIMENSION +20000
                PosLibres[Pos]=Pos;
            }
            else PosLibres[Pos]=-1;
            Pos++;
        }
    }
    srand(time(NULL));
    int ObsRandomPuestos=0,Random,Ocupadas=0,Posicion;

    while(ObsRandomPuestos<CantObsRandom){
        Random = rand()%(Tamano-(ObsRandomPuestos+Ocupadas));

        if(PosLibres[Random]!=-1){
            Posicion=PosLibres[Random];
            Laberinto[Posicion/Dimension][Posicion%Dimension]='1';
            Swapear(PosLibres,Random,Dimension,Tamano,(ObsRandomPuestos+Ocupadas));
            ObsRandomPuestos++;
        }
        else{
            Swapear(PosLibres,Random,Dimension,Tamano,(ObsRandomPuestos+Ocupadas));
            Ocupadas++;
        }
    }
    free(PosLibres);
}

int LayoutLab (FILE *Archivo,char *Laberinto[],int Dimension){
    int Validez=1,CantObsFijos=0,Fila,Columna,ObsRandom;
    char buffer[LargoBuffer],Caracter;
    fgets(buffer,100,Archivo);
    Caracter=fgetc(Archivo);
    while(Caracter == '('&&Validez==1){
        fscanf(Archivo,"%d,%d)\n",&Fila,&Columna);
        if(Verificar(Fila,Columna,Dimension,Laberinto)){
            Laberinto[Fila-1][Columna-1]='1';
            CantObsFijos++;
            Caracter=fgetc(Archivo);
        }
        else Validez=0;
    }
    if (Validez){
        fgets(buffer,100,Archivo);
        fscanf(Archivo,"%d\n",&ObsRandom);
        if(ObsRandom>((Dimension*Dimension)-CantObsFijos-2))Validez=0;
        if (Validez){
            fgets(buffer,100,Archivo);
            fscanf(Archivo,"(%d,%d)\n",&Fila,&Columna);
            if(Verificar(Fila,Columna,Dimension,Laberinto)){
            Laberinto[Fila-1][Columna-1]='I';}
            else Validez=0;
            if(Validez){
                fgets(buffer,100,Archivo);
                fscanf(Archivo,"(%d,%d)\n",&Fila,&Columna);
                if(Verificar(Fila,Columna,Dimension,Laberinto)){
                Laberinto[Fila-1][Columna-1]='X';}
                else Validez=0;
                if(Validez)ObstaculosRandom(Archivo,Laberinto,ObsRandom,Dimension);
            }
        }
    }
    fclose(Archivo);
    return Validez;
}
void Escritura (char *Laberinto[],int Dimension,char NombreSalida[]){
    FILE *ArchivoSalida = fopen(NombreSalida,"w");

    for(int i=0;i<Dimension;++i){
        fprintf(ArchivoSalida,"%s\n",Laberinto[i]);
    }
    fclose(ArchivoSalida);
}
int main (int Argc,char *Argumentos[]){
    FILE *Entrada = fopen(Argumentos[1],"r");
    int Dimension;
    char buffer[LargoBuffer];

    fgets(buffer,20,Entrada);
    fscanf(Entrada,"%d\n",&Dimension);
    char **Laberinto=(char**)malloc(sizeof(char*)*Dimension);
    InicializarLab(Laberinto,Dimension);
    if(LayoutLab(Entrada,Laberinto,Dimension)){
        Escritura(Laberinto,Dimension,Argumentos[2]);
        LiberarMemoria(Laberinto,Dimension);
    }
    else{
        printf("La entrada no es valida\n");
        LiberarMemoria(Laberinto,Dimension);
    }

    return 0;
}
