#include <iostream>
#include <vector>
#include <algorithm>
#include "grafoPMC.h"

#pragma region Ejercicio1
/*Tu agencia de viajes “OTRAVEZUNGRAFO S.A.”  se enfrenta a un curioso cliente. 
Es un personaje sorprendente, no le importa el dinero y quiere hacer el viaje más caro  
posible entre las ciudades que ofertas. Su objetivo es gastarse la mayor cantidad de  
dinero posible y no le importa el origen ni el destino del viaje. 

Sabiendo que es imposible pasar dos veces por la misma ciudad, ya que casualmente 
el grafo de tu agencia de viajes resultó ser acíclico, devolver el coste, origen y destino 
de tan curioso viaje. Se parte de la matriz de costes directos entre las ciudades del grafo.*/

template <typename T>
struct Viaje{
    T coste;
    typedef typename Grafop<T>::vertice vertice; 
    vertice Origen,Destino;
};

template <typename T>
void FloydInverso(GrafoP<T> &G){
    typedef typename GrafoP<T>::vertice vertice;

    for(vertice indice=0; indice<G.numVert(); indice++){
        G[indice][indice] = 0;
    }

    for(vertice k=0; k<G.numVert(); k++){
        for(vertice i=0; i<G.numVert(); i++){
            for(vertice j=0; j<G.numVert(); j++){
                G[i][j] = std::max(G[i][j],G[i][k]+G[k][j]);
            }
        }
    }
}

template <typename T>
Viaje<T> MaximoViaje(const GrafoP<T> &G){
    typedef typename GrafoP<T>::vertice vertice;
    Viaje<T> viaje;
    viaje.coste = 0;
    GrafoP<T> Aux = G;

    FloydInverso(Aux);

    for(vertice i=0; i<Aux.numVert(); i++){
        for(vertice j=0; j<Aux.numVert(); j++){
            if(viaje.coste < Aux[i][j]){
                viaje.coste = Aux[i][j];
                viaje.Origen = i;
                viaje.Destino = j;
            }
        }
    }

    return viaje;
}

#pragma endregion

#pragma region Ejercicio2

#pragma endregion

#pragma region Ejercicio3

#pragma endregion

#pragma region Ejercicio4

#pragma endregion

#pragma region Ejercicio5

#pragma endregion
/*
Code by Falilp
*/