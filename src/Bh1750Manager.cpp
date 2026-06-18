/**
 * @file Bh1750Manager.cpp
 * @brief Implementación de los métodos de control y lectura para el sensor BH1750.
 * @details Contiene la lógica para arrancar el bus I2C nativo, verificar el estado 
 * del hardware, capturar los niveles de lux y categorizar el tipo de iluminación.
 * @date Junio 2026
 */

#include "Bh1750Manager.h"

Bh1750Manager::Bh1750Manager(uint8_t direccion, int sda, int scl) {
    this->direccionI2C = direccion;
    this->pinSDA = sda;
    this->pinSCL = scl;
    this->sensorOperativo = false; // Arranca en false hasta que pase el test en iniciar()
}

bool Bh1750Manager::iniciar() {
    // Inicializar bus I2C en los pines 21 y 22
    Wire.begin(pinSDA, pinSCL);
    
    // Arrancar el sensor en el modo continuo de alta resolución (mide constantemente cada 120ms)
    if (!luxometro.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, direccionI2C, &Wire)) {
        Serial.println("Error crítico: ¡No se encuentra el sensor BH1750!.");
        sensorOperativo = false;
        return false;
    }
    
    Serial.println("Sensor de Luz BH1750 inicializado correctamente.");
    sensorOperativo = true; // El sensor respondió bien, habilitamos las lecturas
    return true;
}

float Bh1750Manager::obtenerLux() {
    // Si el sensor no se inició correctamente en el arranque, evitamos llamadas colgadas y devolvemos 0
    if (!sensorOperativo) return 0.0;
    return luxometro.readLightLevel(); // Retorna la lectura real en Lux (lx)
}

String Bh1750Manager::obtenerEstadoTexto() {
    // Si el sensor falló desde el inicio, retornamos un aviso seguro
    if (!sensorOperativo) return "Desconectado";

    float lux = obtenerLux();
    
    // Clasificación del ambiente según la cantidad de luz recibida
    if (lux < 10)       return " Oscuro";
    if (lux < 150)      return " Tenue";
    if (lux < 500)      return " Claro";
    
    return " Soleado";
}

void Bh1750Manager::mostrarLecturaSerial() {
    float lux = obtenerLux();
    String estado = obtenerEstadoTexto(); 
    
    Serial.println("----- MONITOREO DE LUMINOSIDAD -----");
    Serial.printf("Intensidad de Luz: %.1f lx\n", lux);
    Serial.println("Estado Ambiental: " + estado);
    Serial.println("------------------------------------\n");
}

float Bh1750Manager::obtenerLuz() {
    return obtenerLux(); // Redirecciona directo al método original
}