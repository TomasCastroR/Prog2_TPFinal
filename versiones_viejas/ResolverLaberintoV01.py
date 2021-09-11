import subprocess
import random

def encontrar(laberinto,dimension):
    fila = 0
    columna = 0
    encontrado = False
    while(fila<dimension and not encontrado):
        while(columna<dimension and not encontrado):
            if(laberinto[fila][columna]=="I"):
                inicio = (fila,columna)
                encontrado=True
            columna+=1
        columna = 0
        fila+=1
    return inicio
def explorar(laberinto,Posicion,dimension):
    DicV = [1,0,-1,0]
    DicH = [0,1,0,-1]
    Adyacentes = []
    i=0
    while(i<4):
        X = Posicion[0] + DicV[i]
        Y = Posicion[1] + DicH[i]
        if X>=0 and Y>=0 and X<dimension and Y<dimension and laberinto[X][Y]!="1":
            Adyacentes.append((X,Y))
        i+=1
    return Adyacentes
            
def resolverLaberinto(laberinto,inicio,dimension):
    Queue = []
    Queue.append([inicio])
    Solucion = []
    llegarObjetivo = False
    while(not llegarObjetivo and Queue!=[]):
        path = Queue.pop(0)
        nodo = path[-1]
        if laberinto[nodo[0]][nodo[1]]!=-1:
            laberinto[nodo[0]][nodo[1]]=-1
            vecinos = explorar(laberinto,nodo,dimension)
            for nodoVecino in vecinos:
                if laberinto[nodoVecino[0]][nodoVecino[1]]!=-1:
                    newPath = path + [nodoVecino]
                    Queue.append(newPath)
                    if laberinto[nodoVecino[0]][nodoVecino[1]]=="X":
                        llegarObjetivo = True
                        Solucion = newPath
    return Solucion
def imprimirSolucion(Solucion,intentos):
    for pasos in Solucion:
        print((pasos[0]+1,pasos[1]+1))
    print(intentos)
def main():
    random.seed()
    randomSeed = random.randint(-100000,100000)
    ejecutar = subprocess.run(["./a.exe","entrada.txt","salida.txt",str(randomSeed)])
    Entrada = open(ejecutar.args[2],"r")
    laberinto =list(map(lambda linea:list(linea.strip()),Entrada.readlines()))
    Entrada.close()
    dimension = len(laberinto)
    salida = encontrar(laberinto,dimension)
    recorrido = resolverLaberinto(laberinto,salida,dimension)
    intentos = 0
    while(recorrido == []):
        randomSeed = random.randint(0,100000)
        ejecutar = subprocess.run(["./a.exe","entrada.txt","salida.txt",str(randomSeed)])
        Entrada = open(ejecutar.args[2],"r")
        laberinto =list(map(lambda linea:list(linea.strip()),Entrada.readlines()))
        Entrada.close()
        recorrido = resolverLaberinto(laberinto,salida,dimension)
        intentos +=1
    imprimirSolucion(recorrido,intentos)
main()