/* Archivo:      mpi_plantilla.cpp
* Prop�sito:	Ejercicio 1
*
* Compilaci�n:   mpicxx -g -Wall -o mpi_plantilla mpi_plantilla.cpp
* Ejecuci�n:     mpiexec -n <num_proc> ./mpi_plantilla <secuencia de valores de par�metros>
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
	MPI_Status mpi_status; // para capturar estado al finalizar invocaci�n de funciones MPI

						   /* Arrancar ambiente MPI */
	MPI_Init(&argc, &argv);             		/* Arranca ambiente MPI */
	MPI_Comm_rank(MPI_COMM_WORLD, &mid); 		/* El comunicador le da valor a id (rank del proceso) */
	MPI_Comm_size(MPI_COMM_WORLD, &cnt_proc);  /* El comunicador le da valor a p (n�mero de procesos) */

#  ifdef DEBUG 
	if (mid == 0)
		cin.ignore();
	MPI_Barrier(MPI_COMM_WORLD);
#  endif

	/*-------------------ejecuci�n del proceso principal--------------------*/
	
	double aciertosGlobales = 0.0; //Aciertos globales
	double x = 0.0;
	double y = 0.0;
	double resultado = 0.0;
	int intentos;

	cout << "Digite la cantidad de procesos: " << endl;
	cin >> cnt_proc;
	cout << "Digite la cantidad de intentos: " << endl;
	cin >> intentos;



	/*------------------finalizaci�n de la ejecuci�n paralela----------------*/
	if (mid == 0)
		cin.ignore();
	MPI_Barrier(MPI_COMM_WORLD); // para sincronizar la finalizaci�n de los procesos

	MPI_Finalize();
	return 0;
}  /* main */

   /*---------------------------------------------------------------------
   * REQ: N/A
   * MOD: N/A
   * EFE: despliega mensaje indicando c�mo ejecutar el programa y pasarle par�metros de entrada.
   * ENTRAN:
   *		nombre_prog:  nombre del programa
   * SALEN: N/A
   */
void uso(string nombre_prog /* in */) {
	cerr << nombre_prog.c_str() << " secuencia de par�metros de entrada" << endl;
	exit(0);
}  /* uso */

   /*---------------------------------------------------------------------
   * REQ: N/A
   * MOD: dato_salida
   * EFE: obtiene los valores de los argumentos pasados por "l�nea de comandos".
   * ENTRAN:
   *		nombre_prog:  nombre del programa
   * SALEN:
   *		dato_salida: un dato de salida con un valor de argumento pasado por "l�nea de comandos".
   */
void obt_args(
	char*    argv[]        /* in  */,
	int&     dato_salida  /* out */) {

	dato_salida = strtol(argv[1], NULL, 10); // se obtiene valor del argumento 1 pasado por "l�nea de comandos".

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
