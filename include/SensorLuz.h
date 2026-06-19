/**
 * @file SensorLuz.h
 * @brief Herencia de la clase Sensor.
 * @date Junio 2026
 */

#ifndef SENSOR_LUZ_H
#define SENSOR_LUZ_H

#include "Sensor.h"
#include <Wire.h>
#include <BH1750.h>

class SensorLuz : public Sensor {
private:
    BH1750 luxometro;       /**< Instancia interna de la librería base del sensor BH1750. */
    uint8_t direccionI2C;   /**< Dirección física I2C del sensor (normalmente es 0x23 o 0x5C). */
    int pinSDA;             /**< Pin asignado para la línea de datos I2C (Pin 21 por defecto). */
    int pinSCL;             /**< Pin asignado para la línea de reloj I2C (Pin 22 por defecto). */
    bool sensorOperativo;   /**< Bandera de seguridad; se pone en true si el sensor responde en el arranque. */

public:
    /**
     * @brief Constructor del mánager adaptado a Sensor.
     */
    SensorLuz(uint8_t direccion = 0x23, int sda = 21, int scl = 22);
    
    // Métodos obligatorios que se sobreescriben de la clase abstracta base
    bool iniciar() override;
    float leerValor() override;
    String obtenerEstadoTexto() override;

    // Métodos 
    float obtenerLux();
    float obtenerLuz();
    void mostrarLecturaSerial();
};

#endif