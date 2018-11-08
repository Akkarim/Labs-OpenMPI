#include <mpi.h> 
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//#define DEBUG

void uso(string nombre_prog);

void obt_args(
	char*    argv[]        /* in  */,
	int&     dato_salida  /* out */);

bool esPrimo(int n);
const int n = 100;
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

	/*-------------------------------------------ejecución del proceso principal--------------------------------------------*/
	vector<int> primos;
	int n = 0;
	if (mid == 0) {
		cout << "Ingrese el rango superior(>5): " << endl;
		cin >> n;
	}

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i <= (n/cnt_proc)*mid; i++) { // ver bien la cuestión 
		if (esPrimo(i))
			primos.push_back(i);
	}
	int local_n = n / cnt_proc;
	int difencia = 0;
	for (int actual = 5; primos[actual] < local_n/2; actual++) {
		difencia = n - primos[actual];
		if (!esPrimo(actual)) {
			for (int actual = 5; primos[actual] < local_n / 2; actual++) {
			
			}
		}
		else {
			if (binary_search(primos.begin(), primos.end(), difencia)) {
				cout << primos[actual] << " + " << difencia << " = " << n << endl;
			}
		}
	/*-------------------------------------------finalización de la ejecución paralela-------------------------------------*/
	if (mid == 0)
		cin.ignore();
	MPI_Barrier(MPI_COMM_WORLD); // para sincronizar la finalización de los procesos

	MPI_Finalize();
	int x = 0;
	cin >> x;
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


bool esPrimo(int n) {
	if (n <= 3) return n >= 2;
	if (n % 2 == 0 || n % 3 == 0) return false;
	for (int i = 5;i*i <= n;i += 6) {
		if (n%i == 0 || n % (i + 2) == 0) return false;
	}
	return true;
}

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