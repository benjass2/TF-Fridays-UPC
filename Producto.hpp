#pragma once
#include <iostream>
#include <iomanip>
using namespace std;

class Producto {
protected:
    int id;
    string nombre;
    double precio;
    string categoria;
    bool disponible;

public:

    Producto() : id(0), nombre(""), precio(0.0), categoria(""), disponible(true) {}


    Producto(int _id, string _nombre, double _precio)
        : id(_id), nombre(_nombre), precio(_precio), categoria("general"), disponible(true) {
    }


    Producto(int _id, string _nombre, double _precio, string _categoria, bool _disponible = true)
        : id(_id), nombre(_nombre), precio(_precio), categoria(_categoria), disponible(_disponible) {
    }


    int getId() const { return id; }
    string getNombre() const { return nombre; }
    double getPrecio() const { return precio; }
    string getCategoria() const { return categoria; }
    bool isDisponible() const { return disponible; }

    void setId(int _id) { this->id = _id; }
    void setNombre(const string& _nombre) { this->nombre = _nombre; }
    void setPrecio(double _precio) { this->precio = _precio; }
    void setCategoria(const string& _categoria) { this->categoria = _categoria; }
    void setDisponible(bool _disponible) { this->disponible = _disponible; }


    bool operator==(const Producto& otro) const {
        return id == otro.id;
    }

    bool operator<(const Producto& otro) const {
        return precio < otro.precio;
    }

    virtual void mostrarDetalle() const {
        cout << setfill('-') << setw(50) << "" << endl;
        cout << "ID: " << id << endl;
        cout << "Nombre: " << nombre << endl;
        cout << "Precio: S/" << fixed << setprecision(2) << precio << endl;
        cout << "Categoría: " << categoria << endl;
        cout << "Estado: " << (disponible ? "Disponible" : "No disponible") << endl;
        cout << setfill('-') << setw(50) << "" << endl;
    }

    void aplicarDescuento(double porcentaje) {
        if (porcentaje > 0 && porcentaje <= 100) {
            precio = precio * (1.0 - porcentaje / 100.0);
            cout << "Descuento del " << porcentaje << "% aplicado a " << nombre << endl;
        }
    }

    // Operador de salida
    friend ostream& operator<<(ostream& os, const Producto& p) {
        os << left << setw(4) << p.id
            << setw(35) << p.nombre
            << "S/" << right << setw(6) << fixed << setprecision(2) << p.precio
            << left << setw(12) << ("    [" + p.categoria + "]")
            << (p.disponible ? "      V" : "      X");
        return os;
    }


};