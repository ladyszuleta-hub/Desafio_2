#ifndef NODO_H
#define NODO_H

template <class T>
class Nodo {
private:
    T dato;
    Nodo<T>* siguiente;

public:

    Nodo() {
        siguiente = nullptr;
    }

    Nodo(T dato) {
        this->dato = dato;
        this->siguiente = nullptr;
    }

    Nodo(T dato, Nodo<T>* siguiente) {
        this->dato = dato;
        this->siguiente = siguiente;
    }
    ~Nodo() {}


    T getDato() {
        return dato;
    }

    void setDato(T dato) {
        this->dato = dato;
    }

    Nodo<T>* getSiguiente() {
        return siguiente;
    }
    void setSiguiente(Nodo<T>* siguiente) {
        this->siguiente = siguiente;
    }
};

#endif //NODO
