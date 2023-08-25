#include <iostream>
#include <vector>
#include <algorithm>
#include "grafoPMC.h"
#include "AlgoritmoGrafosPMC.h"

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
/*Se dispone de un laberinto de NxN  casillas del que se conocen las casillas de entrada 
y salida del mismo. Si te encuentras en una casilla sólo puedes moverte en las siguientes 
cuatro direcciones (arriba, abajo, derecha, izquierda). Por otra parte, entre algunas de las 
casillas hay una pared que impide moverse entre las dos casillas que separa dicha pared 
(en caso contrario  no sería un verdadero laberinto). 
Implementa un subprograma que dados 
    N (dimensión del laberinto),  
    la lista de paredes del laberinto, 
    la casilla de entrada y la casilla de salida, 
calcule el camino más corto para ir de la entrada a la salida y su longitud.*/

template <typename T>
typename GrafoP<T>::vertice SeleccionVertice(std::vector<T> &caminos, std::vector<bool> &visitados){
    typedef typename GrafoP<T>::vertice vertice;
    vertice aux;
    T MinCoste = G.INFINITO;

    for(vertice indice=0; indice<caminos.size(); indice++){
        if(!visitados[indice] && caminos[indice] <= MinCoste){
            MinCoste = caminos[indice];
            aux = indice;
        }
    }

    return aux;
}

template <typename T>
std::vector<T> Dijkstra(GrafoP<T> &G, typename GrafoP<T>::vertice Origen){
    typedef typename GrafoP<T>::vertice vertice;
    vertice aux;
    std::vector<T> caminos;
    std::vector<bool> visitados(G.numVert(),false);
    caminos = G[Origen];
    caminos[Origen] = 0;
    visitados[Origen] = true;

    for(vertice i=0; i<G.numVert(); i++){
        aux = SeleccionVertice(caminos,visitados);
        visitados[aux] = true;

        for(vertice j=0; j<G.numVert(); j++){
            if(!visitados[j] && caminos[j] > caminos[aux] + G[aux][j]){
                caminos[j] = caminos[aux] + G[aux][j];
            }    
        }        
    }

    return caminos;
}

template <typename T>
T LongitudMinLaberinto(const GrafoP<T> &G, typename GrafoP<T>::vertice Origen, typename GrafoP<T>::vertice Destino, const GrafoP<bool> &Paredes){
    T longitud = G.INFINITO;
    typedef typename GrafoP<T>::vertice vertice;
    GrafoP<T> Aux = G;
    
    for(vertice i=0; i<Aux.numVert(); i++){
        for(vertice j=0; j<Aux.numVert(); j++){
            if(Paredes[i][j]){
                Aux[i][j] = Aux.INFINITO;
            }
        }
    }

    std::vector<T> caminos = Dijkstra(Aux,Origen);
    longitud = caminos[Destino];

    return longitud;
}

#pragma endregion

#pragma region Ejercicio3
/* Eres  el  orgulloso  dueño  de  una  empresa  de  distribución.  Tu  misión  radica  en  
distribuir todo tu stock entre las diferentes  ciudades en las que tu empresa dispone de 
almacén.  
Tienes un grafo representado mediante la matriz de costes, en el que aparece el coste 
(por  unidad  de  producto)  de  transportar  los  productos  entre  las  diferentes  ciudades  del  
grafo. 
Pero  además  resulta  que  los  Ayuntamientos  de  las  diferentes  ciudades  en  las  que  
tienes  almacén  están  muy  interesados  en  que  almacenes  tus  productos  en  ellas,  por  lo  
que  están  dispuestos  a  subvencionarte  con  un  porcentaje  de  los  gastos  mínimos  de  
transporte  hasta  la  ciudad.  Para  facilitar  el  problema,  consideraremos  despreciables  los  
costes de volver el camión a su base (centro de producción). 
 
He aquí tu problema. Dispones de 
-el  centro  de  producción,  nodo  origen  en  el  que  tienes  tu  producto  (no  tiene  
almacén), 
-una cantidad de unidades de producto (cantidad), 
-la matriz de costes del grafo de distribución con N ciudades, 
-la capacidad de almacenamiento de cada una de ellas, 
-el  porcentaje  de  subvención  (sobre  los  gastos  mínimos)  que  te  ofrece  cada  
Ayuntamiento. 

Las  diferentes  ciudades  (almacenes)  pueden  tener  distinta  capacidad,  y  además  la  
capacidad  total  puede  ser  superior  a  la  cantidad  disponible  de  producto,  por  lo  que  
debes decidir cuántas unidades de producto almacenas en cada una de las ciudades. 
Debes  tener  en  cuenta  además  las  subvenciones  que  recibirás  de  los  diferentes  
Ayuntamientos,  las  cuales  pueden  ser  distintas  en  cada  uno  y  estarán  entre  el  0%  y  el  
100% de los costes mínimos. 
La solución del problema debe incluir las cantidades a almacenar en cada ciudad bajo 
estas  condiciones  y  el  coste  mínimo  total  de  la  operación  de  distribución  para  tu  
empresa*/

template <typename Tcoste>
double DistribuirStock(GrafoP<Tcoste> &G,typename GrafoP<Tcoste>::vertice Sede, int stock, std::vector<int> capacidadCiudades, std::vector<double> porcentajeSubvencion, std::vector<int> &cantidadCiudad){
    double costeTotal = 0;
    typename GrafoP<Tcoste>::vertice aux;
    Tcoste Mincoste;
    std::vector<bool> Almacenado(capacidadCiudades.size(),false);
    std::vector<typename GrafoP<Tcoste>::vertice> vert;
    std::vector<Tcoste> costes = Dijkstra(G,Sede,vert);
    std::vector<double> costePorcentaje(costes.size());

    for(size_t indice=0; indice<porcentajeSubvencion.size(); indice++){
        costePorcentaje[indice] = ((100-porcentajeSubvencion[indice])/100) * costes[indice];
    }

    while(stock > 0){
        Mincoste = G.INFINITO;
        for(typename GrafoP<Tcoste>::vertice indice=0; indice<G.numVert(); indice++){
            if((indice != Sede) && !Almacenado(indice) && (Mincoste > costePorcentaje[indice])){
                Mincoste = costePorcentaje[indice];
                aux = indice;
            }
        }

        if(capacidadCiudades[aux] <= stock){
            stock -= capacidadCiudades[aux];
            cantidadCiudad[aux] = capacidadCiudades[aux];
        }else{
            cantidadCiudad[aux] = capacidadCiudades[aux];
            stock = 0;
        }
        Almacenado[aux] = true;
    }
    
    for(size_t indice=0; indice<Almacenado.size(); indice++){
        if(Almacenado[indice]){
            costeTotal += cantidadCiudad[indice]; 
        }
    }

    return costeTotal;
}

#pragma endregion

#pragma region Ejercicio4

#pragma endregion

#pragma region Ejercicio5

#pragma endregion
/*
Code by Falilp
*/