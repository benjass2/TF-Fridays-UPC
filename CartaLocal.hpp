#pragma once
#include "Carta.hpp"
#include <string>

using namespace std;

class CartaLocal : public Carta {
private:
    int mesasDisponibles;
    string horarioAtencion;

public:
    // Constructor
    CartaLocal() : Carta("local", "Menu para Salon", "Data/menu.txt") {
        mesasDisponibles = 20;
        horarioAtencion = "11:00 - 23:00";
    }


    ~CartaLocal() {}



    // Getters específicos
    string getHorarioAtencion() const { return horarioAtencion; }


    void setHorarioAtencion(const string& horario) {
        horarioAtencion = horario;
    }
};
