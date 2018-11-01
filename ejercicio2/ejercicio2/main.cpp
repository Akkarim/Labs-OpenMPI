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
#include <vector>
#include <string>
using namespace std;

//#define DEBUG

void uso(string nombre_prog);

void obt_args(
	char*    argv[]        /* in  */,
	int&     dato_salida  /* out */);

void usage(string prog_name);

void get_args(
	char*    argv[]        /* in  */,
	int&     bin_count_p   /* out */,
	float&   min_meas_p    /* out */,
	float&   max_meas_p    /* out */,
	int&     data_count_p  /* out */);

void gen_data(
	float   min_meas    /* in  */,
	float   max_meas    /* in  */,
	int		tCount		/* in */,
	vector<float>&   data /* out */,
	int     data_count  /* in  */);

void gen_bins(
	float min_meas      /* in  */,
	float max_meas      /* in  */,
	vector<float>& bin_maxes   /* out */,
	vector<int>&   bin_counts  /* out */,
	int   bin_count     /* in  */);

int which_bin(
	float    data         /* in */,
	vector<float>&    bin_maxes  /* in */,
	int      bin_count    /* in */,
	float    min_meas     /* in */);

void print_histo(
	vector<float>    bin_maxes   /* in */,
	vector<int>     bin_counts   /* in */,
	int      bin_count     /* in */,
	float    min_meas      /* in */);

int main(int argc, char* argv[]) {
	int mid; // id de cada proceso
	int cnt_proc; // cantidad de procesos
	MPI_Status mpi_status; // para capturar estado al finalizar invocación de funciones MPI

	int bin_count = 0;    // cantidad de bins, bin actual, bin == rango quitamos el tCount
	int bin = 0;
	float min_meas, max_meas; // valor inferior de datos, valor superior de datos
	vector<float> bin_maxes;  // vector de m�ximos por bin
	vector<int> bin_counts_local;   // vector para contar valores por bin
	vector<int> bin_counts_global;
	int data_count = 0;     // cantidad de datos
	vector<float> data;    // vector de dato2

	if (argc != 5) usage(argv[0]);
	get_args(argv, bin_count, min_meas, max_meas, data_count);
						   /* Arrancar ambiente MPI */
	MPI_Init(&argc, &argv);             		/* Arranca ambiente MPI */
	MPI_Comm_rank(MPI_COMM_WORLD, &mid); 		/* El comunicador le da valor a id (rank del proceso) */
	MPI_Comm_size(MPI_COMM_WORLD, &cnt_proc);  /* El comunicador le da valor a p (número de procesos) */

#  ifdef DEBUG 
	if (mid == 0)
		cin.ignore();
	MPI_Barrier(MPI_COMM_WORLD);
#  endif

	/*--------------------------------ejecución del proceso principal------------------------------------*/
	bin_maxes.resize(bin_count);
	bin_counts_local.resize(bin_count);
	bin_counts_global.resize(bin_count);
	data.resize(data_count/cnt_proc);

	/* Generate the data */
	gen_data(min_meas, max_meas, cnt_proc, data, data_count/cnt_proc);

	/* Create bins for storing counts */
	gen_bins(min_meas, max_meas, bin_maxes, bin_counts_local, bin_count);


	/* Count number of values in each bin */
	for (int i = 0; i < data_count/cnt_proc; i++) {
		bin = which_bin(data[i], bin_maxes, bin_count, min_meas);
		bin_counts_local[bin]++;
	}

	MPI_Reduce(bin_counts_local.data(), bin_counts_global.data(), bin_count, MPI_INT,MPI_SUM, 0, MPI_COMM_WORLD);

#  ifdef DEBUG
	cout << "bin_counts = ";
	for (int i = 0; i < bin_count; i++)
		cout << " " << bin_counts_local[i];
	cout << endl;
#  endif

	/* Print the histogram */
	cout << endl << endl;
	if (mid == 0)
		print_histo(bin_maxes, bin_counts_global, bin_count, min_meas);

	/*-----------------------------------finalización de la ejecución paralela-----------------------------*/
	if (mid == 0) {
		int x = 0;
		cin >> x;
		cin.ignore();
	}
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

   /*---------------------------------------------------------------------
   * Function:  usage
   * Purpose:   Print a message showing how to run program and quit
   * In arg:    prog_name:
   Luis Carvajal, [29.08.18 16:47]
   the name of the program from the command line
   */
void usage(string prog_name /* in */) {
	cerr << "usage: " << prog_name << "<bin_count> <min_meas> <max_meas> <data_count>\n" << endl;
	exit(0);
}  /* usage */

   /*---------------------------------------------------------------------
   * Function:  get_args
   * Purpose:   Get the command line arguments
   * In arg:    argv:  strings from command line
   * Out args:  bin_count_p:   number of bins
   *            min_meas_p:    minimum measurement
   *            max_meas_p:    maximum measurement
   *            data_count_p:  number of measurements
   */
void get_args(
	char*    argv[]        /* in  */,
	int&     bin_count_p   /* out */,
	float&   min_meas_p    /* out */,
	float&   max_meas_p    /* out */,
	int&     data_count_p  /* out */) {

	bin_count_p = strtol(argv[1], NULL, 10);
	min_meas_p = strtof(argv[2], NULL);
	max_meas_p = strtof(argv[3], NULL);
	data_count_p = strtol(argv[4], NULL, 10);

#  ifdef DEBUG 
	cout << "bin_count = " << bin_count_p << endl;
	cout << "min_meas = " << min_meas_p << "max_meas = " << max_meas_p << endl;
	cout << "data_count = " << data_count_p << endl;
#  endif
}  /* get_args */

   /*---------------------------------------------------------------------
   * Function:  gen_data
   * Purpose:   Generate random floats in the range min_meas <= x < max_meas
   * In args:   min_meas:    the minimum possible value for the data
   *            max_meas:    the maximum possible value for the data
   *            data_count:  the number of measurements
   * Out arg:   data:        the actual measurements
   */
void gen_data(
	float   min_meas    /* in  */,
	float   max_meas    /* in  */,
	int		tCount		/* in */,
	vector<float>&   data /* out */,
	int     data_count  /* in  */) {

	srand(0);
	for (int i = 0; i < data_count; i++)//Debería ir un menos uno, en caso de fallo, ese es un caso probable//OVEJAS_LANUDAS
		data[i] = min_meas + (max_meas - min_meas)*rand() / ((double)RAND_MAX);

#  ifdef DEBUG
	cout << "data = ";
	for (int i = 0; i < data_count; i++)
		cout << " ", data[i];
		cout << endl;
#  endif
}  /* gen_data */

   /*---------------------------------------------------------------------
   * Function:  gen_bins
   * Purpose:   Compute max value for each bin, and store 0 as the
   *            number of values in each bin
   * In args:   min_meas:   the minimum possible measurement
   *            max_meas:   the maximum possible measurement
   *            bin_count:  the number of bins
   * Out args:  bin_maxes:  the maximum possible value for each bin
   *            bin_counts: the number of data values in each bin
   */
void gen_bins(
	float min_meas      /* in  */,
	float max_meas      /* in  */,
	vector<float>& bin_maxes   /* out */,
	vector<int>&   bin_counts  /* out */,
	int   bin_count     /* in  */) {
	float bin_width;

	bin_width = (max_meas - min_meas) / bin_count;

	for (int i = 0; i < bin_count; i++) {
		bin_maxes[i] = min_meas + (i + 1)*bin_width;
		bin_counts[i] = 0;
	}

#  ifdef DEBUG
	cout << "bin_maxes = ";
	for (int i = 0; i < bin_count; i++)
		cout << " " << bin_maxes[i];
		cout << endl;
#  endif
}  /* gen_bins */

   /*---------------------------------------------------------------------
   * Function:  which_bin
   * Purpose:   Use binary search to determine which bin a measurement
   *            belongs to
   * In args:   data:       the current measurement
   *            bin_maxes:  list of max bin values
   *            bin_count:  number of bins
   *            min_meas:   the minimum possible measurement
   * Return:    the number of the bin to which data belongs
   * Notes:
   * 1.  The bin to which data belongs satisfies
   *
   *            bin_maxes[i-1] <= data < bin_maxes[i]
   *
   *     where, bin_maxes[-1] = min_meas
   * 2.  If the search fails, the function prints a message and exits
   */
int which_bin(
	float    data         /* in */,
	vector<float>&    bin_maxes  /* in */,
	int      bin_count    /* in */,
	float    min_meas     /* in */) {
	int bottom = 0, top = bin_count - 1;
	int mid;
	float bin_max, bin_min;

	while (bottom <= top) {
		mid = (bottom + top) / 2;
		bin_max = bin_maxes[mid];
		bin_min = (mid == 0) ? min_meas : bin_maxes[mid - 1];
		if (data >= bin_max)
			bottom = mid + 1;
		else if (data < bin_min)
			top = mid - 1;
		else
			return mid; // Puede haber dualidad de nombre
	}

	/* Whoops! */
	cerr << "Data = " << data << " doesn't belong to a bin!" << endl;
	cerr << "Quitting" << endl;
	exit(-1);
}  /* which_bin */

   /*---------------------------------------------------------------------
   * Function:  print_histo
   * Purpose:   Print a histogram.  The number of elements in each
   *            bin is shown by an array of X's.
   * In args:   bin_maxes:   the max value for each bin
   *            bin_counts:  the number of elements in each bin//*******************************************
   *            bin_count:   the number of bins
   *            min_meas:    the minimum possible measurment
   */
void print_histo(
	vector<float>    bin_maxes   /* in */,
	vector<int>     bin_counts   /* in */,
	int      bin_count     /* in */,
	float    min_meas      /* in */) {
	float bin_max, bin_min;

	for (int i = 0; i < bin_count; i++) {
		bin_max = bin_maxes[i];
		bin_min = (i == 0) ? min_meas : bin_maxes[i - 1];
		printf("%.2f-%.2f:\t", bin_min, bin_max);
		for (int j = 0; j < bin_counts[i]; j++)
			cout << "X";
		cout << endl;
	}

}  /* print_histo */

   //End of file with a Cow (Bettsy)
   //                               __.----.___
   //   ||            ||  (\(__)/)-'||      ;--` ||
   //  _||____________||___`(QQ)'___||______;____||_
   //  -||------------||----)  (----||-----------||-
   //  _||____________||___(o  o)___||______;____||_
   //  -||------------||----`--'----||-----------||-
   //   ||            ||       `|| ||| || ||     ||
   //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^