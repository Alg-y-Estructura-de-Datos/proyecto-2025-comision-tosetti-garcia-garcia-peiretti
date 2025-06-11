#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include <iostream>
using namespace std;
// M�todo para ordenar un arreglo de enteros utilizando el algoritmo QuickSort.
// Par�metros:
//   arr: arreglo de enteros a ordenar.
//   inicio: �ndice inicial del subarreglo a ordenar.
//   fin: �ndice final del subarreglo a ordenar.
//
// Descripci�n:
//   QuickSort es un algoritmo de ordenamiento eficiente basado en el principio de divide y vencer�s.
//   Funciona seleccionando un elemento como pivote y reorganizando el arreglo de tal manera que todos
//   los elementos menores que el pivote se colocan a su izquierda y los mayores a su derecha. Luego,
//   se aplica el mismo proceso recursivamente a las sublistas de la izquierda y la derecha del pivote.
//
// Funcionamiento:
//   - Se selecciona el elemento en la posici�n media como pivote.
//   - Se utilizan dos �ndices, 'i' y 'j', que comienzan en los extremos del subarreglo y se mueven hacia
//     el centro. 'i' se mueve hacia la derecha buscando elementos mayores al pivote, mientras que 'j' se
//     mueve hacia la izquierda buscando elementos menores.
//   - Cuando se encuentran dos elementos fuera de lugar (uno mayor que el pivote en la izquierda y otro
//     menor en la derecha), se intercambian. El proceso contin�a hasta que los �ndices 'i' y 'j' se cruzan.
//   - Despu�s del intercambio, QuickSort se llama recursivamente para ordenar las dos mitades del arreglo.
//
// Complejidad Temporal:
//   - Peor caso: O(n�), cuando el pivote es el menor o mayor elemento en cada partici�n, lo que puede
//     suceder si el arreglo ya est� ordenado o en orden inverso.
//   - Caso promedio: O(n log n), la complejidad esperada en la mayor�a de los casos cuando el pivote
//     divide el arreglo de manera razonablemente equilibrada.
//   - Mejor caso: O(n log n), ocurre cuando el pivote divide el arreglo en partes iguales o casi iguales.
//
// Complejidad Espacial:
//   - O(log n), ya que QuickSort es un algoritmo in-place y utiliza espacio adicional en la pila para las
//     llamadas recursivas. En el peor caso, el uso de espacio puede ser O(n).
//
// Ventajas:
//   - QuickSort es uno de los algoritmos m�s r�pidos en la pr�ctica para ordenar grandes arreglos.
//   - Es un algoritmo in-place, lo que significa que no requiere memoria adicional significativa.
//
// Desventajas:
//   - El rendimiento puede degradarse a O(n�) si no se implementa una buena estrategia para la elecci�n
//     del pivote.
//   - El uso de recursi�n puede ser una limitaci�n en sistemas con pilas limitadas.
//
// Recomendaciones:
//   - Para evitar el peor caso, se recomienda utilizar una estrategia mejorada de selecci�n del pivote,
//     como la "mediana de tres" o un pivote aleatorio.
//
void quickSort(int *arr, int inicio, int fin)
{
    int i, j, medio;
    int pivot, aux;

    medio = (inicio + fin) / 2;
    pivot = arr[medio]; // Se elige el pivote como el elemento medio
    i = inicio;
    j = fin;

    do
    {
        // Encuentra el primer elemento que deber�a ir a la derecha del pivote
        while (arr[i] < pivot) i++;
        // Encuentra el primer elemento que deber�a ir a la izquierda del pivote
        while (arr[j] > pivot) j--;

        // Intercambia los elementos si est�n en posiciones incorrectas
        if (i <= j)
        {
            aux = arr[i];
            arr[i] = arr[j];
            arr[j] = aux;
            i++;
            j--;
        }
    } while (i <= j);

    // Ordena recursivamente las subporciones izquierda y derecha
    if (inicio < j)
        quickSort(arr, inicio, j); // Subarreglo izquierdo
    if (i < fin)
        quickSort(arr, i, fin);     // Subarreglo derecho
}




#endif // QUICKSORT_H_

