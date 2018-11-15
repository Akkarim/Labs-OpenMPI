/* Archivo:      mpi_plantilla.cpp
* Propósito:   ....
*
* Compilación:   mpicxx -g -Wall -o mpi_plantilla mpi_plantilla.cpp
* Ejecución:     mpiexec -n <num_proc> ./mpi_plantilla <secuencia de valores de parámetros>
*
* Entradas:     ...
* Salidas:    ...
*
* Notas:
* 1.  bandera DEBUG produce salida detallada para depuración.
*
*/

#include <mpi.h> 
#include <iostream>
using namespace std;

//#define DEBUG

void uso(string nombre_prog);

void obt_args(
	char*    argv[]        /* in  */,
	int&     dato_salida  /* out */);

int main(int argc, char* argv[]) {
	int mid; // id de cada proceso
	int cnt_proc; // cantidad de procesos
	MPI_Status mpi_status; // para capturar estado al finalizar invocación de funciones MPI

						   /* Arrancar ambiente MPI */
	MPI_Init(&argc, &argv);             		/* Arranca ambiente MPI */
	MPI_Comm_rank(MPI_COMM_WORLD, &mid); 		/* El comunicador le da valor a id (rank del proceso) */
	MPI_Comm_size(MPI_COMM_WORLD, &cnt_proc);  /* El comunicador le da valor a p (número de procesos) */

#  ifdef DEBUG 
	if (mid == 0)
		cin.ignore();
	MPI_Barrier(MPI_COMM_WORLD);
#  endif

	/* ejecución del proceso principal */

	/* finalización de la ejecución paralela */
	if (mid == 0)
		cin.ignore();
	MPI_Barrier(MPI_COMM_WORLD); // para sincronizar la finalización de los procesos

	MPI_Finalize();
	return 0;
}  /* main */

   /*---------------------------------------------------------------------
   * REQ: N/A
   * MOD: N/A
   * EFE: despliega mensaje indicando cómo ejecutar el programa y pasarle parámetros de entrada.
   * ENTRAN:
   *		nombre_prog:  nombre del programa
   * SALEN: N/A
   */
void uso(string nombre_prog /* in */) {
	cerr << nombre_prog.c_str() << " secuencia de parámetros de entrada" << endl;
	exit(0);
}  /* uso */

   /*---------------------------------------------------------------------
   * REQ: N/A
   * MOD: dato_salida
   * EFE: obtiene los valores de los argumentos pasados por "línea de comandos".
   * ENTRAN:
   *		nombre_prog:  nombre del programa
   * SALEN:
   *		dato_salida: un dato de salida con un valor de argumento pasado por "línea de comandos".
   */
void obt_args(
	char*    argv[]        /* in  */,
	int&     dato_salida  /* out */) {

	dato_salida = strtol(argv[1], NULL, 10); // se obtiene valor del argumento 1 pasado por "línea de comandos".

#  ifdef DEBUG
	cout << "dato_salida = " << dato_salida << endl;
#  endif
}  /* obt_args */












/*#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include <math.h>
#include <random>
#include <algorithm>
//#include <yourself>
//#include <love>
#include <time.h>


using namespace std;


bool validaCntHilos(int ch);

//vector<int>* mergesort(int* inicio, int* final, vector<int>* vector);


int main(int argc, char* argv[]) {
	vector<int> aleatorios; //Lista de números
	vector<int> resultado; //Resultados de Merge
	vector<vector<int>> vectores; // Vector de Vectores
	vector<int> myVector; // El que guardamos las copias de aleatorios 
	vector<int> merge1;
	vector<int> merge2;

	int tCount = 0;
	int	n, data_count, local_inicio, local_final, local_n, my_Rank;

	while (!validaCntHilos(tCount)) {
		cout << "Digite la cantidad de hilos ( >= 1 ): ";
		cin >> tCount;
		cout << "Cantidad de numeros: Ajua, aua aua" << endl;
		cin >> data_count;
		cout << endl;
	}

	int k = 0;
	aleatorios.reserve(data_count);
	vectores.reserve(data_count);
	local_n = data_count / tCount;

	for (int i = 0; i < data_count; i++) {
		k = rand() % 100 + 1;
		aleatorios.push_back(k);
	}

	myVector.reserve(local_n);

#pragma omp parallel num_threads(tCount) private(local_inicio,local_final,my_Rank, myVector)
	{
		myVector.clear();//JustInCase
		my_Rank = omp_get_thread_num();
		local_inicio = local_n * my_Rank;
		local_final = local_n + local_inicio;
		sort((aleatorios.begin() + local_inicio), (aleatorios.begin() + local_final));
		for (int i = local_inicio; i < local_final; i++) {
			myVector.push_back(aleatorios[i]); //Hay que cambiar esto para la versión paralela del merge
		}
#pragma omp critical
		vectores.push_back(myVector);
	}

	std::cout << "Lista contiene:";
	for (std::vector<int>::iterator it = aleatorios.begin(); it != aleatorios.end(); ++it)
		std::cout << ' ' << *it;
	cout << '\n';
	
	
	int i2, f2; //-------------------------------------Merge sin paralelizar------------------------------------------------
	int mitad = aleatorios.size()/ 2;
	resultado.resize(data_count);
	local_final = local_n;
	merge1.reserve(local_n);
	merge2.reserve(local_n);
	while(vectores.size()!=1){
		resultado.clear();
		resultado.resize(vectores[0].size()*2, 0);
		merge1 = vectores[0];
		merge2 = vectores[1];
		merge(merge1.begin(), merge1.end(), merge2.begin(), merge2.end(), resultado.begin());
		aleatorios = resultado;
		vectores.push_back(aleatorios);
		vectores.erase(vectores.begin());
		vectores.erase(vectores.begin());
	}

	std::cout << "El resultado es: ";
	for (std::vector<int>::iterator it = resultado.begin(); it != resultado.end(); it++)
		std::cout << ' ' << *it;
	std::cout << '\n';
	//---------------------------------------------------------------------------------------------------------------------

	cin >> n;
	return 0;
}

bool validaCntHilos(int ch) {
	return ch >= 1;
}*/