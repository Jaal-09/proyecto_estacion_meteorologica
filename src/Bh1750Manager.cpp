#include "Bh1750Manager.h"

Bh1750Manager::Bh1750Manager(uint8_t direccion, int sda, int scl) {
    this->direccionI2C = direccion;
    this->pinSDA = sda;
    this->pinSCL = scl;
    this->sensorOperativo = false;
}

bool Bh1750Manager::iniciar() {
    // Inicializar bus I2C en tus pines confiables 21 y 22
    Wire.begin(pinSDA, pinSCL);
    
    // Arrancar el sensor en el modo continuo de alta resolución
    if (!luxometro.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, direccionI2C, &Wire)) {
        Serial.println("Error crítico: ¡No se encuentra el sensor BH1750!.");
        sensorOperativo = false;
        return false;
    }
    
    Serial.println("Sensor de Luz BH1750 inicializado correctamente.");
    sensorOperativo = true;
    return true;
}

float Bh1750Manager::obtenerLux() {
    if (!sensorOperativo) return 0.0;
    return luxometro.readLightLevel(); // Retorna la lectura real en Lux (lx)
}

void Bh1750Manager::mostrarLecturaSerial() {
    float lux = obtenerLux();
    
    Serial.println("----- MONITOREO DE LUMINOSIDAD -----");
    Serial.print("Intensidad de Luz: ");
    Serial.print(lux);
    Serial.println(" lx");
    
    if (lux < 10) Serial.println("Estado: 🌑 Oscuridad total / Noche");
    else if (lux < 150) Serial.println("Estado: 💡 Iluminación de interiores tenue");
    else if (lux < 500) Serial.println("Estado: 🖥️ Oficina / Laboratorio bien iluminado");
    else Serial.println("Estado: ☀️ Luz solar directa / Exterior brillante");
    
    Serial.println("------------------------------------\n");
}