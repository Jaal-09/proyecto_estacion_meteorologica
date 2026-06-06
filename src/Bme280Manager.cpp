#include "Bme280Manager.h"

Bme280Manager::Bme280Manager() {
    sensorOperativo = false;
}

bool Bme280Manager::iniciar() {
    Serial.println("[BME280] Iniciando bus I2C secundario...");
    
    //Pines para el bme-280
    bool busIniciado = Wire1.begin(18, 19); 
    
    if (!busIniciado) {
        Serial.println("[ERROR] No se pudo inicializar el bus I2C en pines 18 y 19.");
        return false;
    }

    Serial.println("[BME280] Buscando sensor físico...");
    if (!bme.begin(0x76, &Wire1)) {
        Serial.println("[ERROR] ¡No se encontró el BME280! Revisa conexiones.");
        sensorOperativo = false;
        return false;
    }
    
    Serial.println("[BME280] ¡Sensor detectado en pines 18 y 19 con éxito!");
    sensorOperativo = true;
    return true;

}