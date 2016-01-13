#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <vector>
#include <pthread.h>
#include <iostream>
#include <algorithm>
#include "comunicacion.h"
#define DEFNUMS 3500
#define MYMAX 1000
using namespace std;
char ip[16];
int localPort = 5432;

struct AuxHiloBucket{
	char ip[16];
	int puerto;
	int superior;
	int inferior;
	int size;
	std::vector<int> numeros;
	
	void agregaNumero(int n){
		size++;
		numeros.push_back(n);
	}
};

void *procesaCubeta(void * auxiliar);
void rellenaCubetas(AuxHiloBucket * arr, int cubetas);
void iniciaSemilla();

int main(int argc, char** argv){
	
	iniciaSemilla();
	
	/*
		Pedimos el número de cubetas por medio de la entrada estandar
		y reservamos en memoria el espacio para nuestro struct auxiliar 
		y para los hilos equivalente al número de cubetas.
	*/
	int cubetas;
	AuxHiloBucket * auxiliares;
	pthread_t * hilos;
	scanf("%d",&cubetas);

	auxiliares = (AuxHiloBucket *)malloc(cubetas * sizeof(AuxHiloBucket));
	hilos = (pthread_t *)malloc(cubetas * sizeof(pthread_t));
	
	/*
		Mandamos llamar a la función para llenar las cubetas.
	*/
	rellenaCubetas(auxiliares, cubetas);

	/*
		Por medio de la entrada estandar pedimos tanto las ips com los puertos.
	*/
	for(int j = 0; j < cubetas; j++){
		scanf("%s",auxiliares[j].ip);
		scanf("%d",&auxiliares[j].puerto);
	}
	
	/* 
		Creamos los hilos.
		Para cada hilos haremos que procese el vector de elementos
	*/
	for(int i = 0; i < cubetas; i++){
		if(pthread_create(&hilos[i], NULL, procesaCubeta, (void *)&auxiliares[i]) != 0){
			perror("Error al crear los hilos.");
		}
	}
	
	//Lo unimos.
	for (int i = 0; i < cubetas; i++) 
		pthread_join (hilos[i], NULL);
	
	//Al final solo imprimimos lo que el servidor haya procesado.	
	for (int i = 0; i < cubetas; i++)
		for(int j = 0; j < auxiliares[i].numeros.size(); j++)
			printf(" %d ",auxiliares[i].numeros[j]);
		
	return 0;
}

void iniciaSemilla(){
	srand(time(NULL));
}

void * procesaCubeta(void * auxiliar){
	AuxHiloBucket * a = (AuxHiloBucket *) auxiliar;
	int vsize = sizeof(std::vector<int>) + (sizeof(int) * a->numeros.size());
	int sd = comunicacion();
	bool verdadero = ordenaNumeros(a->numeros,sd,a->ip,a->puerto);
}

void rellenaCubetas(AuxHiloBucket * arr, int cubetas){	
	
	int rango = ceil(((double)MYMAX) / ((double)cubetas));

	// Generamos los numeros aleatorios y los insertamos en la cubeta correspondiente
	for(int i=0;i<DEFNUMS;i++){
		int nn = rand();
		
		nn = nn % MYMAX;
		for(int j = 0; j < cubetas; j++){	
			arr[j].inferior = j * rango;
			arr[j].superior = (j+1) * rango;
			if(nn >= arr[j].inferior &&  nn < arr[j].superior){
				arr[j].agregaNumero(nn);
			}
		}
	}
}

