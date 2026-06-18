# 🌦️ METEOROS-32 Sistema Embebido de Monitoreo Atmosférico e Iluminación

## 📝 Descripción del Proyecto
**METEOROS-32** es una estación meteorológica embebida de alta precisión basada en el microcontrolador **ESP32**. El sistema integra sensores de alta precisión  **BME280** (temperatura, humedad y presión atmosférica) y **BH1750** (intensidad lumínica). para capturar datos ambientales en tiempo real.  Además, administra el tiempo local mediante sincronización en la nube (NTP) y despliega una interfaz gráfica dinámica de doble vista en una pantalla **TFT LCD de 2.4" (ILI9341)** controlada mediante un botón de hardware externo.

Desarrollado en **PlatformIO** bajo el framework de **Arduino**, el firmware aplica principios de **Programación Orientada a Objetos (POO)** mediante *Managers de Control* independientes, y emplea múltiples buses **I2C** para aislar periféricos, optimizando la estabilidad del sistema y la precisión de las lecturas.

---

## 🏛️ Institución y Autoría
* **Institución:** Universidad de Pamplona
* **Programa:** Ingeniería Mecatrónica  
* **Autores:**
  * Jesus Alberto Arias Lizcano
  * Andrés Stivent Cortés Gomez
  * Darwin David Naranjo Calderón
  * Karen Daniela Suárez Rico
* **Fecha:** Junio 2026

---

## 🏗️ Arquitectura del Sistema (Mánagers de Control)
El firmware del proyecto está estructurado bajo un modelo modular, donde cada periférico físico posee su propia abstracción lógica (archivos `.h` y `.cpp` dedicados dentro de `include/` y `src/`):

1. **`Bme280Manager` (Ambiente):** Controla el sensor integrado Adafruit BME280 para extraer lecturas calibradas de temperatura (°C), humedad relativa (% RH) y presión atmosférica (hPa).
2. **`Bh1750Manager` (Luminosidad):** Gestiona el luxómetro digital BH1750 para medir la iluminancia en Lux (lx) y procesar un diagnóstico natural del entorno en texto (Oscuridad, Interior, Sol Directo).
3. **`RelojManager` (Tiempo):** Administra el chip de tiempo real (RTC) DS1307. Cuenta con lógica de conexión Wi-Fi temporizada para interrogar servidores horarios **NTP** (`pool.ntp.org`), ajustar la zona horaria fija (**UTC-5** para Colombia) y apagar el módulo de radiofrecuencia (`WIFI_OFF`) para eliminar interferencias electromagnéticas.
4. **`PantallaManager` (Interfaz Gráfica):** Controla la renderización en el panel TFT ILI9341 a través de la librería `TFT_eSPI`. Implementa un menú cíclico de dos vistas:
   * **Vista 1:** Fecha, hora digital en tiempo real y temperatura ambiente.
   * **Vista 2:** Detalles avanzados de humedad, presión atmosférica y estado natural de luz.

---

## Conexiones Físicas (Mapeo de Pines)

### 📊 Configuración de Buses I2C (Aislamiento de Periféricos)
Para evitar colisiones de direcciones en el bus y disminuir la capacitancia parásita en las líneas de datos, se han levantado dos buses I2C independientes dentro del ESP32.

### 📌 Tabla Completa de Asignación de Pines

| Periférico / Componente | Pin Físico Pantalla/Sensor | Pin GPIO ESP32 | Tipo de Bus / Función |
| :--- | :--- | :--- | :--- |
| **Pulsador Externo** | Pin NO / Signal | **GPIO 5** | Entrada Digital con Pull-Up Interno |
| **Sensor Luxómetro BH1750** | SDA | **GPIO 21** | Bus I2C Nativo (`Wire`) |
| **Sensor Luxómetro BH1750** | SCL | **GPIO 22** | Bus I2C Nativo (`Wire`) |
| **Reloj Físico RTC DS1307** | SDA | **GPIO 21** | Bus I2C Nativo (`Wire`) |
| **Reloj Físico RTC DS1307** | SCL | **GPIO 22** | Bus I2C Nativo (`Wire`) |
| **Sensor Ambiental BME280** | SDA | **GPIO 18** | Bus I2C Secundario (`Wire1`) |
| **Sensor Ambiental BME280** | SCL | **GPIO 19** | Bus I2C Secundario (`Wire1`) |
| **Pantalla TFT ILI9341** | TFT_RD (Read) | **GPIO 4** | Control Bus Paralelo 8-bits |
| **Pantalla TFT ILI9341** | TFT_WR (Write) | **GPIO 33** | Control Bus Paralelo 8-bits |
| **Pantalla TFT ILI9341** | TFT_DC (Data/Cmd) | **GPIO 32** | Control Bus Paralelo 8-bits |
| **Pantalla TFT ILI9341** | TFT_CS (Chip Sel) | **GPIO 2** | Control Bus Paralelo 8-bits |
| **Pantalla TFT ILI9341** | TFT_RST (Reset) | **GPIO 15** | Control Bus Paralelo 8-bits |
| **Pantalla TFT ILI9341** | TFT_D0 | **GPIO 12** | Bus de Datos Paralelo (Bit 0) |
| **Pantalla TFT ILI9341** | TFT_D1 | **GPIO 13** | Bus de Datos Paralelo (Bit 1) |
| **Pantalla TFT ILI9341** | TFT_D2 | **GPIO 26** | Bus de Datos Paralelo (Bit 2) |
| **Pantalla TFT ILI9341** | TFT_D3 | **GPIO 25** | Bus de Datos Paralelo (Bit 3) |
| **Pantalla TFT ILI9341** | TFT_D4 | **GPIO 17** | Bus de Datos Paralelo (Bit 4) |
| **Pantalla TFT ILI9341** | TFT_D5 | **GPIO 16** | Bus de Datos Paralelo (Bit 5) |
| **Pantalla TFT ILI9341** | TFT_D6 | **GPIO 27** | Bus de Datos Paralelo (Bit 6) |
| **Pantalla TFT ILI9341** | TFT_D7 | **GPIO 14** | Bus de Datos Paralelo (Bit 7) |

---

## 💻 Características del Firmware e Inmunidad a Fallos
* **Algoritmo de Debounce Mecánico:** La conmutación de pantallas mediante el botón del GPIO 5 se gestiona mediante control por flanco de bajada combinada con un retardo controlado de 250ms para absorber rebotes eléctricos espurios.
* **Aislamiento en Arranque:** Si un sensor se desconecta físicamente o falla en el arranque, la bandera `sensorOperativo` conmuta a `false`. El firmware evita bloqueos en el bucle principal (`loop`), devolviendo valores seguros `0.0` y notificando el estado en pantalla.
* **Formateo Estricto de Datos:** Se implementa un método privado de conversión de cadenas (`formatearDosDigitos`) para garantizar la simetría visual en el reloj digital, anteponiendo ceros cuando los valores son menores a 10.

---

## 🛠️ Tecnologías y Librerías Utilizadas
El entorno de compilación está gestionado por las siguientes dependencias oficiales especificadas en el archivo `platformio.ini`:

* **`bodmer/TFT_eSPI` (v2.5.43):** Driver de renderizado de alta velocidad modificado para operar bajo bus paralelo de 8 bits (`ESP32_PARALLEL=1`).
* **`adafruit/RTClib` (v2.1.4):** Interfaz para la lectura y ajuste del integrado DS1307.
* **`adafruit/Adafruit BME280 Library` (v2.2.4):** API para la captura y compensación matemática de datos termo-higro-barométricos.
* **`claws/BH1750` (v1.3.0):** Driver del luxómetro configurado en Modo Continuo de Alta Resolución (Mide constantemente cada 120ms).

---

## 🚀 Estructura de Archivos

   ```text
   ├── include/
   │   ├── Bh1750Manager.h
   │   ├── Bme280Manager.h
   │   ├── PantallaManager.h
   │   └── RelojManager.h
   ├── src/
   │   ├── Bh1750Manager.cpp
   │   ├── Bme280Manager.cpp
   │   ├── PantallaManager.cpp
   │   ├── RelojManager.cpp
   │   └── main.cpp
   └── platformio.ini
   ```

## 📚 Referencias

1. **Espressif Systems (2022).** *ESP32 Technical Reference Manual*. 
   [Official Hardware Documentation](https://www.espressif.com/en/support/documents/technical-documents)
2. **Adafruit Industries (2024).** *Adafruit BME280 Sensor Overview & Calibration*. 
   [Adafruit Learning System Guide](https://learn.adafruit.com/adafruit-bme280-humidity-barometric-pressure-temperature-sensor-breakout)
3. **Bodmer (2025).** *TFT_eSPI Library for ESP32 Parallel and SPI Interfaces*. 
   [GitHub Repository](https://github.com/Bodmer/TFT_eSPI)
4. **Santos, R., & Santos, S. (2023).** *ESP32: TFT LCD Touchscreen Display – 2.8 inch ILI9341 240×320 (Arduino IDE)*. 
   [Random Nerd Tutorials Guide](https://randomnerdtutorials.com/esp32-tft-touchscreen-display-2-8-ili9341-arduino/)
5. **ElectroPeak (2023).** *Interfacing 2.8-inch TFT LCD Touch Screen with ESP32 Step-by-Step Guide*. 
   [ElectroPeak Learn Article](https://electropeak.com/learn/interfacing-2-8-inch-tft-lcd-touch-screen-with-esp32/)
6. **Wouterlood, F. (2021).** *ESP32-WROOM-32 and ILI9341 TFT display – an interesting match*. 
   [TheSolarUniverse Engineering Blog](https://thesolaruniverse.wordpress.com/2021/03/01/esp32-wroom-32-and-ili9341-tft-display-an-interesting-match/)
7. **r/Esphome Community (2025).** *2.4" TFT LCD Connection Shield to ESP32 V1.1 (Hardware Pin Mapping & Troubleshooting)*. 
   [Reddit Engineering Discussion](https://www.reddit.com/r/Esphome/comments/1irok4n/24_tft_lcd_connection_shield_to_esp32_v11/?tl=es-419)
8. **Teach Me Something (2023).** *ESP32 TFT LCD Shield DHT11 Temperature & Humidity Monitor (ILI9341 2.4" Parallel Interface Setup)*. 
   [Video Tutorial Guide](https://www.youtube.com/watch?v=xJzI6glntpA)
9. **Eder (2024).** *Construyendo tu propia estación meteorológica IoT con ESP32: Monitoreo en tiempo real*. 
   [Medium Engineering Publication](https://medium.com/@eder88559/construyendo-tu-propia-estaci%C3%B3n-meteorol%C3%B3gica-iot-con-esp32-monitoreo-en-tiempo-real-y-datos-en-58505fbffa18)
10. **TecnoReBot. (2023).** *Estación meteorológica con ESP32 y DISPLAY LCD ( GUÍA COMPLETA )*. 
    [Video Tutorial Guide](https://www.youtube.com/watch?v=jBKp-3VPRVA)