#pragma once
#include "Pago.hpp"
#include "PagoEfectivo.hpp"
#include "PagoTarjeta.hpp"
#include "Pedido.hpp"
#include <iostream>
#include <limits>
using namespace std;

class GestorPagos {
private:
    static bool validarNumeroTarjetaRecursivo(const string& numero, int posicion) {
        if (posicion >= numero.length()) {
            return true;
        }
        if (!isdigit(numero[posicion])) {
            return false;
        }
        return validarNumeroTarjetaRecursivo(numero, posicion + 1);
    }

public:
    static bool procesarPagoEfectivo(Pedido* pedido, double montoPagado, string fecha) {
        if (pedido == nullptr) {
            cout << ">>> ERROR: Pedido invalido" << endl;
            system("pause");
            return false;
        }
        double total = pedido->calcularTotal();
        PagoEfectivo pago(total, fecha, montoPagado);
        cout << "\n>>> Procesando pago del pedido #" << pedido->getNumeroPedido() << endl;
        string resultado = pago.procesarPago();
        cout << ">>> " << resultado << endl;
        if (montoPagado >= total) {
            pago.mostrarDetalle();
            double cambio = pago.getCambio();
            if (cambio > 0) {
                cout << "\n" << string(50, '*') << endl;
                cout << "***  SU CAMBIO: S/" << fixed << setprecision(2)
                    << cambio << string(50 - 21 - to_string((int)cambio).length(), ' ') << "***" << endl;
                cout << "***  Entregue el cambio al cliente" << string(15, ' ') << "***" << endl;
                cout << string(50, '*') << endl;
            }
            else {
                cout << "\n>>> Pago exacto. No hay cambio." << endl;
            }
            cout << "\n";
            system("pause");
            pedido->cambiarEstado("pagado");
            return true;
        }
        system("pause");
        return false;
    }

    static bool procesarPagoTarjeta(Pedido* pedido, string numeroTarjeta,
        string tipoTarjeta, string banco, string fecha) {
        if (pedido == nullptr) {
            cout << ">>> ERROR: Pedido invalido" << endl;
            system("pause");
            return false;
        }
        double total = pedido->calcularTotal();
        PagoTarjeta pago(total, fecha, numeroTarjeta, tipoTarjeta, banco);
        cout << "\n>>> Procesando pago del pedido #" << pedido->getNumeroPedido() << endl;
        string resultado = pago.procesarPago();
        cout << ">>> " << resultado << endl;
        if (numeroTarjeta.length() >= 16) {
            pago.mostrarDetalle();
            cout << "\n";
            system("pause");
            pedido->cambiarEstado("pagado");
            return true;
        }
        system("pause");
        return false;
    }

    static void mostrarResumenPago(Pedido* pedido) {
        if (pedido == nullptr) return;
        cout << "\n" << string(50, '=') << endl;
        cout << "        RESUMEN DE PAGO" << endl;
        cout << string(50, '=') << endl;
        cout << "Pedido #" << pedido->getNumeroPedido() << endl;
        cout << "Cliente: " << pedido->getNombreCliente() << endl;
        cout << "Tipo: " << pedido->getTipoServicio() << endl;
        cout << string(50, '-') << endl;
        pedido->mostrarProductos();
        cout << string(50, '-') << endl;
        cout << "TOTAL A PAGAR:  S/" << fixed << setprecision(2)
            << setw(10) << right << pedido->calcularTotal() << endl;
        cout << string(50, '=') << endl;
    }

    static void mostrarMenuPago() {
        cout << "\n" << string(40, '=') << endl;
        cout << "     METODOS DE PAGO DISPONIBLES" << endl;
        cout << string(40, '=') << endl;
        cout << "1. Pago en Efectivo" << endl;
        cout << "2. Pago con Tarjeta de Credito" << endl;
        cout << "3. Pago con Tarjeta de Debito" << endl;
        cout << "0. Cancelar" << endl;
        cout << string(40, '=') << endl;
        cout << "Seleccione una opcion: ";
    }

    static bool validarNumeroTarjeta(const string& numero) {
        if (numero.length() < 16) return false;

        return validarNumeroTarjetaRecursivo(numero, 0);
    }
};