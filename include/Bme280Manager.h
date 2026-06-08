/**
 * @file Bme280Manager.h
 * @brief Control y lectura del sensor termo-higro-barométrico BME280.
 * @details Esta clase gestiona la comunicación I2C con el sensor BME280 de Adafruit,
 * permitiendo extraer los valores de temperatura, humedad relativa 
 * y presión atmosférica para la estación.
 * @date Junio 2026
 */

#ifndef BME280_MANAGER_H
#define BME280_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

 /**
 * @class Bme280Manager
 * @brief Clase mánager para el control unificado de las lecturas del BME280.
 */
class Bme280Manager {
private:
    Adafruit_BME280 bme;  /**< Instancia interna de la librería de Adafruit para controlar el chip BME280. */
    bool sensorOperativo; /**< Bandera de control; se pone en true solo si el chip responde correctamente en el inicio. */

public:

    /**
     * @brief Constructor del mánager del BME280.
     * @details Inicializa los estados lógicos internos, marcando el sensor como no operativo por seguridad antes del arranque.
     */
    Bme280Manager();

    /**
     * @brief Inicializa el hardware del sensor a través del bus I2C.
     * @details Busca el dispositivo en la dirección física del bus y configura sus registros.
     * @return true si el sensor fue encontrado y arrancó bien, false si hay un problema de cableado o dirección errónea.
     */
    bool iniciar();
    
    /**
     * @brief Obtiene la lectura de temperatura ambiente actual.
     * @details Realiza la conversión del sensor y devuelve el valor en grados Celsius. Si el sensor falló, retorna 0.0 de forma segura.
     * @return Valor flotante de la temperatura en °C.
     */
    float obtenerTemperatura();

    /**
     * @brief Obtiene la lectura de humedad relativa del aire.
     * @details Captura el porcentaje de humedad en el ambiente. Si el hardware no responde, retorna 0.0 para evitar cálculos erróneos.
     * @return Valor flotante del porcentaje de humedad relativa (% RH).
     */
    float obtenerHumedad();

    /**
     * @brief Obtiene la lectura de la presión atmosférica.
     * @details Captura la presión en Pascales internos y la convierte a Hectopascales (hPa), que es la unidad estándar en meteorología.
     * @return Valor flotante de la presión atmosférica en hPa.
     */
    float obtenerPresion();
};

#endif