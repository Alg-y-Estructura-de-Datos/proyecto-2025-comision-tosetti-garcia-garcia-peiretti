#ifndef HASHENTRY_H
#define HASHENTRY_H

template <class K, class T>
class HashEntry {
private:
    K clave;
    T valor;

public:
    HashEntry(K clave, T valor) : clave(clave), valor(valor) {}

    K getClave() const {
        return clave;
    }

    T getValor() const {
        return valor;
    }

    bool operator==(const HashEntry<K, T>& otro) const {
        return clave == otro.clave;
    }
};

#endif

