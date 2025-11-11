#pragma once
#include "Funcionalidades.hpp"

class Comprobante {
protected:
    static int contadorComprobantes;
    int numeroComprobante;
    string fecha;
    double subtotal;
    double igv;
    double total;
    string nombreCliente;
    string documentoCliente;

public:
    Comprobante(string fecha = "", double total = 0.0,
        string nombreCliente = "", string documentoCliente = "")
        : fecha(fecha), nombreCliente(nombreCliente),
        documentoCliente(documentoCliente) {

        numeroComprobante = ++contadorComprobantes;

        this->subtotal = total / 1.18;
        this->igv = total - subtotal;
        this->total = total;
    }


    virtual ~Comprobante() {}


    virtual void mostrarComprobante() const = 0;
    virtual string getTipoComprobante() const = 0;


    int getNumeroComprobante() const { return numeroComprobante; }
    string getFecha() const { return fecha; }
    double getTotal() const { return total; }
    double getSubtotal() const { return subtotal; }
    double getIgv() const { return igv; }
    string getNombreCliente() const { return nombreCliente; }
    string getDocumentoCliente() const { return documentoCliente; }


    void mostrarEncabezado() const {
        cout << "\n";
		imprimirSeparadorRojoBlanco(29);

        cout << "                   RESTAURANTE FRIDAYS                           " << endl;
        cout << "                    RUC: 20123456789                              " << endl;
        cout << "                 Av. Principal 123, Lima                         " << endl;
        cout << "                      Tel: 555-1234                                 " << endl;
        imprimirSeparadorRojoBlanco(29);

    }


    void mostrarTotales() const {
        imprimirSeparadorRojoBlanco(29);
        cout << " Subtotal:        S/" << setw(36) << right << fixed
            << setprecision(2) << subtotal << endl;
        cout << " IGV (18%):       S/" << setw(36) << right << igv << endl;
        cout << " TOTAL:           S/" << setw(36) << right << total << endl;
        imprimirSeparadorRojoBlanco(29);
    }

};


int Comprobante::contadorComprobantes = 0;