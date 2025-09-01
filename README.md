# Wasser Flow Sensor Master

Dieses Projekt misst die Wassermenge und die Durchflussrate (Liter pro Minute) mit einem YF-S201 Wasser-Flow-Sensor und einem D1 Mini ESP32.

## Features

- Messung der aktuellen Durchflussrate (L/min)
- Berechnung der Gesamtmenge (Liter)
- Ausgabe der Werte über die serielle Schnittstelle
- Hardware-Timer für präzise, unterbrechungsfreie Messung

## Hardware

- D1 Mini ESP32
- YF-S201 Wasser-Flow-Sensor
- Anschluss: Signal-Pin des Sensors an GPIO2 (D2) des ESP32

### Anschlussplan YF-S201 an D1 Mini ESP32

| YF-S201 | ESP32 (D1 Mini) |
|---------|-----------------|
| Rot     | 5V              |
| Schwarz | GND             |
| Gelb    | D2 (GPIO2)      |

**Hinweis:**
- Der Signal-Pin (Gelb) wird mit D2 (GPIO2) des ESP32 verbunden.
- VCC des Sensors benötigt 5V. Die meisten D1 Mini ESP32 Boards haben einen 5V Pin.
- GND des Sensors kommt an GND des ESP32.

#### ASCII-Schaltbild

```
YF-S201         D1 Mini ESP32
  +-----+         +-----------+
  | Rot |---------| 5V        |
  | Gelb|---------| D2 (GPIO2)|
  |Schwarz|------| GND        |
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

## Beispiel-Ausgabe

```
Durchflussrate: 2.34 L/min, Gesamt: 5.67 L
```

## Anpassungen

- Für andere Sensoren kann der Wert `PULSES_PER_LITER` angepasst werden.
- Die Ausgabe kann beliebig erweitert werden (z.B. Webserver, Display).

## Lizenz

MIT
