#pragma once
#include "Pago.hpp"
#include "Funcionalidades.hpp"
using namespace ConsolaColor;
using namespace UtilidadesConsola;

class PagoEfectivo : public Pago {
private:
    double montoPagado;
    double cambio;
public:
    PagoEfectivo(double monto = 0.0, string fecha = "", double montoPagado = 0.0)
        : Pago(monto, fecha), montoPagado(montoPagado) {
        cambio = montoPagado - monto;
    }

    string procesarPago() override {
        if (montoPagado >= monto) {
            return "Pago en efectivo procesado exitosamente";
        }
        else {
            return "ERROR: Monto insuficiente. Faltan S/" +
                to_string(monto - montoPagado);
        }
    }

    string getTipoPago() const override {
        return "Efectivo";
    }

    void mostrarDetalle() const override {
        /*
        cout << "        COMPROBANTE DE PAGO - EFECTIVO" << endl;

        cout << "Fecha: " << fecha << endl;

        cout << "Monto total:    S/" << fixed << setprecision(2)
            << setw(10) << right << monto << endl;*/
        imprimirSeparadorRojoBlanco(20);
        cout << "Monto pagado:   S/" << setw(10) << right << montoPagado << endl;
        cout << "Cambio:         S/" << setw(10) << right << cambio << endl;
        cout << endl;
        cout << "Gracias por su compra!" << endl;


    }

    double getCambio() const { return cambio; }
    double getMontoPagado() const { return montoPagado; }


};