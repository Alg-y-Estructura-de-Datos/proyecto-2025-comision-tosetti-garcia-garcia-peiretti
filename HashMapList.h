#ifndef U05_HASH_HASHMAP_HASHMAPLIST_H_
#define U05_HASH_HASHMAP_HASHMAPLIST_H_
#include "HashEntry.h"
#include "Lista.h"

//Tabla hash con manejo de colisiones usando listas enlazadas
template <class K, class T>
class HashMapList {
private:
    Lista<HashEntry<K, T>> **tabla;
    unsigned int tamanio;

    static unsigned int hashFunc(K clave);
    unsigned int (*hashFuncP)(K clave);

public:
    explicit HashMapList(unsigned int k);
    HashMapList(unsigned int k, unsigned int (*hashFuncP)(K clave));

~HashMapList();

    void put(K clave, T valor);
    void remove(K clave);
    T get(K clave);

    bool esVacio();
    void print();
    Lista<HashEntry<K, T>>* getListaPorIndice(unsigned int i);
};

template <class K, class T>
HashMapList<K, T>::HashMapList(unsigned int k) {
    tamanio = k;
    tabla = new Lista<HashEntry<K, T>> *[tamanio];
    for(int i = 0; i < tamanio; i++) {
        tabla[i] = nullptr;
    }
    hashFuncP = hashFunc;
}

template <class K, class T>
HashMapList<K, T>::HashMapList(unsigned int k, unsigned int (*fp)(K)) {
    tamanio = k;
    tabla = new Lista<HashEntry<K, T>> *[tamanio];
    for(int i = 0; i < tamanio; i++) {
        tabla[i] = nullptr;
    }
    hashFuncP = fp;
}

template <class K, class T>
HashMapList<K, T>::~HashMapList() {
    for(int i = 0; i < tamanio; i++) {
        if(tabla[i] != nullptr) {
            delete tabla[i];
        }
    }
}

template <class K, class T>
void HashMapList<K, T>::put(K clave, T valor) {
    unsigned int pos = hashFuncP(clave) % tamanio;
    if(tabla[pos] == nullptr) {
        tabla[pos] = new Lista<HashEntry<K, T>>();
    }
    tabla[pos]->insertarUltimo(HashEntry<K, T>(clave, valor));
}

template <class K, class T>
T HashMapList<K, T>::get(K clave) {
    unsigned int pos = hashFuncP(clave) % tamanio;

    if (tabla[pos] == nullptr) throw std::runtime_error("Clave no encontrada");

    Nodo<HashEntry<K, T>>* actual = tabla[pos]->getInicio();
    while (actual != nullptr) {
        if (actual->getDato().getClave() == clave) {
            return actual->getDato().getValor();
        }
        actual = actual->getSiguiente();
    }

    throw std::runtime_error("Clave no encontrada");
}

template <class K, class T>
void HashMapList<K, T>::remove(K clave) {
    unsigned int pos = hashFuncP(clave) % tamanio;
    if (tabla[pos] == nullptr) throw std::runtime_error("Clave no encontrada");

    Nodo<HashEntry<K, T>>* actual = tabla[pos]->getInicio();
    int index = 0;

    while (actual != nullptr) {
        if (actual->getDato().getClave() == clave) {
            tabla[pos]->remover(index);
            if (tabla[pos]->esVacia()) {
                delete tabla[pos];
                tabla[pos] = nullptr;
            }
            return;
        }
        actual = actual->getSiguiente();
        index++;
    }

    throw std::runtime_error("Clave no encontrada");
}

template <class K, class T>
bool HashMapList<K, T>::esVacio() {
    for(int i = 0; i < tamanio; i++) {
        if(tabla[i] != nullptr) {
            return false;
        }
    }
    return true;
}
template <class K, class T>
unsigned int HashMapList<K, T>::hashFunc(K clave) {
    std::hash<K> hasher;
    return (unsigned int)(hasher(clave));
}

template <class K, class T>
void HashMapList<K, T>::print() {
    for (int i = 0; i < tamanio; ++i) {
        if (tabla[i] != nullptr) {
            std::cout << "Indice " << i << ":" << std::endl;
            tabla[i]->imprimir();
        }
    }
}

template <class K, class T>
Lista<HashEntry<K, T>>* HashMapList<K, T>::getListaPorIndice(unsigned int i) {
    if (i >= tamanio) return nullptr;
    return tabla[i];
}

#endif // U05_HASH_HASHMAP_HASHMAPLIST_H_

