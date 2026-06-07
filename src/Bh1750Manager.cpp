#include "Bh1750Manager.h"

Bh1750Manager::Bh1750Manager() {
    sensorOperativo = false;
}

bool Bh1750Manager::iniciar() {

    Serial.println("[LUZ] Iniciando bus I2C para BH1750...");
    
    // GPIO 5 para SDA y GPIO 23 para SCL
    bool busIniciado = Wire.begin(5, 23); 
    
    if (!busIniciado) {
        Serial.println("[ERROR] No se pudo inicializar el bus I2C en pines 5 y 23.");
        return false;
    }
}