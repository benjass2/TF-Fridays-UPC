#pragma once
#include "Producto.hpp"
using namespace std;

class Bebida : public Producto {
public:
    Bebida() : Producto() {
        categoria = "bebida";
    }


    Bebida(int _id, string _nombre, double _precio)
        : Producto(_id, _nombre, _precio, "bebida") {
    }


    ~Bebida() {}


    void mostrarDetalle() const override {
        cout << string(50, '=') << endl;
        cout << "BEBIDA - Detalles" << endl;
        cout << string(50, '=') << endl;
        cout << "ID: " << getId() << endl;
        cout << "Nombre: " << getNombre() << endl;
        cout << "Precio: S/" << fixed << setprecision(2) << getPrecio() << endl;
        cout << "Categoria: " << getCategoria() << endl;
        cout << "Estado: " << (isDisponible() ? "Disponible" : "No disponible") << endl;
        cout << string(50, '=') << endl;
    }
};

