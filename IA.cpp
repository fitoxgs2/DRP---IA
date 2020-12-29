#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

//Utilidades
#define debug(n1) cout << n1 << endl
#define ll long long
#define ld long double
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define eb emplace_back

#define MAX_SIZE 708
//MAX_SIZE es la mayor cantidad de eventos posibles en las instancias

vector<pair<int,int>> OHCAs;
vector<bool> radios_cubiertos[MAX_SIZE]; //Matriz que indica si i cubre a j.(simetrica)
vector<bool> cubiertos;
vector<bool> ubicados;

//Calcula la distancia entre dos puntos
float distancia(pair<int,int> a, pair<int,int> b){
    float d = sqrt(pow((a.fi-b.fi),2) + pow((a.se-b.se),2));
    return d;
}

//Realiza la suma de todos los elementos de una lista
//Se utiliza para obtener el total de OHCA cubiertos por un AED.
//Y para obtener el total de OHCA cubiertos.
int sumatoria(vector<bool> lista){
    int suma = 0;
    for(vector<bool>::iterator it = lista.begin(); it != lista.end(); it++){
        suma += *it;
    }
    return suma;
}

//Actualiza la lista de OHCA cubiertos
void actualizar_cubiertos(unsigned n_size){
    int aed,ohca;
    for(ohca = 0; ohca < n_size; ohca++){
        cubiertos[ohca] = 0;
    }
    for(aed = 0; aed < n_size; aed++){
        if (ubicados[aed]){
            for(ohca = 0; ohca < n_size; ohca++){
                if (radios_cubiertos[aed][ohca]){
                    cubiertos[ohca] = 1;
                }
            }
        }
    }
}

//Evaluacion del movimiento SWAP entre dos AED
//Se cambia uno ubicado por otro que no
//Entrega la cantidad de nodos diferentes que serian cubiertos
//Positivo si es que se cubren mas que antes, 0 si no cambia y negativo si se cubren menos
int funcion_eval(int aed1, int aed2, unsigned n_size){
    vector<bool> save_state = cubiertos;
    int valor_original = sumatoria(cubiertos);
    //Swap
    ubicados[aed1] = 0;
    ubicados[aed2] = 1;
    actualizar_cubiertos(n_size);
    int valor_nuevo = sumatoria(cubiertos);
    //Deshacer swap
    ubicados[aed1] = 1;
    ubicados[aed2] = 0;
    cubiertos = save_state;
    return valor_nuevo-valor_original;
}

//MM es un array que contiene los indices de los 2 AED a cambiar y el valor de su funcion
void vecindario_swap(int aed, int MM[], unsigned n_size){
    int actual_value;
    for(int i=0; i<n_size; i++){
        if(!ubicados[i]){
            actual_value = funcion_eval(aed, i, n_size);
            if (actual_value > MM[2]){
                MM[0] = aed;
                MM[1] = i;
                MM[2] = actual_value;
            }
        }
    }
}

//Solucion inicial por greedy involucra seleccionar los OHCA que mas eventos cubran.
//(No revisara si es que cubre multiples veces algun evento)
float greedy(float budget, unsigned n_size){
    vector<int> greedy_choice_list;
    float gastos = 0;
    int best_value, best_index, i;
    for(i=0; i<n_size; i++){
        greedy_choice_list.pb(sumatoria(radios_cubiertos[i]));
    }
    //El primer AED ubicado es aleatorio, distinto de uno existente.
    srand(time(NULL));
    int primer_aed = rand() % n_size;
    while(ubicados[primer_aed]){
        primer_aed = rand() % n_size;
    }
    gastos = gastos + 1;
    ubicados[primer_aed] = 1;
    actualizar_cubiertos(n_size);
    
    while(gastos<budget){
        best_index = -1;
        best_value = 0;
        for (i=0; i<n_size; i++){
            if(!ubicados[i]){
                if(greedy_choice_list[i] > best_value){
                    best_index = i;
                    best_value = greedy_choice_list[i];
                }
            }
        }
        gastos = gastos + 1;
        ubicados[best_index] = 1;
        actualizar_cubiertos(n_size);
    }
    return gastos;
}

int main(int argc, char *argv[])
{
    clock_t inicio = clock();
    double tiempo_ejecucion = 0.0;
    float gastos, B; //Budget
    unsigned N, R; //Numero de eventos y radio
    int x, y, c; //Variables que tomaran los valores de cada linea en la instancia
    unsigned i,j; //iteradores
    int MM[3]; //Mejor mejora. Contiene AED1, AED2 y valor de la funcion evaluacion.
    
    string line;
    ifstream instancia (argv[1]);
    //Set de Parametros base
    getline(instancia,line);
    stringstream inputSS(line);
    inputSS >> N >> B >> R;

    for (i=0; i<N; i++) {
        getline(instancia,line);
        stringstream inputSS(line);
        inputSS >> x >> y >> c;
        OHCAs.pb(mp(x,y));
        ubicados.pb(c);
        cubiertos.pb(0);
        for (j=0; j<=i; j++){
            if (distancia(OHCAs[i], OHCAs[j]) <= R){
                radios_cubiertos[i].pb(1);
                radios_cubiertos[j].pb(1);
            } else {
                radios_cubiertos[i].pb(0);
                radios_cubiertos[j].pb(0);
            }
        }
    }
    //Algoritmo greedy para obtener solucion inicial
    gastos = greedy(B,N);

    //Hill climbing (se detiene al llegar a la cima)
    while(true){
        //Se comienza a iterar en el vecindario para obtener el mejor swap posible
        MM[0] = -1;
        MM[1] = -1;
        MM[2] = -1;

        for (i=0; i<N; i++){
            if(ubicados[i]){
                vecindario_swap(i, MM, N);
            }
        }
        if(MM[2] >= 0){
            ubicados[MM[0]] = 0;
            ubicados[MM[1]] = 1;
            actualizar_cubiertos(N);
        } else {
            break;
        }
    }
    clock_t fin = clock();
    tiempo_ejecucion += (double)(fin-inicio)/CLOCKS_PER_SEC;
    cout << "Numero de eventos cubiertos: " << sumatoria(cubiertos) <<endl;
    cout << "Porcentaje de eventos cubiertos: " << 100*sumatoria(cubiertos)/N << "%" << endl;
    cout << "Tiempo de ejecucion: " << tiempo_ejecucion << " segundos." << endl;
    cout << "Cantidad de iteraciones alcanzadas: " << 1 << endl;
    cout << "Solucion: " << endl;
    for (i=0; i<N; i++){
        if(ubicados[i])
            cout << OHCAs[i].fi <<"-"<< OHCAs[i].se << "(nuevo)" << endl;
    }
    cout << "Presupuesto sobrante: " << B-gastos <<endl;
    return 0;
}
