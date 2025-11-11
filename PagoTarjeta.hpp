#pragma once
#include "Pago.hpp"
#include <string>
#include <iostream>
using namespace std;

class PagoTarjeta : public Pago {
private:
    string numeroTarjeta;
    string tipoTarjeta;
    string banco;
    string codigoAutorizacion;
public:
    PagoTarjeta(double monto = 0.0, string fecha = "", string numeroTarjeta = "",
        string tipoTarjeta = "Credito", string banco = "")
        : Pago(monto, fecha), numeroTarjeta(numeroTarjeta),
        tipoTarjeta(tipoTarjeta), banco(banco) {
        codigoAutorizacion = "AUTH" + to_string(rand() % 90000 + 10000);
    }

    string procesarPago() override {
        if (numeroTarjeta.length() >= 16) {
            return "Pago autorizado - Codigo: " + codigoAutorizacion;
        }
        else {
            return "ERROR: Numero de tarjeta invalido";
        }
    }

    string getTipoPago() const override {
        return "Tarjeta de " + tipoTarjeta;
    }

    void mostrarDetalle() const override {
        cout << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << "Fecha: " << fecha << endl;
        cout << "Tipo: " << tipoTarjeta << endl;
        cout << "Banco: " << banco << endl;
        cout << "Tarjeta: **** **** **** "
            << numeroTarjeta.substr(numeroTarjeta.length() - 4) << endl;

        cout << "Monto total:    S/" << fixed << setprecision(2)
            << setw(10) << right << monto << endl;
        cout << "Codigo Auth:    " << codigoAutorizacion << endl;
        cout << endl;

    }

    string getNumeroTarjeta() const { return numeroTarjeta; }
    string getTipoTarjeta() const { return tipoTarjeta; }
    string getBanco() const { return banco; }
    string getCodigoAutorizacion() const { return codigoAutorizacion; }

    void setNumeroTarjeta(const string& numero) { numeroTarjeta = numero; }
    void setTipoTarjeta(const string& tipo) { tipoTarjeta = tipo; }
    void setBanco(const string& b) { banco = b; }
};
