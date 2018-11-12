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
	int primos[100000];
	vector<int> vPrime;
	int j = 0, c = 0, n = 0;
	if (mid == 0) {
		cout << "Ingrese el rango superior(>5): " << endl;
		cin >> n;
		vPrime.reserve(n);
		for (int i = 3; i <= n ; i+=2) {
			if (esPrimo(i)) {
				vPrime.push_back(i);
			}
		}
		c = vPrime.size();
	}
	
	MPI_Bcast(&c, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (mid!=0) {
		vPrime.resize(c);
	}

			
	
	MPI_Bcast(&vPrime[0], c, MPI_INT, 0, MPI_COMM_WORLD);
	
	//int local_n = n / cnt_proc;
	int local_n = n-5 / cnt_proc;

	int a, b, x, y, z; // para la suma
	if (mid == 0) {
		for (int i = 6; i < local_n; i++) {
			a = b = c = 2;
			x = y = z = 0;
			if (i % 2 == 0) { // si es par
				while (a + b != i) {
					a = vPrime[x];
					x++;
					if (a + b > i) {
						b = vPrime[y];
						y++;
						x = 0; // Greeeeeeeeeeedy
						a = 2;
					}
				}
				cout << i << " = " << a << " + " << b << endl;
			}
			else { // Si es impar
				while (a + b + c != i) {
					c = vPrime[z];
					z++;
					if (a + b + c > i) { //vamoh a tocar b
						b = vPrime[y];
						y++;
						if (b >= i) { // si llegamos al "top"
							a = vPrime[x];
							x++;
							y = 0;
							b = 2;
							z = 0;
							c = 2;
						}
						else {
							z = 0;
							c = 2;
						}
					}
				}
				cout << i << " = " << a << " + " << b << " + "  << c << endl;
			}
		}
	}
	else {
		for (int i = (local_n*mid) + 1; i < (local_n+1)*mid+1; i++) {
			for (int i = 6; i < local_n; i++) {
				a = b = c = 2;
				x = y = z = 0;
				if (i % 2 == 0) { // si es par
					while (a + b != i) {
						a = vPrime[x];
						x++;
						if (a + b > i) {
							b = vPrime[y];
							y++;
							x = 0; // Greeeeeeeeeeedy
							a = 2;
						}
					}
					cout << i << " = " << a << " + " << b << endl;
				}
				else { // Si es impar
					while (a + b + c != i) {
						c = vPrime[z];
						z++;
						if (a + b + c > i) { //vamoh a tocar b
							b = vPrime[y];
							y++;
							if (b >= i) { // si llegamos al "top"
								a = vPrime[x];
								x++;
								y = 0;
								b = 2;
								z = 0;
								c = 2;
							}
							else {
								z = 0;
								c = 2;
							}
						}
					}
					cout << i << " = " << a << " + " << b << " + " << c << endl;
				}
			}
		}

	}

	/*-------------------------------------------finalización de la ejecución paralela---------------------------------------*/
	if (mid == 0) {
		cin.ignore();
		int felipe = 15;

		cout << "Ya terminé we " << felipe/4 << endl;
	}
	MPI_Barrier(MPI_COMM_WORLD); // para sincronizar la finalización de los procesos

	MPI_Finalize();
	int chit = 0;
	cin >> chit;
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