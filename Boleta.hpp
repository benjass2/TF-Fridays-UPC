#pragma once
#include "Comprobante.hpp"
#include "Cola.hpp"
#include "Producto.hpp"
#include "Funcionalidades.hpp"
using namespace ConsolaColor;

class Boleta : public Comprobante {
private:
    string dni;
    Cola<Producto*>* productos;

    void mostrarProductosRecursivo(int posicion) const {
        if (posicion >= productos->getTamaño()) {
            return;
        }

        Producto* p = productos->obtenerEnPosicion(posicion);
        string nombre = p->getNombre();
        if (nombre.length() > 35) {
            nombre = nombre.substr(0, 32) + "...";
        }
        cout << "" << setw(3) << right << (posicion + 1) << ". "
            << setw(38) << left << nombre
            << "   S/" << setw(8) << right << fixed << setprecision(2)
            << p->getPrecio() << " " << endl;

        mostrarProductosRecursivo(posicion + 1);

    }

public:
    Boleta(string fecha, double total, string nombreCliente, string dni,
        Cola<Producto*>* productos = nullptr)
        : Comprobante(fecha, total, nombreCliente, dni),
        dni(dni), productos(productos) {
    }

    ~Boleta() {}

    string getTipoComprobante() const override {
        return "BOLETA DE VENTA ELECTRONICA";
    }

    void mostrarComprobante() const override {
        mostrarEncabezado();

        cout << " " << getTipoComprobante() << "                  " << endl;
        cout << " N°: B001-" << setw(8) << setfill('0') << numeroComprobante
            << setfill(' ') << "                                 " << endl;
        cout << " Fecha: " << setw(47) << left << fecha << "" << endl;
        cout << "                                                  " << endl;

        cout << " CLIENTE:                                               " << endl;
        cout << " Nombre: " << setw(46) << left << nombreCliente << "" << endl;
        cout << " DNI: " << setw(49) << left << dni << "" << endl;
        cout << "                                                         " << endl;

        cout << " DETALLE DE PRODUCTOS:                                 " << endl;
        cout << "                                                        " << endl;

        if (productos != nullptr && !productos->estaVacia()) {
            mostrarProductosRecursivo(0);
        }

        mostrarTotales();
        cout << endl;
        cout << "                ¡Gracias por su compra!       " << endl;
        /* cout << "               Conserve este comprobante          " << endl;*/
        cout << endl;
    }

    string getDni() const { return dni; }
};