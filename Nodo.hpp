#pragma once

template<typename T>
class Nodo {
private:
    T dato;
    Nodo<T>* siguiente;
    Nodo<T>* anterior;

public:
    Nodo() : siguiente(nullptr), anterior(nullptr) {}
    Nodo(T valor) : dato(valor), siguiente(nullptr), anterior(nullptr) {}
    Nodo(T valor, Nodo<T>* sig, Nodo<T>* ant = nullptr)
        : dato(valor), siguiente(sig), anterior(ant) {
    }

    ~Nodo() {}

    T getData() const { return dato; }
    T& getDataRef() { return dato; }
    Nodo<T>* getSiguiente() const { return siguiente; }
    Nodo<T>* getAnterior() const { return anterior; }

    void setData(T valor) { dato = valor; }
    void setSiguiente(Nodo<T>* sig) { siguiente = sig; }
    void setAnterior(Nodo<T>* ant) { anterior = ant; }

    bool operator==(const Nodo<T>& otro) const {
        return dato == otro.dato;
    }

    bool tieneSiguiente() const { return siguiente != nullptr; }
    bool tieneAnterior() const { return anterior != nullptr; }
};

