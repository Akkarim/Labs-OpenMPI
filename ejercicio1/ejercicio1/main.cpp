/* Archivo:  mpi_hola.cpp
* Propósito: típico "hola mundo" que usa MPI.
*
* Compilación:   mpicxx -g -Wall -o mpi_hola mpi_hola.cpp
* Ejecución:     mpiexec -n <num_proc> ./mpi_hola "mensaje entre comillas"
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
#include <sstream>
using namespace std;

//#define DEBUG

void uso(string nombre_prog);

void obt_args(
	char*    argv[]        /* in  */,
	string&  saludo  /* out */);

int obt_linea(
	char entrada[]);

const int MAX_STRING = 100;

int main(int argc, char* argv[]) {
	int mid; // id de cada proceso
	int cnt_proc; // cantidad de procesos
	ostringstream buffer_saludo;
	string saludo;
	string msj_enviado;
	char msj_recibido_arr[MAX_STRING];

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
	obt_args(argv, saludo); // cada proceso obtiene la entrada desde argv
	if (mid != 0) {
		/* Crear el mensaje */
		buffer_saludo << saludo << " desde proceso " << mid << " de " << cnt_proc << endl;
		msj_enviado = buffer_saludo.str();
		/* enviar mensaje al proceso 0 */
		MPI_Send(msj_enviado.c_str(), msj_enviado.size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
	else {
		/* desplegar mensaje */
		cout << saludo << " desde " << mid << " de " << cnt_proc << endl;
		for (int q = 1; q < cnt_proc; q++) {
			/* recibir mensaje del proceso q */
			MPI_Recv(msj_recibido_arr, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			/* desplegar mensaje del proceso q */
			string msj_recibido_str(msj_recibido_arr, obt_linea(msj_recibido_arr));
			cout << msj_recibido_str << endl;
		}
	}

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
   *		saludo: saludo pasado por "línea de comandos".
   */
void obt_args(
	char*    argv[]        /* in  */,
	string&  saludo  /* out */) {

	saludo = argv[1]; // se obtiene saludo pasado por "línea de comandos".

#  ifdef DEBUG
	cout << "saludo = " << saludo << endl;
#  endif
}  /* obt_args */

   /*---------------------------------------------------------------------
   * REQ: N/A
   * MOD: N/A
   * RET: cuenta la cantidad de caracteres en "entrada" hasta '\n'.
   * ENTRAN:
   *		entrada:  arreglo de caracteres con la entrada
   * SALEN:
   *		cantidad .
   */
int obt_linea(
	char entrada[] /* in  */) {
	int i = 0;
	char x;
	while ((x = entrada[i++]) != '\n');
	return i - 1;
} /* obt_linea */
