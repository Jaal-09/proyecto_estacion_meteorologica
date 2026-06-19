/**
 * @file SensorLuz.cpp
 * @brief Implementación de la clase SensorLuz.
 * @date Junio 2026
 */

#include "SensorLuz.h"

SensorLuz::SensorLuz(uint8_t direccion, int sda, int scl) 
    : Sensor("Iluminacion", "lx") {
    this->direccionI2C = direccion;
    this->pinSDA = sda;
    this->pinSCL = scl;
    this->sensorOperativo = false; // Arranca en false hasta que pase el test en iniciar()
}

bool SensorLuz::iniciar() {
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

float SensorLuz::leerValor() {
    // Si el sensor no se inició correctamente en el arranque, evitamos llamadas colgadas y devolvemos 0
    if (!sensorOperativo) return 0.0f;
    return luxometro.readLightLevel(); // Retorna la lectura real en Lux (lx)
}

float SensorLuz::obtenerLux() {
    return leerValor(); // Redirecciona al método unificado
}

float SensorLuz::obtenerLuz() {
    return leerValor(); // Redirecciona directo al método original
}

String SensorLuz::obtenerEstadoTexto() {
    // Si el sensor falló desde el inicio, retornamos un aviso seguro
    if (!sensorOperativo) return "Desconectado";

    float lux = leerValor();
    
    // Clasificación del ambiente según la cantidad de luz recibida 
    if (lux < 15)       return " Oscuro";
    if (lux < 800)      return " Tenue";
    if (lux < 12000)    return " Claro";
    
    return " Soleado";
}

void SensorLuz::mostrarLecturaSerial() {
    float lux = leerValor();
    String estado = obtenerEstadoTexto(); 
    
    Serial.println("----- MONITOREO DE LUMINOSIDAD -----");
    Serial.printf("Intensidad de Luz: %.1f lx\n", lux);
    Serial.println("Estado Ambiental: " + estado);
    Serial.println("------------------------------------\n");
}