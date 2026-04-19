#ifndef LISTA_H
#define LISTA_H

#include <cstddef>  // size_t
#include "Nodo.h"

template <class T>
class Lista {
private:
    Nodo<T>* cabeza;
    int n;

    static size_t memoriaTotal;

    Nodo<T>* nodoEn(int i) const {
        Nodo<T>* actual = cabeza;
        for (int k = 0; k < i; ++k) {
            actual = actual->getSiguiente();
        }
        return actual;
    }

public:
    Lista() : cabeza(nullptr), n(0) {}
    ~Lista() {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            Nodo<T>* tmp = actual;
            actual = actual->getSiguiente();
            delete tmp;
            memoriaTotal -= sizeof(Nodo<T>);
        }
        cabeza = nullptr;
        n = 0;
    }

    int tamano() const {
        return n;
    }

    bool esVacia() const {
        return n == 0;
    }

    T primero() const {
        return cabeza->getDato();
    }

    T ultimo() const {
        Nodo<T>* actual = cabeza;
        while (actual->getSiguiente() != nullptr) {
            actual = actual->getSiguiente();
        }
        return actual->getDato();
    }

    void agregar(const T& e, int i) {
        if (i < 0 || i > n) return;

        Nodo<T>* nuevo = new Nodo<T>(e);
        memoriaTotal += sizeof(Nodo<T>);

        if (i == 0) {
            nuevo->setSiguiente(cabeza);
            cabeza = nuevo;
        } else {
            Nodo<T>* prev = nodoEn(i - 1);
            nuevo->setSiguiente(prev->getSiguiente());
            prev->setSiguiente(nuevo);
        }
        n++;
    }

    void eliminar(const T& e) {
        if (esVacia()) return;

        if (cabeza->getDato() == e) {
            Nodo<T>* tmp = cabeza;
            cabeza = cabeza->getSiguiente();
            delete tmp;
            memoriaTotal -= sizeof(Nodo<T>);
            n--;
            return;
        }

        Nodo<T>* actual = cabeza;
        while (actual->getSiguiente() != nullptr) {
            if (actual->getSiguiente()->getDato() == e) {
                Nodo<T>* tmp = actual->getSiguiente();
                actual->setSiguiente(tmp->getSiguiente());
                delete tmp;
                memoriaTotal -= sizeof(Nodo<T>);
                n--;
                return;
            }
            actual = actual->getSiguiente();
        }
    }

    T consultar(int i) const {
        if (i < 0 || i >= n) {
            return T();
        }
        return nodoEn(i)->getDato();
    }

    void reemplazar(const T& e, int i) {
        if (i < 0 || i >= n) return;
        nodoEn(i)->setDato(e);
    }

    void concatenar(const Lista<T>& B) {
        Nodo<T>* actualB = B.cabeza;

        while (actualB != nullptr) {
            agregar(actualB->getDato(), n);
            actualB = actualB->getSiguiente();
        }
    }

    static size_t memoryUsage() {
        return memoriaTotal;
    }
};

template <class T>
size_t Lista<T>::memoriaTotal = 0;

#endif //LISTA_H
