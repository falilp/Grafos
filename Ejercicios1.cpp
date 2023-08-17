#include <iostream>
#include <vector>
#include <algorithm>
#include "grafoPMC.h"

#pragma region Ejercicio1
/*Crear una función genérica, llamada DijkstraInv, para resolver el problema inverso al de Dijkstra, con los mismos 
tipos de parámetros y de resultado que la función ya incluida para éste. La nueva función, por tanto, debe hallar 
el camino de coste mínimo hasta un destino desde cada vértice del grafo y su correspondiente coste.*/

/*Debido a la implementación del grafo ponderado, este tiene un valor infinito por lo tanto si uno de los dos elementos es
infinito se devolverá ese valor en lugar de sumarlos, en caso contrario, se sumarán estos dos elementos.*/ 
template <typename T>
T sumaElementos(const T elemento1,const T elemento2){
    const T Infinito = Grafo<T>::INFINITO;
    if(elemento1 == Infinito || elemento2 == Infinito){
        return Infinito;
    }else{
        return elemento1 + elemento2;
    }
}

/*Aplicar Floyd a una matriz auxiliar con los valores de G*/
template <typename T>
std::vector<std::vector<T>> Floyd(const GrafoP<T>& G){
    std::vector<std::vector<T>> Aux;
    const size_t tam = G.numVert();

    for(size_t indice=0; indice<tam; indice++){
        Aux[indice] = G[indice];                    //Copia del vector de G
        Aux[indice][indice] = 0;                    //Colocar diagonal en 0
    }

    for(size_t k=0; k<tam; k++){
        for(size_t i=0; i<tam; i++){
            for(size_t j=0; j<tam; j++){
                Aux[i][j] = std::min(Aux[i][j],sumaElementos(Aux[k][j],Aux[i][k]));
            }
        }
    }

    return Aux;
}

/*Dijkstra inverso al cual se le asigna a un vector los caminos minimos desde cualquier nodo al nodo de destino*/
template <typename T>
std::vector<T> DijkstraInv(const GrafoP<T>& G, const typename GrafoP<T>::vertice VerticeDestino, typename std::vector<typename GrafoP<T>::vertice>& Vector){
    typename GrafoP<T>::vertice indice;
    const size_t tam = G.numVert();
    std::vector<T> Dinv(tam,0);
    std::vector<std::vector<T>> Aux = Floyd(G);

    for(indice=0; indice<tam; indice++){
        Dinv[indice] = Aux[indice][VerticeDestino];
    }

    return Dinv;
}

#pragma endregion

#pragma region Ejercicio2
/*Definiremos el pseudocentro de un grafo conexo como el nodo del mismo que minimiza la suma de las distancias mínimas a sus dos nodos más alejados. 
Definiremos el diámetro del grafo como la suma de las distancias mínimas a los dos nodos más alejados del pseudocentro del grafo. 
Dado un grafo conexo representado mediante matriz de costes, implementa un subprograma que devuelva la longitud de su diámetro.*/

template <typename T>
T DiamatroGrafo(const std::vector<T> vec){
    const size_t tam = vec.size();
    const T infinito = GrafoP<T>::INFINITO;

    for(size_t indice=0; indice<tam; indice++){
        if(infinito == vec[indice]){
            vec[infinito] = 0;
        }
    }

    std::sort(vec.begin(),vec.end());

    if((vec[vec.end()-1] != 0) && (vec[vec.end()-2] != 0)){
        return vec[vec.end()-1] + vec[vec.end()-2];
    }else{
        return infinito;
    }
}

template <typename T>
typename GrafoP<T>::vertice Pseudocentro(const GrafoP<T> &G,T& Diametro){
    typename GrafoP<T>::vertice Nodo,Centro;
    const size_t tam = G.numVert();
    size_t MinDiametro = GrafoP<T>::INFINITO;

    std::vector<std::vector<T>> Aux = Floyd(G);

    for(Nodo=0; Nodo<tam; Nodo++){
        T MinAux = DiamatroGrafo(Aux);
        if(MinDiametro > MinAux){
            MinDiametro = MinAux;
            Centro=Nodo;
        }
    }
    
    Diametro = MinDiametro;
    return Nodo;
}

#pragma endregion
/*
Code by Falilp
*/