#include "SensorPresion.h"

SensorPresion::SensorPresion(Adafruit_BME280* instance) 
    : Sensor("Presion", "hPa"), bme(instance) {}

bool SensorPresion::iniciar() {
    return true;
}

float SensorPresion::leerValor() {
    return bme->readPressure() / 100.0F; // Conversión de Pa a hPa
}

String SensorPresion::obtenerEstadoTexto() {
    float presion = leerValor();
    if (presion < 775.0)  return "Lluvioso";
    if (presion <= 784.0) return "Nublado";
    return "Despejado";
}