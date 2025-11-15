#pragma once

template<typename T>
class Nodo {
private:
    T data;
    Nodo<T>* siguiente;
    Nodo<T>* anterior;

public:
    Nodo() : siguiente(nullptr), anterior(nullptr) {}
    Nodo(T valor) : data(valor), siguiente(nullptr), anterior(nullptr) {}
    Nodo(T valor, Nodo<T>* sig, Nodo<T>* ant = nullptr)
        : data(valor), siguiente(sig), anterior(ant) {
    }

    ~Nodo() {}

    T getData() const { return data; }
    T& getDataRef() { return data; }  // Retorna referencia para modificar
    Nodo<T>* getSiguiente() const { return siguiente; }
    Nodo<T>* getAnterior() const { return anterior; }

    void setData(T valor) { data = valor; }
    void setSiguiente(Nodo<T>* sig) { siguiente = sig; }
    void setAnterior(Nodo<T>* ant) { anterior = ant; }

    bool operator==(const Nodo<T>& otro) const {
        return data == otro.data;
    }

    bool tieneSiguiente() const { return siguiente != nullptr; }
    bool tieneAnterior() const { return anterior != nullptr; }
};

