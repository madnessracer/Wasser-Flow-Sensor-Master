
# 4-fach Wasser Flow Sensoren Master


Dieses Projekt misst die Wassermenge und die Durchflussrate (Liter pro Minute) mit bis zu vier YF-S201 Wasser-Flow-Sensoren und einem D1 Mini ESP32.
Die gemessenen Werte werden dauerhaft in einem externen FRAM-Modul gespeichert und bleiben auch nach einem Stromausfall erhalten.


## Features

- Messung der aktuellen Durchflussrate (L/min) für bis zu 4 Sensoren
- Berechnung der Gesamtmenge (Liter) je Sensor
- Ausgabe der Werte für alle Sensoren über die serielle Schnittstelle
- Hardware-Timer für präzise, unterbrechungsfreie Messung


## Hardware

- D1 Mini ESP32
- Bis zu 4x YF-S201 Wasser-Flow-Sensor
- Anschluss: Signal-Pins der Sensoren an GPIO2 (D2), GPIO3 (D3), GPIO4 (D4), GPIO5 (D5) des ESP32


### Anschlussplan YF-S201 an D1 Mini ESP32 (für 4 Sensoren)

| YF-S201 | ESP32 (D1 Mini) |
|---------|-----------------|
| Rot     | 5V              |
| Schwarz | GND             |
| Gelb    | D2 (GPIO2)      |
| Gelb    | D3 (GPIO3)      |
| Gelb    | D4 (GPIO4)      |
| Gelb    | D5 (GPIO5)      |


**Hinweis:**
- Jeder Sensor bekommt einen eigenen Signal-Pin am ESP32 (D2, D3, D4, D5).
- VCC des Sensors benötigt 5V. Die meisten D1 Mini ESP32 Boards haben einen 5V Pin.
- GND des Sensors kommt an GND des ESP32.
- Die Gesamtmenge jedes Sensors wird im externen FRAM-Modul (I2C, z.B. MB85RC256V) gespeichert und bleibt auch nach Stromausfall erhalten.
- Die gesamte Flowmeter-Logik ist in einer eigenen Klasse gekapselt (`flowmeter.h`).

#### ASCII-Schaltbild (4 Sensoren, D1 Mini ESP32, FRAM)

```
YF-S201_1         D1 Mini ESP32
  +-----+         +-----------+
  | Rot |---------| 5V        |
  | Gelb|---------| D2 (GPIO2)|
  |Schwarz|------| GND        |
  +-----+         +-----------+

YF-S201_2         D1 Mini ESP32
  +-----+         +-----------+
  | Rot |---------| 5V        |
  | Gelb|---------| D3 (GPIO3)|
  |Schwarz|------| GND        |
  +-----+         +-----------+

YF-S201_3         D1 Mini ESP32
  +-----+         +-----------+
  | Rot |---------| 5V        |
  | Gelb|---------| D4 (GPIO4)|
  |Schwarz|------| GND        |
  +-----+         +-----------+

YF-S201_4         D1 Mini ESP32
  +-----+         +-----------+
  | Rot |---------| 5V        |
  | Gelb|---------| D5 (GPIO5)|
  |Schwarz|------| GND        |
  +-----+         +-----------+

FRAM-Modul (z.B. MB85RC256V)
  +-----+         +-----------+
  | VCC |---------| 3.3V/5V   |
  | GND |---------| GND       |
  | SCL |---------| SCL (D1)  |
  | SDA |---------| SDA (D2)  |
  +-----+         +-----------+
```

## Installation

1. PlatformIO installieren
2. Projekt klonen:
   ```sh
   git clone https://github.com/madnessracer/Wasser-Flow-Sensor-Master.git
   ```
3. Abhängigkeiten installieren (falls benötigt)
4. Board auswählen und Code flashen


## Code-Überblick

- Die Impulse des Sensors werden per Interrupt gezählt.
- Ein Hardware-Timer berechnet jede Sekunde die Durchflussrate und die Gesamtmenge.
- Die aktuellen Werte werden in Variablen gespeichert und seriell ausgegeben.
- Die Gesamtmengen jedes Sensors werden dauerhaft im externen FRAM-Modul gespeichert und beim Start automatisch geladen.


## Beispiel-Ausgabe

```
Sensor 1: Durchflussrate: 2.34 L/min, Gesamt: 5.67 L
Sensor 2: Durchflussrate: 1.12 L/min, Gesamt: 3.45 L
Sensor 3: Durchflussrate: 0.00 L/min, Gesamt: 0.00 L
Sensor 4: Durchflussrate: 0.00 L/min, Gesamt: 0.00 L
----------------------
```


## Anpassungen & Hinweise

- Für andere Sensoren kann der Wert `PULSES_PER_LITER` angepasst werden.
- Die Ausgabe kann beliebig erweitert werden (z.B. Webserver, Display).
- Die Flowmeter-Logik ist modular in einer eigenen Klasse ausgelagert (`include/flowmeter.h`).
- Die Werte werden sicher im FRAM gespeichert und beim Start automatisch geladen.

## Lizenz

MIT
