/* Archivo:      mpi_plantilla.cpp
* Propósito:	Ejercicio 1
*
* Compilación:   mpicxx -g -Wall -o mpi_plantilla mpi_plantilla.cpp
* Ejecución:     mpiexec -n <num_proc> ./mpi_plantilla <secuencia de valores de parámetros>
*/

#include <mpi.h> 
#include <iostream>
using namespace std;

#define DEBUG

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

	/*-------------------ejecución del proceso principal--------------------*/
	
	double aciertosGlobales = 0.0; //Aciertos globales
	double aciertosLocales = 0.0; //Aciertos locales
	double x = 0.0;
	double y = 0.0;
	double resultado = 0.0;
	
	double start, stop;

	int intentos = 0;

	if (mid == 0) { // Si es el proceso 0
		cout << "Digite la cantidad de intentos: " << endl;
		cin >> intentos;
	}
	
	start = MPI_Wtime();

	MPI_Bcast(&intentos, 1, MPI_INT, 0, MPI_COMM_WORLD); // Manda cantidad de intentos a todos
	int rango = intentos / (cnt_proc); // lo que tiene que hacer cada proceso

	for (int i = 1 ; i < rango; i++) {
		x = ((double)rand() / (RAND_MAX + 1) + (rand() % 2) - 1.0);
		y = ((double)rand() / (RAND_MAX + 1) + (rand() % 2) - 1.0);
		
		if (((x*x) + (y*y)) <= 1) {
			aciertosLocales++;	
		}
	}

	MPI_Reduce(&aciertosLocales, &aciertosGlobales, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	stop = MPI_Wtime();

	if (mid == 0) {
		resultado = 4 * (aciertosGlobales) / ((double)intentos);
		cout << "La aproximación es: " << resultado << " con " << aciertosGlobales << " aciertos." << endl;
		cout << "Uso " << intentos << " intentos" << " y dura " << stop-start << endl;
	}




	/*------------------finalización de la ejecución paralela----------------*/
	if (mid == 0)
		cin.ignore();
	MPI_Barrier(MPI_COMM_WORLD); // para sincronizar la finalización de los procesos

	MPI_Finalize();
	int shit = 0;
	cin >> shit;
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

/*
Betty						__.----.___
--|            ||  (\(__)/)-'||      ;--` ||
_||____________||___`(QQ)'___||______;____||_
-||------------||----)  (----||-----------||-
_||____________||___(o  o)___||______;____||_
-||------------||----`--'----||-----------||-
 ||            ||       `|| ||| || ||     ||
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
*/
