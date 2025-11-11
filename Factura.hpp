#pragma once
#include "Comprobante.hpp"
#include "Cola.hpp"
#include "Producto.hpp"

class Factura : public Comprobante {
private:
    string ruc;
    string razonSocial;
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
        cout << " " << setw(3) << right << (posicion + 1) << ". "
            << setw(38) << left << nombre
            << " S/" << setw(8) << right << fixed << setprecision(2)
            << p->getPrecio() << " " << endl;

        mostrarProductosRecursivo(posicion + 1);
    }

public:
    Factura(string fecha, double total, string razonSocial, string ruc,
        Cola<Producto*>* productos = nullptr)
        : Comprobante(fecha, total, razonSocial, ruc),
        ruc(ruc), razonSocial(razonSocial), productos(productos) {
    }

    ~Factura() {}

    string getTipoComprobante() const override {
        return "FACTURA ELECTRONICA";
    }

    void mostrarComprobante() const override {
        system("cls");
        mostrarEncabezado();

        cout << " " << getTipoComprobante() << "                       " << endl;
        cout << " N°: F001-" << setw(8) << setfill('0') << numeroComprobante
            << setfill(' ') << "                                 " << endl;
        cout << " Fecha: " << setw(47) << left << fecha << " " << endl;
        cout << "                                                     " << endl;

        cout << " DATOS DEL CLIENTE:                                    " << endl;
        cout << " Razon Social: " << setw(40) << left << razonSocial << "" << endl;
        cout << " RUC: " << setw(49) << left << ruc << "" << endl;
        cout << "                                                        " << endl;

        cout << " DETALLE DE PRODUCTOS:                                 " << endl;
        cout << "                                                       " << endl;

        if (productos != nullptr && !productos->estaVacia()) {
            mostrarProductosRecursivo(0);
        }

        mostrarTotales();
    }

    string getRuc() const { return ruc; }
    string getRazonSocial() const { return razonSocial; }
};