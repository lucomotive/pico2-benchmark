# Projekt
In diesem Projekt wird ein RP Pico 2 für die Berechnung verschiedener Matrixoperationen getestet.
Dazu wird die Library "Eigen" verwendet

## Orderstruktur
### projects
In diesem Ordner sind mehrere Projekte angelegt, die jeweils den pico 2 auf verschiedene Matrixoperationen testen.

### results
Hier werden die Benchmark-Ergebnisse gespeichert.

Jedes benchmark wartet auf ein Nutzersignal. Erst dann fängt der microcontroller and zu rechnen.
```C
//  this inits the serial connection
stdio_init_all();

// this waits for the usb connection
while (!stdio_usb_connected()) {
  sleep_ms(100);
}
```
Das hat den Nutzen um den Microcontroller und PC zu synchronisieren, damit die Ergebnisse anständig in einer Datei gespeichert werden können.
Bsp:
```bash
tio /dev/ttyACM0 | tee results/.../benchmark.csv
```

### build
In diesem Ordner werden die build outputs jedes Projektes in "projects" abgelegt um diese kollektiv in der .gitignore auszuschließen

### .nix
Hier sind alle [nix](https://nixos.org/) bezogenen Dateien abgelegt.

## Fertige Benchmarks
- LU Zerlegung
- QR Zerlegung
- Matrix multiplikation

## Geplante Benchmarks
- Matrix-Multiplikation
- Inverse
- Division (matmul mit Inverse)

- Speicherallokation
  - Stack vs Heap
  - IO Read/Write
  - Ram Painting
  - Heap fragemnation (allocate and delete)
  - Copy

- Determinante
- Transpose
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
