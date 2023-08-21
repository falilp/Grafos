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
        T MinAux = DiamatroGrafo(Aux[Nodo]);
        if(MinDiametro > MinAux){
            MinDiametro = MinAux;
            Centro=Nodo;
        }
    }
    
    Diametro = MinDiametro;
    return Centro;
}

#pragma endregion

#pragma region Ejercicio3
/*Comprobar si un Grafo de costes es Acíclico*/

template <typename T>
bool Ciclico(const GrafoP<T> &G, typename GrafoP<T>::vertice nodo, std::vector<bool> visitados, std::vector<bool> apilados){
    const size_t tam = G.numVert();
    typename GrafoP<T>::vertice nodos;
    visitados[nodo] = true;
    apilados[nodo] = true;

    for(nodos=0; nodos<G[nodos]; nodos++){
        if(!visitados[nodos]){
            if(Ciclico(G,nodos,visitados,apilados)){
                return true;
            }
        }else if(apilados[nodos]){
            return true;
        }
    }    

    apilados[nodo] = false;
    return apilados[nodo];
}

template <typename T>
bool GrafoAciclico(const GrafoP<T> &G){
    const size_t tam = G.numVert();
    typename GrafoP<T>::vertice nodos;
    bool Aciclico = true;
    std::vector<bool> visitados(tam,false),apilados(tam,false);

    for(nodos=0; nodos<tam && Aciclico; nodos++){
        if(!visitados[nodos] && Ciclico(G,nodos,visitados,apilados)){
            Aciclico = false;
        }
    }

    return Aciclico;
}

#pragma endregion

#pragma region Ejercicio4

/*Se  necesita  hacer  un  estudio  de  las  distancias  mínimas  necesarias  para  viajar  entre 
dos  ciudades  cualesquiera  de  un  país  llamado  Zuelandia.  El  problema  es  sencillo  pero 
hay que tener en cuenta unos pequeños detalles: 
    a) La orografía de Zuelandia es un poco especial, las carreteras son muy estrechas 
    y por tanto solo permiten un sentido de la circulación. 

    b) Actualmente  Zuelandia  es  un  país  en  guerra.  Y  de  hecho  hay  una  serie  de 
    ciudades  del  país  que  han  sido  tomadas  por  los  rebeldes,  por  lo  que  no  pueden 
    ser usadas para viajar. 

    c) Los  rebeldes  no  sólo  se  han  apoderado  de  ciertas  ciudades  del  país,  sino  que 
    también han cortado ciertas carreteras, (por lo que estas carreteras no pueden ser 
    usadas). 

    d) Pero  el  gobierno  no  puede  permanecer  impasible  ante  la  situación  y  ha  exigido 
    que absolutamente todos los viajes que se hagan por el país pasen por la capital 
    del mismo, donde se harán los controles de seguridad pertinentes. 

Dadas estas cuatro condiciones, se pide implementar un subprograma que dados 
• el grafo (matriz de costes) de Zuelandia en situación normal, 
• la relación de las ciudades tomadas por los rebeldes, 
• la relación de las carreteras cortadas por los rebeldes 
• y la capital de Zuelandia, 
calcule la matriz de costes mínimos para viajar entre cualesquiera dos ciudades 
zuelandesas en esta situación.*/

template <typename T>
std::vector<std::vector<T>>& Zuelandia(GrafoP<T> &G, std::vector<typename GrafoP<T>::vertice> &CiudadesGuerra, std::vector<std::vector<bool>> &CarreterasCortadas, const typename GrafoP<T>::vertice Capital){
    std::vector<std::vector<T>> mapaFinal;
    const typename GrafoP<T>::vertice i,j;

    for(i=0; i<CiudadesGuerra.size(); i++){
        for(j=0; j<G.numVert(); j++){
            G[j][CiudadesGuerra[i]] = G.INFINITO;
            G[CiudadesGuerra[i]][j] = G.INFINITO;
        }
    }

    for(i=0; i<CarreterasCortadas.size(); i++){
        for(j=0; j<CarreterasCortadas.size(); j++){
            if(CarreterasCortadas[i][j]){
                G[i][j] = G.INFINITO;
            }
        }
    }

    mapaFinal = Floyd(G);

    for(i=0; i<G.numVert(); i++){
        for(j=0; j<G.numVert(); j++){
            if(mapaFinal[i][Capital] == G.INFINITO || mapaFinal[Capital][j] == G.INFINITO){
                mapaFinal[i][j] = G.INFINITO;
            }else if(i != j){
                mapaFinal[i][j] =  mapaFinal[i][Capital] +  mapaFinal[Capital][j];
            }
        }
    }

    return mapaFinal;
}

#pragma endregion
/*
Code by Falilp
*/