#ifndef SENSOR_PRESION_H
#define SENSOR_PRESION_H

#include "Sensor.h"
#include <Adafruit_BME280.h>

class SensorPresion : public Sensor {
private:
    Adafruit_BME280* bme;
public:
    SensorPresion(Adafruit_BME280* instance);
    bool iniciar() override;
    float leerValor() override;
    String obtenerEstadoTexto() override;
};

#endif