import subprocess
from math import sqrt
import random
""" Diseño de datos:
    Representaremos un laberinto como una lista de listas (una matriz) donde cada lista representa
    una fila y la posicion en esa lista, la columna. Lo que se encuentra en cada casilla del laberinto se representa
    a traves de un caracter string.

    Para representar la ubicacion, posicion, coordenadas de una casilla o nodo del laberinto usaremos una dupla (int,int)
    donde el primer elemento representa la fila-1 y el segundo la columna-1. Tambien cuando nos referimos a nodo, objetivo
    o inicio, en realidad nos referimos a sus coordenadas.

    El programa comienza generando un numero random que sera la seedrand para el programa en C, luego ejecuta el archivo
    compilado de C pasando como argumentos el archivo de entrada, el nombre del texto de salida y la seedrand.
    
    En caso que el laberinto no tenga solucion, se generan laberintos hasta que alguno tenga solucion."""

# encontrar: List[List[string]] int -> Tupla(Tupla(int))
# Recibe un laberinto y su tamaño, devuelve en forma de tupla, las tuplas con las coordenadas
# de la salida y el objetivo respectivamente
def encontrar(laberinto,dimension):
    fila = 0
    columna = 0
    encontrado = 0
    while(fila<dimension and encontrado<2):
        while(columna<dimension and encontrado<2):
            if(laberinto[fila][columna]=="I"):
                inicio = (fila,columna)
                encontrado +=1
            if(laberinto[fila][columna]=="X"):
                final = (fila,columna)
                encontrado +=1
            columna+=1
        columna = 0
        fila+=1
    return (inicio,final)

# distancia: Tupla(int) Tupla(int) -> int
# Dado un nodo del laberinto y el objetivo, devuelve la distancia entre ellos
def distancia(nodo,objetivo):
    return sqrt((objetivo[0]-nodo[0])**2 + (objetivo[1]-nodo[1])**2)

# ordenarPorDistancia: List[Tupla(int)] Tupla(int)
# Dada una lista de nodos y el objetivo, ordena la lista en base a la distancia
# al objetivo de cada nodo de mayor a menor
def ordenarPorDistancia(listaNodos,objetivo):
    listaDistancia = []
    for nodo in listaNodos:
        Dist = distancia(nodo,objetivo)
        listaDistancia.append((nodo[0],nodo[1],Dist))
        
    listaNodos.clear()
    listaDistancia.sort(key=lambda tupla: tupla[2],reverse=True)
    for nodo in listaDistancia:
        listaNodos.append((nodo[0],nodo[1]))

# explorar: List[List[string]] Tupla(int) Tupla(int) int -> List[Tupla(int)]
# Dado un laberinto, un nodo, el objetivo y el tamaño del laberinto,
# devuelve una lista con los nodos adyacentes al nodo de entrada 
# que no sean una pared y no hayan sido visitados ordenados por la distancia al objetivo
def explorar(laberinto,nodo,objetivo,dimension):
    DicV = [1,0,-1,0]
    DicH = [0,1,0,-1]
    Adyacentes = []
    for index in range(0,4):
        X = nodo[0] + DicV[index]
        Y = nodo[1] + DicH[index]
        if X>=0 and Y>=0 and X<dimension and Y<dimension and laberinto[X][Y]!="1" and laberinto[X][Y]!="-1":
            Adyacentes.append((X,Y))
    ordenarPorDistancia(Adyacentes,objetivo)
    return Adyacentes

# resolverLaberinto: List[List[string]] Tupla(int) Tupla(int) int -> List[Tupla(int)]
# Recibe un laberinto, el inicio, el objetivo, y la dimension del laberinto,
# devuelve una lista de nodos en secuencia que representan una solucion del laberinto
def resolverLaberinto(laberinto,inicio,objetivo,dimension):
    stack = []
    stack.append([inicio])
    Solucion = []
    llegarObjetivo = False
    while(not llegarObjetivo and stack):
        path = stack.pop(-1)
        nodo = path[-1]
        if laberinto[nodo[0]][nodo[1]]!="-1":
            laberinto[nodo[0]][nodo[1]]="-1"
            vecinos = explorar(laberinto,nodo,objetivo,dimension)
            for nodoVecino in vecinos:
                newPath = path + [nodoVecino]
                stack.append(newPath)
                if laberinto[nodoVecino[0]][nodoVecino[1]]=="X":
                    llegarObjetivo = True
                    Solucion = newPath
    return Solucion

# imprimirSolucion: List[Tupla(int)]
# Recibe una solucion del laberinto, escribe la secuencia de nodos en un archivo
def imprimirSolucion(Solucion,archivoSolucion):
    salida = open(archivoSolucion,"w")
    for pasos in Solucion:
        salida.write("({0},{1})\n".format(pasos[0]+1,pasos[1]+1))
    salida.close()

def main():
    archivoEntradaC = "entrada.txt"
    archivoLaberinto = "salida.txt"
    archivoSalida = "solucion.txt"
    ejecutableDeC = "./a.exe"
    randomSeed = str(random.randrange(1000000000))
    ejecutar = subprocess.run([ejecutableDeC,archivoEntradaC,archivoLaberinto,randomSeed])
    #Pregunta si se genero la salida, en caso que sea False significa que la entrada no era valida
    if(ejecutar.returncode == 0):
        Entrada = open(ejecutar.args[2],"r")
        laberinto =list(map(lambda linea:list(linea.strip()),Entrada.readlines()))
        Entrada.close()
        dimension = len(laberinto)
        inicio_fin = encontrar(laberinto,dimension)
        recorrido = resolverLaberinto(laberinto,inicio_fin[0],inicio_fin[1],dimension)
        while(recorrido == []):
            randomSeed = str(random.randrange(1000000000))
            ejecutar = subprocess.run([ejecutableDeC,archivoEntradaC,archivoLaberinto,randomSeed])
            Entrada = open(ejecutar.args[2],"r")
            laberinto =list(map(lambda linea:list(linea.strip()),Entrada.readlines()))
            Entrada.close()
            recorrido = resolverLaberinto(laberinto,inicio_fin[0],inicio_fin[1],dimension)
        imprimirSolucion(recorrido,archivoSalida)
main()
