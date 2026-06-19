#ifndef ESTACION_METEOROLOGICA_H
#define ESTACION_METEOROLOGICA_H

#include <Arduino.h>
#include <Adafruit_BME280.h>
#include "PantallaManager.h"
#include "RelojManager.h"
#include "Sensor.h"

class EstacionMeteorologica {
private:
    int pinBoton;
    unsigned long tiempoUltimaActualizacion;
    const unsigned long INTERVALO_TFT;
    bool primeraActualizacion;
    bool estadoBotonAnterior;

    const char* ssid;
    const char* password;

    PantallaManager pantalla;
    RelojManager reloj;
    
    // Objeto físico compartido del BME280
    Adafruit_BME280 bmeFisico;

    // El arreglo polimórfico de sensores heredados
    Sensor* misSensores[4];

public:
    EstacionMeteorologica(int pinBoton, const char* wifiSSID, const char* wifiPASS);
    ~EstacionMeteorologica();
    void iniciar();
    void procesar();
};

#endif