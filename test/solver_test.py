import sys
sys.path.append("../")
from solver import distancia


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
    