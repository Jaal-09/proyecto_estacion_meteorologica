#include "SensorHumedad.h"

SensorHumedad::SensorHumedad(Adafruit_BME280* instance) 
    : Sensor("Humedad", "% RH"), bme(instance) {}

bool SensorHumedad::iniciar() {
    return true;
}

float SensorHumedad::leerValor() {
    return bme->readHumidity();
}

String SensorHumedad::obtenerEstadoTexto() {
    return "";
}