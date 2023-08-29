#include <iostream>
#include <vector>
#include <algorithm>
#include "grafoPMC.h"
#include "AlgoritmoGrafosPMC.h"
#include "matriz.h"
#include "map"

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
/*Eres  el  orgulloso  dueño  de  la  empresa  “Cementos  de  Zuelandia  S.A”.  Empresa  
dedicada a la fabricación y distribución de cemento, sita en la capital de Zuelandia. Para 
la distribución del cemento entre tus diferentes clientes (ciudades de Zuelandia)  
dispones de una flota de camiones y de una plantilla de conductores zuelandeses. 
El  problema  a  resolver  tiene  que  ver  con  el  carácter  del  zuelandés.  El  zuelandés  es  
una  persona  que  se  toma  demasiadas  “KmExtra”  en  su  trabajo,  de  hecho,  tienes  
fundadas  sospechas  de  que  tus  conductores  utilizan  los  camiones  de  la  empresa  para  
usos  particulares  (es  decir  indebidos,  y  a  tu  costa)  por  lo  que  quieres  controlar  los  
kilómetros que recorren tus camiones. 
Todos  los  días  se  genera  el  parte  de  trabajo,  en  el  que  se  incluyen  el  número  de  
cargas  de  cemento  (1  carga  =  1  camión  lleno  de  cemento)  que  debes  enviar  a  cada  
cliente (cliente =  ciudad de Zuelandia). Es innecesario indicar que no todos los días hay 
que enviar cargas a todos los clientes, y además, puedes suponer razonablemente que tu 
flota de camiones es capaz de hacer el trabajo diario. 
Para  la  resolución  del  problema  quizá  sea  interesante  recordar  que  Zuelandia  es  un  
país  cuya  especial  orografía  sólo  permite  que  las  carreteras  tengan  un  sentido  de  
circulación. 
Implementa  una  función  que  dado  el  grafo  con  las  distancias  directas  entre  las  
diferentes  ciudades  zuelandesas,  el  parte  de  trabajo  diario,  y  la  capital  de  Zuelandia,  
devuelva la distancia total en kilómetros que deben recorrer tus camiones en el día, para 
que puedas descubrir si es cierto o no que usan tus camiones en actividades ajenas a la 
empresa.*/

template <typename Tcoste>
std::map<bool,double> CementosZuelandia(GrafoP<Tcoste> &G, typename GrafoP<Tcoste>::vertice Capital, std::vector<int> parteCamiones){
    typedef std::map<bool,double> KmExtra;
    KmExtra CamionesKmExtra;
    matriz<typename GrafoP<Tcoste>::vertice> vert;
    matriz<Tcoste> Caminos = Floyd(G,vert);

    for(size_t indice=0; indice<parteCamiones.size(); indice++){
        if(parteCamiones[indice] > 0 && indice != Capital && Caminos[Capital][indice] != G.INFINITO && Caminos[indice][Capital] != G.INFINITO){
            double distancia = (Caminos[Capital][indice] + Caminos[indice][Capital]);
            double distanciaRealizada = parteCamiones[indice];

            if(distancia < distanciaRealizada){
                CamionesKmExtra.insert(std::make_pair(true,distanciaRealizada-distancia));
            }
        }
    }

    return CamionesKmExtra;
}

#pragma endregion

#pragma region Ejercicio5
/*Se  dispone  de  tres  grafos  que  representan  la  matriz  de  costes  para  viajes  en  un  
determinado país pero por diferentes medios de transporte, por supuesto todos los grafos 
tendrán  el  mismo  número  de  nodos.  El  primer  grafo  representa  los  costes  de  ir  por  
carretera, el segundo en tren y el tercero en avión. Dado un viajero que dispone de una 
determinada cantidad de dinero, que es alérgico a uno de los tres medios de transporte, y 
que  sale  de  una  ciudad  determinada,  implementar  un  subprograma  que  determine  las  
ciudades a las que podría llegar nuestro infatigable viajero.*/

//supondre que se le pasa a la función dos grafos, puesto a que uno se descarta por que es alérgico a uno de los tres medios de transporte.

template <typename Tcoste>
std::vector<bool> Viajero(GrafoP<Tcoste> &G1, GrafoP<Tcoste> &G2, typename GrafoP<Tcoste>::vertice Origen, int dinero){
    std::vector<bool> visitados(G1.numVert(),false);
    std::vector<typename GrafoP<Tcoste>::vertice> vertG1,vertG2;

    std::vector<Tcoste> costesG1 = Dijkstra(G1,Origen,vertG1);
    std::vector<Tcoste> costesG2 = Dijkstra(G2,Origen,vertG2);

    for(size_t indice=0; indice<costesG1.size(); indice++){
        if(Origen != indice && costesG1[indice] => costesG2[indice]){
            dinero -= costesG1[indice];
        }else if(Origen != indice  && costesG1[indice] < costesG2[indice]){
            dinero -= costesG2[indice];
        }

        visitados[indice] = (dinero >= 0);
    }

    return visitados;
}

#pragma endregion

#pragma region Ejercicio6
/*Al  dueño  de  una  agencia  de  transportes  se  le  plantea  la  siguiente  situación.  La  
agencia  de  viajes  ofrece  distintas  trayectorias  combinadas  entre  N  ciudades  españolas  
utilizando tren y autobús. Se dispone de dos grafos que representan los costes (matriz de 
costes)  de  viajar  entre  diferentes  ciudades,  por  un  lado  en  tren,  y  por  otro  en  autobús  
(por supuesto entre las ciudades que tengan línea directa entre ellas). Además coincide 
que los taxis de toda España se encuentran en estos momentos en huelga general, lo que 
implica  que  sólo  se  podrá  cambiar  de  transporte  en  una  ciudad  determinada  en  la  que,  
por casualidad, las estaciones de tren y autobús están unidas. 
Implementa una función que calcule la tarifa mínima (matriz de costes mínimos) de 
viajar entre cualesquiera de las N ciudades disponiendo del grafo de costes en autobús, 
del grafo de costes en tren, y de la ciudad que tiene las estaciones unidas.*/

template <typename Tcoste>
matriz<Tcoste> TarifaMinima(GrafoP<Tcoste> &Autobus, GrafoP<Tcoste> &Tren){
    GrafoP<Tcoste> Aux(Autobus.numVert());
    matriz<Tcoste> MatrizAux;
    typedef typename GrafoP<Tcoste>::vertice vertice;

    for(vertice i=0; i<Autobus.numVert(); i++){
        for(vertice j=0; j<Autobus.numVert(); j++){
            if(Autobus[i][j] != Autobus.INFINITO){
                Aux[i][j] = Autobus[i][j];
            }
            if(Tren[i][j] != Autobus.INFINITO){
                Aux[i][j] = std::min(Aux[i][j],Tren[i][j]);
            }
        }
    }

    matriz<Tcoste> TarifaMin = Floyd(Aux,MatrizAux);

    return TarifaMin;
}

#pragma endregion

#pragma region Ejercicio7
/*Se dispone de dos grafos (matriz de costes) que representan los costes de viajar entre 
N  ciudades  españolas  utilizando  el  tren  (primer  grafo)  y  el  autobús  (segundo  grafo).  
Ambos grafos representan viajes entre las mismas N ciudades. 
Nuestro objetivo es hallar el camino de coste mínimo para viajar entre dos ciudades 
concretas del grafo, origen y destino, en las siguientes condiciones: 

-La ciudad origen sólo dispone de transporte por tren. 
-La ciudad destino sólo dispone de transporte por autobús. 
-El sector del taxi, bastante conflictivo en nuestros problemas, sigue en huelga, 

por  lo  que  únicamente  es  posible  cambiar  de  transporte  en  dos  ciudades  del  
grafo,  cambio1  y  cambio2,  donde  las  estaciones  de  tren  y  autobús  están  
unidas. 
Implementa  un  subprograma  que  calcule  la  ruta  y  el  coste  mínimo  para  viajar  entre  
las ciudades Origen y Destino en estas condiciones.*/

template <typename Tcoste>
struct Viaje{
    size_t coste;
    std::vector<typename GrafoP<Tcoste>::vertice> camino;
};


template <typename Tcoste>
GrafoP<Tcoste> CrearSuperGrafo(GrafoP<Tcoste> &G1, GrafoP<Tcoste> &G2){
    size_t SumaVertices = tren.numVert() + autobus.numVert();
    GrafoP<Tcoste> SuperGrafo(SumaVertices);

    for(size_t i=0; i<G1.numVert(); i++){
        for(size_t j=0; j<G1.numVert(); j++){
            SuperGrafo[i][j] = G1[i][j];
        }
    }
    for(size_t i=0; i<G2.numVert(); i++){
        for(size_t j=0; j<G2.numVert(); j++){
            SuperGrafo[i+G1.numVert()][j+G1.numVert()] = G2[i][j];
        }
    }

    return SuperGrafo;
}

template <typename Tcoste>
Viaje<Tcoste> ViajeRuta(GrafoP<Tcoste> &tren, GrafoP<Tcoste> &autobus, typename GrafoP<Tcoste>::vertice Cambio1, typename GrafoP<Tcoste>::vertice Cambio2,typename GrafoP<Tcoste>::vertice Origen, typename GrafoP<Tcoste>::vertice Destino){
    Viaje<Tcoste> viaje;
    size_t SumaVertices = tren.numVert() + autobus.numVert();
    GrafoP<Tcoste> SuperGrafo(SumaVertices);
    SuperGrafo = CrearSuperGrafo(tren,autobus);

    SuperGrafo[Cambio1][Cambio1+tren.numVert()]=0;
    SuperGrafo[Cambio1+tren.numVert()][Cambio1]=0;
    SuperGrafo[Cambio2][Cambio2+tren.numVert()]=0;
    SuperGrafo[Cambio2+tren.numVert()][Cambio2]=0;

    matriz<Tcoste> caminos;
    matriz<Tcoste> GrafoFinal = Floyd(SuperGrafo,caminos);   
    viaje.coste = GrafoFinal[Origen][Destino];

    typename GrafoP<Tcoste>::vertice intermedio = caminos[Origen][Destino];
    typename GrafoP<Tcoste>::vertice vertice = Origen;
    typename GrafoP<Tcoste>::vertice aux;

    viaje.camino.push_back(Origen);
    while(intermedio != Destino){
        viaje.camino.push_back(intermedio);

        aux = intermedio;
        intermedio = caminos[vertice][intermedio];
        vertice = aux;
    }    
    viaje.camino.push_back(Destino);

    return viaje;
}

#pragma endregion

#pragma region Ejercicio8
/*“UN  SOLO  TRANSBORDO,  POR  FAVOR”.  Este  es  el  título  que  reza  en  tu  
flamante  compañía  de  viajes.  Tu  publicidad  explica,  por  supuesto,  que  ofreces  viajes  
combinados  de  TREN  y/o  AUTOBÚS  (es  decir,  viajes  en  tren,  en  autobús,  o  usando  
ambos),  entre  N  ciudades  del  país,  que  ofreces  un  servicio  inmejorable,  precios  muy  
competitivos, y que garantizas ante notario algo que no ofrece ninguno de tus 
competidores:  que  en  todos  tus  viajes  COMO  MÁXIMO  se  hará  un  solo  transbordo  
(cambio de medio de transporte). 
Bien, hoy es 1 de Julio y comienza la temporada de viajes. 
¡Qué suerte! Acaba de aparecer un cliente en tu oficina. Te explica que quiere viajar 
entre dos ciudades, Origen y Destino, y quiere saber cuánto le costará. 
Para  responder  a  esa  pregunta  dispones  de  dos  grafos  de  costes  directos  (matriz  de  
costes) de viajar entre las N ciudades del país, un grafo con los costes de viajar en tren y 
otro en autobús. 
Implementa un subprograma que calcule la tarifa mínima en estas condiciones.*/

template <typename Tcoste>
std::vector<Tcoste> DijkstraInv(const GrafoP<Tcoste>& G, const typename GrafoP<Tcoste>::vertice VerticeDestino){
    typename GrafoP<Tcoste>::vertice indice;
    const size_t tam = G.numVert();
    std::vector<Tcoste> Dinv(tam,0);
    matriz<typename GrafoP<tCoste>::vertice> Vector;
    matriz<Tcoste> Aux = Floyd(G,vec);

    for(indice=0; indice<tam; indice++){
        Dinv[indice] = Aux[indice][VerticeDestino];
    }

    return Dinv;
}

template <typename Tcoste>
Tcoste UnicoTransbordo(GrafoP<Tcoste> &tren, GrafoP<Tcoste> &autobus, typename GrafoP<Tcoste>::vertice Origen,typename GrafoP<Tcoste>::vertice Destino){
    std::vector<typename GrafoP<Tcoste>::vertice> vecTren, vecBus, vecTrenInverso, vecBusInverso;
    Tcoste Minimo = tren.INFINITO;

    std::vector<Tcoste> caminoDirectotren = Dijkstra(tren,Origen,vecTren);
    std::vector<Tcoste> caminoDirectoAutobus = Dijkstra(autobus,Origen,vecBus);
    std::vector<Tcoste> caminoInversotren = DijkstraInv(tren,Destino);
    std::vector<Tcoste> caminoInversoAutobus = DijkstraInv(autobus,Destino);

    if(caminoDirectotren[Destino] <= caminoDirectoAutobus[Destino]){
        Minimo = caminoDirectotren[Destino];
    }else{
        Minimo = caminoDirectoAutobus[Destino];
    }

    for(size_t indice=0; indice<caminoDirectotren.size(); indice++){
        if((caminoDirectotren[indice] + caminoInversoAutobus[indice]) < Minimo){
            Minimo = (caminoDirectotren[indice] + caminoInversoAutobus[indice]);
        }else if((caminoDirectoAutobus[indice] + caminoInversotren[indice]) < Minimo){
            Minimo = (caminoDirectoAutobus[indice] + caminoInversotren[indice]);
        }
    }

    return Minimo;
}

#pragma endregion

#pragma region Ejercicio9
/*Se  dispone  de  dos  grafos  que  representan  la  matriz  de  costes  para  viajes  en  un  
determinado país, pero por diferentes medios de transporte (tren y autobús, por 
ejemplo).  Por  supuesto  ambos  grafos  tendrán  el  mismo  número  de  nodos,  N.  Dados  
ambos  grafos,  una  ciudad  de  origen,  una  ciudad  de  destino  y  el  coste  del  taxi  para  
cambiar de una estación a otra dentro de cualquier ciudad (se supone constante e igual 
para todas las ciudades), implementa un subprograma que  calcule el camino y el coste 
mínimo para ir de la ciudad origen a la ciudad destino.*/

template <typename Tcoste>
GrafoP<Tcoste> CrearSuperGrafo_v2(GrafoP<Tcoste> &G1, GrafoP<Tcoste> &G2, Tcoste taxi){
    size_t SumaVertices = tren.numVert() + autobus.numVert();
    GrafoP<Tcoste> SuperGrafo(SumaVertices);

    for(size_t i=0; i<G1.numVert(); i++){
        for(size_t j=0; j<G1.numVert(); j++){
            SuperGrafo[i][j] = G1[i][j];
            SuperGrafo[i+G1.numVert()][j] = taxi; 
            SuperGrafo[i][j+G1.numVert()] = taxi;
        }
    }
    for(size_t i=0; i<G2.numVert(); i++){
        for(size_t j=0; j<G2.numVert(); j++){
            SuperGrafo[i+G1.numVert()][j+G1.numVert()] = G2[i][j];
        }
    }

    return SuperGrafo;
}

template <typename Tcoste>
Tcoste ViajeTrenBus(GrafoP<Tcoste> &tren, GrafoP<Tcoste> &autobus, typename GrafoP<Tcoste>::vertice Origen,typename GrafoP<Tcoste>::vertice Destino, Tcoste taxi, std::vector<typename GrafoP<Tcoste>::vertice> &camino){
    size_t tam = 2*tren.numVert();
    Tcoste costeMin = tren.INFINITO;

    GrafoP<Tcoste> superGrafo = CrearSuperGrafo_v2(tren,autobus,taxi);
    std::vector<Tcoste> caminosMinimos = Dijkstra(superGrafo,Origen,camino);

    for(size_t indice=0; indice<3; indice++){
        if(caminosMinimos[Destino + (caminosMinimos.size()*indice)] < costeMin){
            costeMin = caminosMinimos[Destino + (caminosMinimos.size()*indice)];
        }
    }

    return costeMin;
}

#pragma endregion

#pragma region Ejercico10
/*Se  dispone  de  tres  grafos  que  representan  la  matriz  de  costes  para  viajes  en  un  
determinado país, pero por diferentes medios de transporte (tren, autobús y avión). Por 
supuesto los tres grafos tendrán el mismo número de nodos, N. 
Dados los siguientes datos: 

-los tres grafos, 
-una ciudad de origen, 
-una ciudad de destino, 
-el coste del taxi para cambiar, dentro de una ciudad, de la estación de tren a la       
de autobús o viceversa (taxi-tren-bus) y 
-el  coste  del  taxi  desde  el  aeropuerto  a  la  estación  de  tren  o  la  de  autobús,  o  
viceversa (taxi-aeropuerto-tren/bus) y asumiendo que ambos costes de taxi (distintos entre sí, son dos costes diferentes) son 
constantes e iguales para todas las ciudades, implementa un subprograma que  calcule el 
camino y el coste mínimo para ir de la ciudad origen a la ciudad destino.*/

template <typename Tcoste>
size_t ViajeAvionTrenBus(GrafoP<Tcoste> &avion, GrafoP<Tcoste> &tren, GrafoP<Tcoste> &autobus, typename GrafoP<Tcoste>::vertice Origen,typename GrafoP<Tcoste>::vertice Destino, Tcoste Taxi, std::vector<typename GrafoP<Tcoste>::vertice> &caminos){
    size_t costeMin = avion.INFINITO;
    GrafoP<Tcoste> Aux = CrearSuperGrafo(avion,tren);
    GrafoP<Tcoste> pais = CrearSuperGrafo(Aux,autobus);

    std::vector<Tcoste> camino = Dijkstra(pais,Origen,caminos);

    for(size_t indice=0; indice<3; indice++){
        if(camino[Destino + (camino.size()*indice)] < costeMin){
            costeMin = camino[Destino + (camino.size()*indice)];
        }
    }

    return costeMin;
}

#pragma endregion 

#pragma region EjercicioExtra
/*
Disponemos de dos grafos que nos indican los precios de viajar entre diferentes ciudades por tren y por avion
Del coste del taxi para cambiar dentro de una misma ciudad de tren a avion y viceversa
Asumiendo que el coste del taxi es constante e igual para todas las ciudades,
implementa una función que calcule el camino y el coste mínimo para ir de la ciudad
origen a la ciudad destino.
*/
#pragma endregion
/*
Code by Falilp
*/