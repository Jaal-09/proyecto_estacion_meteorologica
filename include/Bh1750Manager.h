/**
 * @file Bh1750Manager.h
 * @brief Control y lectura del sensor de luz digital BH1750.
 * @details Esta clase se encarga de configurar el luxómetro BH1750 a través del bus 
 * I2C usando los pines nativos del ESP32, encargándose de entregar lecturas limpias en Lux.
 * @date Junio 2026
 */

#ifndef BH1750_MANAGER_H
#define BH1750_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>

/**
 * @class Bh1750Manager
 * @brief Clase mánager para gestionar el sensor de iluminancia BH1750.
 */
class Bh1750Manager {
private:
    BH1750 luxometro;       /**< Instancia interna de la librería base del sensor BH1750. */
    uint8_t direccionI2C;   /**< Dirección física I2C del sensor (normalmente es 0x23 o 0x5C). */
    int pinSDA;             /**< Pin asignado para la línea de datos I2C (Pin 21 por defecto). */
    int pinSCL;             /**< Pin asignado para la línea de reloj I2C (Pin 22 por defecto). */
    bool sensorOperativo;   /**< Bandera de seguridad; se pone en true si el sensor responde en el arranque. */

public:
    /**
     * @brief Constructor del mánager del sensor de luz.
     * @details Prepara las configuraciones iniciales de dirección y pines sin arrancar el bus todavía.
     * @param direccion Dirección esclava del bus I2C (Por defecto 0x23).
     * @param sda Pin de datos para el ESP32 (Por defecto el pin 21).
     * @param scl Pin de reloj para el ESP32 (Por defecto el pin 22).
     */
    Bh1750Manager(uint8_t direccion = 0x23, int sda = 21, int scl = 22);
    
    /**
     * @brief Enciende el bus I2C y despierta al sensor en el setup.
     * @details Inicializa la comunicación Wire en los pines y verifica que el chip esté conectado.
     * @return true si el sensor inició con éxito, false si no se encontró en el bus.
     */
    bool iniciar();

    /**
     * @brief Obtiene la medición actual de luz ambiental.
     * @details Realiza la lectura del sensor solo si pasó la prueba de inicio. Si el sensor falló, devuelve 0.0.
     * @return Valor flotante de la intensidad de luz calculada en Lux (lx).
     */
    float obtenerLux();

    /**
     * @brief Alias compatible para obtener la lectura de luz.
     * @details Apunta directamente a obtenerLux() para mantener la compatibilidad con los llamados del main.cpp.
     * @return Valor flotante en Lux.
     */
    float obtenerLuz();

    /**
     * @brief Obtiene una descripción textual y natural del estado de iluminación.
     * @details Analiza los luxes actuales y devuelve una cadena con un emoji y el diagnóstico 
     * ambiental para que pueda ser mostrado directamente en la pantalla TFT o en el Serial.
     * @return String Cadena de texto con el diagnóstico (ej: "Interior Tenue 💡").
     */
    String obtenerEstadoTexto();

    /**
     * @brief Envía la lectura de iluminancia directo al Monitor Serie.
     * @details Utilidad de depuración rápida para verificar los Lux actuales sin mirar la pantalla TFT.
     */
    void mostrarLecturaSerial();
};

#endif