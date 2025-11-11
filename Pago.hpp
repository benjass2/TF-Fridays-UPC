#pragma once
#include <string>
#include <iostream>
using namespace std;

class Pago {
protected:
    double monto;
    string fecha;
public:
    Pago(double monto = 0.0, string fecha = "") : monto(monto), fecha(fecha) {}

    virtual ~Pago() {}

    virtual string procesarPago() = 0;
    virtual string getTipoPago() const = 0;
    virtual void mostrarDetalle() const = 0;

    double getMonto() const { return monto; }
    string getFecha() const { return fecha; }

    void mostrarMonto() const {
        cout << "Total a pagar: S/" << monto << endl;
    }
};
