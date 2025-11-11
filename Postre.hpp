#pragma once
#include "Producto.hpp"
using namespace std;

class Postre : public Producto {
public:
    Postre() : Producto() {
        categoria = "postre";
    }

    Postre(int _id, string _nombre, double _precio)
        : Producto(_id, _nombre, _precio, "postre") {
    }

    // Destructor
    ~Postre() {}


    void mostrarDetalle() const override {
        cout << string(50, '=') << endl;
        cout << "POSTRE - Detalles" << endl;
        cout << string(50, '=') << endl;
        cout << "ID: " << getId() << endl;
        cout << "Nombre: " << getNombre() << endl;
        cout << "Precio: S/" << fixed << setprecision(2) << getPrecio() << endl;
        cout << "Categoria: " << getCategoria() << endl;
        cout << "Estado: " << (isDisponible() ? "Disponible" : "No disponible") << endl;
        cout << string(50, '=') << endl;
    }
};

