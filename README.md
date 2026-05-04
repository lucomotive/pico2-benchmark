# Projekt
In diesem Projekt wird ein RP Pico 2 für die Berechnung verschiedener Matrixoperationen getestet.
Dazu wird die Library "Eigen" verwendet

## Orderstruktur
### results
Hier werden die Benchmark-Ergebnisse gespeichert.

Der pico 2 kommuniziert mit einem python script, dass anweisung gibt welches benchmark mit welcher präzision (float/double) durchgeführt werden soll.

### src + include
hier sind ist der code für den pico 2 und das python script abgelegt

### console
hier ist ein python script um den pico 2 zu steuern

## Python script
das python script kommuniziert mit dem microcontroller. in der konsole kann hier mit 
```
run --benchmark=<...> --precision=<float|double>
```
ein benchmark gestartet werden.
Das script erstell automatisch eine datei und legt die ergebnisse ab


## Fertige Benchmarks
- LU Zerlegung
- QR Zerlegung
- Matrix multiplikation
- inverse
- Determinante
- Copy
- Stack vs Heap
- Heap fragemnation (allocate and delete)

## Geplante Benchmarks
- Division (matmul mit Inverse) (überflüssig)

- Speicherallokation
  - IO Read/Write
  - Ram Painting

- Transpose (überflüssig, kopie mit extra steps)
- LGS
- Convolution
- Fourier Transform

- ggf multicore Berechnung


### Vergleich
Jede Matrix-Operation wird mit verschiedenen Größen getestet um einen qualifizierten Vergleich zu erhalten.
- Geschwindigkeit
- Stromverbrauch

### Hardware
- PICO 1
- PICO 2 (arm + risc-v)
- Zero 2 W (mit Alpine Linux)
- x86 PC (Ryzen 5 5600h)


## Prognose
### PICO 2
Der PICO 2 hat [520kB SRAM](https://pip-assets.raspberrypi.com/categories/1088-raspberry-pi-pico-2-w/documents/RP-008304-DS-2-pico-2-w-datasheet.pdf?disposition=inline) zur Verfügung.
Das entspricht 130,000 floats mit 32 bit Größe.
Eine 360x360 Matrix würde diesen Platz gänzlich ausreizen.
