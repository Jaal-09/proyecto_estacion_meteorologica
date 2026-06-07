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

    Serial.println("[LUZ] Buscando sensor BH1750 en pines 5 y 23...");

    // Vinculamos el sensor al bus 'Wire' que acabamos de mover a los pines 5 y 23
    if (!lightSensor.begin(0x23, &Wire)) {
        Serial.println("[ERROR] ¡No se encontró el BH1750! Revisa pines 5/23 o energía.");
        sensorOperativo = false;
        return false;
    }

    Serial.println("[LUZ] ¡Sensor BH1750 detectado en pines 5 y 23 con éxito!");
    sensorOperativo = true;
    return true;
}

float Bh1750Manager::obtenerLuz() {
    if (!sensorOperativo) return 0.0;

    // Retorna el valor real en Lux
    return lightSensor.readLightLevel(); 
}