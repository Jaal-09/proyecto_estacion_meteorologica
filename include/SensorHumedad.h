#ifndef SENSOR_HUMEDAD_H
#define SENSOR_HUMEDAD_H

#include "Sensor.h"
#include <Adafruit_BME280.h>

class SensorHumedad : public Sensor {
private:
    Adafruit_BME280* bme;
public:
    SensorHumedad(Adafruit_BME280* instance);
    bool iniciar() override;
    float leerValor() override;
    String obtenerEstadoTexto() override;
};

#endif