from ResolverLaberinto import *

# El archivo de ejemplo posee una lista de 2 elemementos por linea
# los cuales el primero es el laberinto y el segundo la dimension
# El archivo de resultados posee la tupla esperada por linea
def test_encontrar():
    ejemplosFile = open("encontrarEjemplos.txt","r")
    resultadosFile = open("encontrarResultados.txt","r")
    ejemplos = ejemplosFile.readlines()
    resultados = resultadosFile.readlines()
    ejemplosFile.close()
    resultadosFile.close()
    cantTests = len(ejemplos)
    index = 0
    while(index < cantTests):
        ejemplo = eval(ejemplos[index])
        assert encontrar(ejemplo[0],ejemplo[1])==eval(resultados[index])
        index += 1

# El archivo de ejemplo posee una lista de 2 elemementos por linea
# los cuales el primero son las coordenadas de un nodo cualquiera y
# el segundo las coordendas del objetivo
# El archivo de resultados posee la distancia esperada por linea
def test_distancia():
    ejemplosFile = open("distanciaEjemplos.txt","r")
    resultadosFile = open("distanciaResultados.txt","r")
    ejemplos = ejemplosFile.readlines()
    resultados = resultadosFile.readlines()
    ejemplosFile.close()
    resultadosFile.close()
    cantTests = len(ejemplos)
    index = 0
    while(index < cantTests):
        ejemplo = eval(ejemplos[index])
        assert distancia(ejemplo[0],ejemplo[1]) == eval(resultados[index])
        index += 1
# El archivo de ejemplo posee una lista de 4 elemementos por linea,
# el primero es un laberinto, el segundo es el nodo a explorar, el tercero es el objetivo
# y el cuarto la dimension del laberinto
# El archivo de resultados posee la lista de nodos vecinos esperada
def test_explorar():
    ejemplosFile = open("explorarEjemplos.txt","r")
    resultadosFile = open("explorarResultados.txt","r")
    ejemplos = ejemplosFile.readlines()
    resultados = resultadosFile.readlines()
    ejemplosFile.close()
    resultadosFile.close()
    cantTests = len(ejemplos)
    index = 0
    while(index < cantTests):
        ejemplo = eval(ejemplos[index])
        assert explorar(ejemplo[0],ejemplo[1],ejemplo[2],ejemplo[3]) == eval(resultados[index])
        index += 1
# El archivo de ejemplo posee una lista de 4 elemementos por linea,
# el primero es un laberinto, el segundo es el inicio, el tercero es objetivo
# y el cuarto la dimension del laberinto
# El archivo de resultados posee una solucion del laberinto
def test_resolverLaberinto():
    ejemplosFile = open("resolverLaberintoEjemplos.txt","r")
    resultadosFile = open("resolverLaberintoResultados.txt","r")
    ejemplos = ejemplosFile.readlines()
    resultados = resultadosFile.readlines()
    ejemplosFile.close()
    resultadosFile.close()
    cantTests = len(ejemplos)
    index = 0
    while(index < cantTests):
        ejemplo = eval(ejemplos[index])
        assert resolverLaberinto(ejemplo[0],ejemplo[1],ejemplo[2],ejemplo[3]) == eval(resultados[index])
        index += 1
    