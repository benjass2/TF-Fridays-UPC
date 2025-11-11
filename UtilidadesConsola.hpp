#pragma once
#include "Funcionalidades.hpp"
#include "iostream"
using namespace std;
using namespace ConsolaColor;

namespace UtilidadesConsola {


    void imprimirSeparadorRojoBlanco(int longitud_pares) {

        std::cout << NEGRITA;

        for (int i = 0; i < longitud_pares; ++i) {
            std::cout << ROJO << "-" << BLANCO << "-";
        }

        std::cout << RESET << std::endl;
    }


    void mostrarLogoPrincipal() {
        cout << ConsolaColor::ROJO << ConsolaColor::ROJO_BRILLANTE;

        cout << endl;
        cout << ROJO << "                                /$$$$$$$$        /$$       /$$                              " << endl;
        cout << BLANCO << "                               | $$_____/       |__/      | $$                              " << endl;
        cout << ROJO << "                               | $$     /$$$$$$  /$$  /$$$$$$$  /$$$$$$  /$$   /$$  /$$$$$$$" << endl;
        cout << BLANCO << "                               | $$$$$ /$$__  $$| $$ /$$__  $$ |____  $$| $$  | $$ /$$_____/" << endl;
        cout << ROJO << "                               | $$__/| $$  \\__/| $$| $$  | $$  /$$$$$$$| $$  | $$|  $$$$$$ " << endl;
        cout << BLANCO << "                               | $$   | $$      | $$| $$  | $$ /$$__  $$| $$  | $$ \\____  $$" << endl;
        cout << ROJO << "                               | $$   | $$      | $$|  $$$$$$$|  $$$$$$$|  $$$$$$$ /$$$$$$$/" << endl;
        cout << BLANCO << "                               |__/   |__/      |__/ \\_______/ \\_______/ \\____  $$|_______/ " << endl;
        cout << ROJO << "                                                                         /$$  | $$          " << endl;
        cout << BLANCO << "                                              T.G.I     Restaurante     |  $$$$$$/     " << endl;
        cout << ROJO << "                                                                          \\______/           " << endl;

        cout << ConsolaColor::BLANCO;
    }




}