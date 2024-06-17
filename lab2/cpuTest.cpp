#include <cstdio>
#include <omp.h>
#include <sys/time.h>
#include <stdlib.h>	
#include <inttypes.h>
#include <cmath>
#include <string>
#include <fstream>
#include <cstring>

#define COUNT_OPERATIONS 100

double wtime()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

template<typename T>
void matrix_vector_product(T *a, T *b, T *c, int m, int n) {
	for (int i = 0; i < m; i++) {
		c[i] = 0.0;
		for (int j = 0; j < n; j++)
			c[i] += a[i * n + j] * b[j];
	}
}

template<typename T>
double run_serial(int m, int n) {
	T *a, *b, *c;
	a = new T[n * m];
	b = new T[n];
	c = new T[m];
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			a[i * n + j] = i + j;
	}
	for (int j = 0; j < n; j++)
		b[j] = j;
	double t = wtime();
	matrix_vector_product(a, b, c, m, n);
	t = wtime() - t;
	delete(a);
	delete(b);
	delete(c);
	return t;
}

double maxElem(double* timeBuf) {
	double max = timeBuf[0];
	for (int i = 1; i < 10; i++) {
		if (max < timeBuf[i]) {
			max = timeBuf[i];
		}
	}
	return max;
}

double calcDisp(double* timeBuf, double averageTime) {
	double disp = 0;
	for (int i = 0; i < 10; i++) {
		disp += pow(timeBuf[i] - averageTime, 2);
	}
	return disp / 10;
}

double calcAverageTime(double* timeBuf) {
	double averageTime = 0;
	for (int i = 0; i < 10; i++)
		averageTime += timeBuf[i];
	averageTime /= 10;
	printf("Average time is %.3fsec.\n", averageTime);
	return averageTime;	
}

double calcW(double* timeBuf, double averageTime) {	
	double omega = (pow(COUNT_OPERATIONS, 2) / averageTime) / 1000000;
	printf("W = %.0f\n", omega);
	return omega;
}

void writeFile(double* timeBuf, double averageTime, double omega, double relErr, double absErr, double disp, double otclon) {
	std::ofstream fd("omega.txt");
	fd << "TimeBuf:\t";
	for (int i = 0; i < 10; i++) {
		timeBuf[i] = round(timeBuf[i] * 1000) / 1000;
		fd << timeBuf[i] << '\t';
	}
	fd << "\nAvTime=\t" << round(averageTime * 1000) / 1000 << "sec.\nW=\t" << omega << '\n';
	fd << "relErr=\t" << round(relErr * 1000) / 1000 << "sec.\nabsErr=\t" << round(absErr * 1000) / 1000
	 << "%\nDisp=\t" << disp << "\nDeviation=\t" << round(otclon * 10000) / 10000 << '\n';
	fd.close();
}

int main(int argc, char **argv)
{
	int m, n;
	m = n = COUNT_OPERATIONS;
	double t;
	double timeBuf[10];
	//printf("Matrix-vector product (c[m] = a[m, n] mult b[n]; m = %d, n = %d)\n", m, n);
	if (argc == 2) {
		if (strcmp(argv[1], "int") == 0) {
			for (int i = 0; i < 10; i++) {
				t = run_serial<int>(m, n);
				//printf("Test #%d: Elapsed time (serial): %.6f sec.\n", i + 1,t);
				timeBuf[i] = t;
			}
		}
		if (strcmp(argv[1], "double") == 0) {
			for (int i = 0; i < 10; i++) {
				t = run_serial<double>(m, n);
				//printf("Test #%d: Elapsed time (serial): %.6f sec.\n", i + 1,t);
				timeBuf[i] = t;
			}
		}
		if (strcmp(argv[1], "float") == 0) {
			for (int i = 0; i < 10; i++) {
				t = run_serial<float>(m, n);
				//printf("Test #%d: Elapsed time (serial): %.6f sec.\n", i + 1,t);
				timeBuf[i] = t;
			}
		}
		double averageTime = calcAverageTime(timeBuf);
		double omega = calcW(timeBuf, averageTime);
		double absErr = fabs(maxElem(timeBuf) - averageTime);
		double relErr = absErr / averageTime * 100;
		double disp = calcDisp(timeBuf, averageTime);
		double otclon = sqrt(disp);
		writeFile(timeBuf, averageTime, omega, relErr, absErr, disp, otclon);
	}
	return 0;
}


//plot "linear.txt" with linespoints title "Linear speedup", "15.txt" with linespoints pointtype 4 title "N = 15000", "20.txt" with linespoints pointtype 8 title "N = 20000", "25.txt" with linespoints pointtype 3 title "N = 25000"
