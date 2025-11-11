#pragma once
#include "Funcionalidades.hpp"
using namespace std;

class Mesa {
private:
    int numeroMesa;
    int capacidad;
    string ubicacion;      // "Interior", "Terraza", "VIP"
    bool disponible;
    string tipoMesa;       // "Regular", "Familiar", "VIP"

public:
    Mesa()
        : numeroMesa(0), capacidad(0), ubicacion(""),
        disponible(true), tipoMesa("Regular") {
    }


    Mesa(int numero, int cap, string ubic, string tipo = "Regular")
        : numeroMesa(numero), capacidad(cap), ubicacion(ubic),
        disponible(true), tipoMesa(tipo) {
    }


    ~Mesa() {}



    int getNumeroMesa() const { return numeroMesa; }
    int getCapacidad() const { return capacidad; }
    string getUbicacion() const { return ubicacion; }
    bool isDisponible() const { return disponible; }
    string getTipoMesa() const { return tipoMesa; }



    void setNumeroMesa(int numero) { numeroMesa = numero; }
    void setCapacidad(int cap) { capacidad = cap; }
    void setUbicacion(const string& ubic) { ubicacion = ubic; }
    void setDisponible(bool disp) { disponible = disp; }
    void setTipoMesa(const string& tipo) { tipoMesa = tipo; }



    // Ocupar la mesa
    void ocupar() {
        disponible = false;
    }

    // Liberar la mesa
    void liberar() {
        disponible = true;
    }



    // Mostrar información completa
    void mostrarInfo() const {

        cout << "  INFORMACIÓN DE LA MESA            " << endl;
        cout << endl;
        cout << " Mesa N: " << numeroMesa << endl;
        cout << " Capacidad: " << capacidad << " personas" << endl;
        cout << " Ubicación: " << ubicacion << endl;
        cout << " Tipo: " << tipoMesa << endl;
        cout << " Estado: " << (disponible ? " Disponible" : " Ocupada") << endl;

    }

    // Mostrar resumen (para listados)
    void mostrarResumen() const {
        cout << left << setw(6) << numeroMesa
            << setw(12) << capacidad
            << setw(12) << ubicacion
            << setw(10) << tipoMesa
            << (disponible ? "Disponible" : "Ocupada") << endl;
    }
    //Operadores de comparacion para arboles

    // Comparar por número de mesa
    bool operator<(const Mesa& otra) const {
        return numeroMesa < otra.numeroMesa;
    }

    bool operator>(const Mesa& otra) const {
        return numeroMesa > otra.numeroMesa;
    }

    bool operator==(const Mesa& otra) const {
        return numeroMesa == otra.numeroMesa;
    }

    bool operator!=(const Mesa& otra) const {
        return numeroMesa != otra.numeroMesa;
    }

    bool operator<=(const Mesa& otra) const {
        return numeroMesa <= otra.numeroMesa;
    }

    bool operator>=(const Mesa& otra) const {
        return numeroMesa >= otra.numeroMesa;
    }


    friend ostream& operator<<(ostream& os, const Mesa& mesa) {
        os << "Mesa " << mesa.numeroMesa
            << " [" << mesa.capacidad << " pers.] "
            << mesa.ubicacion << " - " << mesa.tipoMesa
            << " (" << (mesa.disponible ? "Disponible" : "Ocupada") << ")";
        return os;
    }



    // Verificar si tiene capacidad para X personas
    bool tieneCapacidadPara(int personas) const {
        return personas <= capacidad;
    }

    // Obtener estado como string
    string getEstadoTexto() const {
        return disponible ? "Disponible" : "Ocupada";
    }
};