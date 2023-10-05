// =================================================================
//
// File: exercise01.cpp
// Author(s): Sergio Augusto Macías Corona
// ID: A01352038
// Description: This file contains the code to count the number of
//				even numbers within an array. The time this implementation
//				takes will be used as the basis to calculate the
//				improvement obtained with parallel technologies.
//
// =================================================================

#include <iostream>
#include <iomanip>
#include <chrono>
#include <pthread.h>
#include "utils.h"

using namespace std;
using namespace std::chrono;

#define tam 500000000
//Aqui definimos cuantos nudos utilizaremos
#define Canthilos 4

int *arreglo;

struct ThreadArgs {
    int* dato;
    int TM;
    int numhilo;
    long long PS;
};

// aQUI SE CALCULA LA SUMA EN LOS PARES DEL ARREGLO
void* partialSum(void* args) {
    ThreadArgs* tArgs = static_cast<ThreadArgs*>(args);
    long long sum = 0;
    for (int i = tArgs->numhilo; i < tArgs->TM; i += Canthilos) {
        if (tArgs->dato[i] % 2 == 0) {
            sum += tArgs->dato[i];
        }
    }
    tArgs->PS = sum;
    return nullptr; 
}

int main(int argc, char* argv[]) {
    //SE LLENA EL ARREGLO CON DATOS
    myarray = new int[tam];
    fill_array(arreglo, tam);

    // HILOS
    pthread_t threads[Canthilos];
    ThreadArgs thread_args[Canthilos];
    
    high_resolution_clock::time_point start, end;
    double timeElapsed = 0;
    long long total_sum = 0;

    cout << "Starting...\n";
    
    for (int j = 0; j < Canthilos; j++) {
        thread_args[j].dato = arreglo;
        thread_args[j].TM = tam;
        thread_args[j].numhilo = j;
        
        start = high_resolution_clock::now();

        pthread_create(&threads[j], nullptr, partialSum, &thread_args[j]);

        pthread_join(threads[j], nullptr);

        total_sum += thread_args[j].PS;
        
        end = high_resolution_clock::now();
        timeElapsed += duration<double, std::milli>(end - start).count();
    }
    
    cout << "Nuestro resultado es ---> " << total_sum << "\n";
    cout << "El tiempo promedio es ---> " << fixed << setprecision(3) 
         << (timeElapsed / Canthilos) << " ms\n";

    delete [] arreglo;
    return 0;
}
