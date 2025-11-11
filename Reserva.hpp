#pragma once
#include "Funcionalidades.hpp"
#include "UtilidadesConsola.hpp"
#include "Cliente.hpp"
#include "Mesa.hpp"
using namespace UtilidadesConsola;

class Reserva {
private:
    string codigoReserva;
    Cliente cliente;
    Mesa mesa;
    string fecha;
    string hora;
    int numeroPersonas;
    string estado;
    string observaciones;

    static int contadorReservas;


    string generarCodigoReserva() {
        contadorReservas++;
        string codigo = "RES";


        if (contadorReservas < 10) codigo += "00";
        else if (contadorReservas < 100) codigo += "0";

        codigo += to_string(contadorReservas);
        return codigo;
    }

public:


    Reserva() : codigoReserva(""), numeroPersonas(0), estado("Pendiente"), observaciones("") {
    }

    // Constructor con parámetros (genera código automático)
    Reserva(const Cliente& cli, const Mesa& m, string f, string h, int numPers, string obs = "")
        : cliente(cli), mesa(m), fecha(f), hora(h),
        numeroPersonas(numPers), estado("Pendiente"), observaciones(obs) {
        codigoReserva = generarCodigoReserva();
    }

    // Constructor con código específico (para cargar desde archivo)
    Reserva(string codigo, const Cliente& cli, const Mesa& m, string f, string h, int numPers, string est, string obs = "")
        : codigoReserva(codigo), cliente(cli), mesa(m), fecha(f), hora(h),
        numeroPersonas(numPers), estado(est), observaciones(obs) {
    }


    ~Reserva() {}


    //getters
    string getCodigoReserva() const { return codigoReserva; }
    Cliente getCliente() const { return cliente; }
    Mesa getMesa() const { return mesa; }
    string getFecha() const { return fecha; }
    string getHora() const { return hora; }
    int getNumeroPersonas() const { return numeroPersonas; }
    string getEstado() const { return estado; }
    string getObservaciones() const { return observaciones; }

    //setters
    void setCodigoReserva(const string& codigo) { codigoReserva = codigo; }
    void setCliente(const Cliente& cli) { cliente = cli; }
    void setMesa(const Mesa& m) { mesa = m; }
    void setFecha(const string& f) { fecha = f; }
    void setHora(const string& h) { hora = h; }
    void setNumeroPersonas(int numPers) { numeroPersonas = numPers; }
    void setEstado(const string& est) { estado = est; }
    void setObservaciones(const string& obs) { observaciones = obs; }


    //Gestion de estado
    void confirmar() { estado = "Confirmada"; }
    void cancelar() { estado = "Cancelada"; }
    void completar() { estado = "Completada"; }
    void marcarPendiente() { estado = "Pendiente"; }

    bool estaPendiente() const { return estado == "Pendiente"; }
    bool estaConfirmada() const { return estado == "Confirmada"; }
    bool estaCancelada() const { return estado == "Cancelada"; }
    bool estaCompletada() const { return estado == "Completada"; }



    bool esValida() const {
        return !codigoReserva.empty() &&
            numeroPersonas > 0 &&
            numeroPersonas <= mesa.getCapacidad() &&
            !fecha.empty() &&
            !hora.empty() &&
            cliente.datosCompletos();
    }

    //Mostrar informacion completa de reserva
    void mostrarInfo() const {

        cout << ROJO << "INFORMACIÓN DE LA RESERVA" << RESET << endl;
        imprimirSeparadorRojoBlanco(30);

        cout << " Código: " << codigoReserva << endl;
        cout << " Estado: ";

        // Mostrar estado con indicador visual
        if (estado == "Confirmada") cout << " Confirmada";
        else if (estado == "Cancelada") cout << " Cancelada";
        else if (estado == "Completada") cout << " Completada";
        else cout << " Pendiente";

        cout << endl;

        cout << " DATOS DEL CLIENTE:" << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << " Nombre: " << cliente.getNombreCompleto() << endl;
        cout << " DNI: " << cliente.getDni() << endl;
        cout << " Teléfono: " << cliente.getTelefono() << endl;

        cout << " DETALLES DE LA RESERVA:" << endl;
        cout << " Fecha: " << fecha << endl;
        cout << " Hora: " << hora << endl;
        cout << " Personas: " << numeroPersonas << endl;
        cout << " Mesa: N " << mesa.getNumeroMesa() << " (" << mesa.getUbicacion() << " - " << mesa.getTipoMesa() << ")" << endl;

        if (!observaciones.empty()) {

            cout << " OBSERVACIONES:" << endl;
            cout << " " << observaciones << endl;
        }


    }

    // Mostrar resumen 
    void mostrarResumen() const {
        cout << left << setw(10) << codigoReserva
            << setw(25) << cliente.getNombreCompleto()
            << setw(12) << fecha
            << setw(8) << hora
            << setw(8) << ("Mesa " + to_string(mesa.getNumeroMesa()))
            << setw(6) << numeroPersonas
            << setw(12) << estado << endl;
    }

    //operadores para comparacion en arboles

    // Comparar por fecha y hora (para ordenar cronológicamente)
    bool operator<(const Reserva& otra) const {
        // Primero comparar por fecha
        if (fecha != otra.fecha) {
            return compararFechas(fecha, otra.fecha) < 0;
        }
        // Si las fechas son iguales, comparar por hora
        return hora < otra.hora;
    }

    bool operator>(const Reserva& otra) const {
        if (fecha != otra.fecha) {
            return compararFechas(fecha, otra.fecha) > 0;
        }
        return hora > otra.hora;
    }

    bool operator==(const Reserva& otra) const {
        return codigoReserva == otra.codigoReserva;
    }

    bool operator!=(const Reserva& otra) const {
        return codigoReserva != otra.codigoReserva;
    }

    bool operator<=(const Reserva& otra) const {
        return (*this < otra) || (*this == otra);
    }

    bool operator>=(const Reserva& otra) const {
        return (*this > otra) || (*this == otra);
    }



    // Comparar fechas en formato DD/MM/YYYY

    static int compararFechas(const string& f1, const string& f2) {
        // Extraer día, mes, año de f1
        int dia1 = stoi(f1.substr(0, 2));
        int mes1 = stoi(f1.substr(3, 2));
        int anio1 = stoi(f1.substr(6, 4));

        // Extraer día, mes, año de f2
        int dia2 = stoi(f2.substr(0, 2));
        int mes2 = stoi(f2.substr(3, 2));
        int anio2 = stoi(f2.substr(6, 4));

        // Comparar año
        if (anio1 != anio2) return (anio1 < anio2) ? -1 : 1;

        // Comparar mes
        if (mes1 != mes2) return (mes1 < mes2) ? -1 : 1;

        // Comparar día
        if (dia1 != dia2) return (dia1 < dia2) ? -1 : 1;

        return 0; // Son iguales
    }



    friend ostream& operator<<(ostream& os, const Reserva& reserva) {
        os << reserva.codigoReserva << " - " << reserva.cliente.getNombreCompleto() << " - " << reserva.fecha << " " << reserva.hora << " - " << "Mesa " << reserva.mesa.getNumeroMesa() << " - " << reserva.estado;
        return os;
    }

    static void resetearContador() {
        contadorReservas = 0;
    }

    static int getContadorActual() {
        return contadorReservas;
    }
};

int Reserva::contadorReservas = 0;