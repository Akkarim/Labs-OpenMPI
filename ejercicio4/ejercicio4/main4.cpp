#include <mpi.h> 
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <math.h>
#include <algorithm>
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

	/*-----------------------------------------ejecución del proceso principal-----------------------------------------*/
	int cnt_proc_local = 0, local_n = 0, n = 0, max = 0, random = 0;
	double tStart =  0.0, tStop = 0.0, local_elapsed = 0.0, elapsed = 0.0;
	int* arrLocal; // Arreglo para cada proceso
	int* arrFinal; //Donde queda todo, se imprime
	int* arrAux; // Arreglo para cambios

	if (mid == 0) {
		cout << "Ingrese la cantidad de números: " << endl;
		cin >> n;
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	cnt_proc_local = cnt_proc; //cnt_proc_local = cnt_proc_act, cnt_proc = cnt_proc_org
	local_n = (n/cnt_proc_local);
	arrLocal = (int*)malloc(n * sizeof(int));
	arrFinal = (int*)malloc(n * sizeof(int));
	arrAux = (int*)malloc(n * sizeof(int));
	/*Random*/
	tStart = MPI_Wtime();
	if (mid == 0) {
		for (int i = 0; i < n; i++) {
			random = rand() % n; // random común de c++
			arrFinal[i] = random;
		}
	}
	MPI_Scatter(arrFinal, local_n, MPI_INT, arrLocal, local_n, MPI_INT, 0, MPI_COMM_WORLD);
	
	sort(arrLocal, arrLocal + local_n);
	max = cnt_proc_local;

	
	while (cnt_proc_local > 1) {
		cnt_proc_local /= 2;
		if (mid >= cnt_proc_local && mid < max) {
			MPI_Send(arrLocal, local_n, MPI_INT, mid - cnt_proc_local, 0, MPI_COMM_WORLD);
		}
		if (mid < cnt_proc_local) {
			MPI_Recv(arrAux, local_n, MPI_INT, mid + cnt_proc_local, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE); //puede faltar MPI_STATUSES_IGNORE 
			merge(arrLocal, arrLocal + local_n, arrAux, arrAux + local_n, arrFinal);
			local_n *= 2;
			arrLocal = &arrFinal[0];
		}
		max /= 2;
		MPI_Barrier(MPI_COMM_WORLD);
	}
	tStop = MPI_Wtime();
	local_elapsed = tStop - tStart; 
	MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if (mid == 0) {
		cout << "Tiempo transcurrido = " << elapsed << endl;
	}
	if (mid == 0 && cnt_proc_local > 1) {
		for (int i = 0; i < n; i++) {
			cout << arrFinal[i] << ",";
		}
	}
	else if (mid == 0 && cnt_proc_local == 1) {
		for (int i = 0; i < n; i++) {
			cout << arrLocal[i] << ",";
		}
	}

	free(arrFinal);
	free(arrAux);


	/*--------------------------------------finalización de la ejecución paralela-------------------------------------*/
	int e;
	cin >> e;
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

   //End of file with a Cow (Bettsy)
   //                               __.----.___
   //   ||            ||  (\(__)/)-'||      ;--` ||
   //  _||____________||___`(QQ)'___||______;____||_
   //  -||------------||----)  (----||-----------||-
   //  _||____________||___(o  o)___||______;____||_
   //  -||------------||----`--'----||-----------||-
   //   ||            ||       `|| ||| || ||     ||(L)FD
   //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^




