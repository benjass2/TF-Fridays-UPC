#include <iostream>
#include "locale.h"
#include "pch.h"
#include "Fridays.hpp"  


int main() {
    setlocale(LC_ALL, "Spanish");

    Fridays sistema;
    sistema.ejecutar();


    return 0;
}
