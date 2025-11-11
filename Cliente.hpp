#pragma once
#include "Funcionalidades.hpp"

class Cliente {
private:
    int id;
    string nombre;
    string apellido;
    string telefono;
    string dni;
    string direccion;

public:

    Cliente() : id(0), nombre(""), apellido(""), telefono(""), dni(""), direccion("") {}


    Cliente(int id, string nombre, string apellido, string telefono, string dni, string direccion = "")
        : id(id), nombre(nombre), apellido(apellido), telefono(telefono), dni(dni), direccion(direccion) {
    }


    ~Cliente() {}


    int getId() const { return id; }
    string getNombre() const { return nombre; }
    string getApellido() const { return apellido; }
    string getNombreCompleto() const { return nombre + " " + apellido; }
    string getTelefono() const { return telefono; }
    string getDni() const { return dni; }
    string getDireccion() const { return direccion; }


    void setId(int id) { this->id = id; }
    void setNombre(const string& nombre) { this->nombre = nombre; }
    void setApellido(const string& apellido) { this->apellido = apellido; }
    void setTelefono(const string& telefono) { this->telefono = telefono; }
    void setDni(const string& dni) { this->dni = dni; }
    void setDireccion(const string& direccion) { this->direccion = direccion; }

    // Mostrar información del cliente
    void mostrarInfo() const {

        cout << "DATOS DEL CLIENTE" << endl;

        cout << "ID: " << id << endl;
        cout << "Nombre: " << getNombreCompleto() << endl;
        cout << "DNI: " << dni << endl;
        cout << "Telefono: " << telefono << endl;
        if (!direccion.empty()) {
            cout << "Direccion: " << direccion << endl;
        }

    }


    bool datosCompletos() const {
        return !nombre.empty() && !apellido.empty() && !telefono.empty() && !dni.empty();
    }


    bool operator==(const Cliente& otro) const {
        return dni == otro.dni;
    }
};