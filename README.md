# Herramientas-Cpp · Diseño nativo

Ocho herramientas de diseño en C++20, sin dependencias gráficas: generan recursos
SVG, CSS y HTML que puedes abrir en un navegador o editor vectorial.

[Ver los ejemplos generados](ejemplos/README.md).

## Compilar

Con CMake 3.20+ y un compilador C++20 (MSVC en Windows o GCC/Clang en Linux):

```console
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Ejecutable `build/Release/diseno.exe` en Visual Studio o `build/diseno` en Linux.
También puedes compilar en Linux con `g++ -std=c++20 -O2 src/main.cpp -o diseno`.

```console
diseno degradado fondo.svg "#10152E" "#46C9B5"
diseno mosaico mosaico.svg "#10152E" "#46C9B5" 11
diseno puntos puntos.svg "#10152E" "#46C9B5"
diseno monograma icono.svg SP "#173B37" "#D6E7A8"
diseno contraste "#FFFFFF" "#10152E"
diseno paleta tonos.css "#468C79"
diseno espaciado escala.svg
diseno galeria C:\Fotos galeria.html
```

Desde la carpeta del ejecutable, usa `./diseno` o `.\diseno.exe` según tu terminal.
Los destinos deben ser nuevos y las carpetas padre deben existir. Se rechaza una
extensión de salida equivocada. Si falla una escritura, se intenta retirar su
salida parcial. Los SVG no son ICO ni PNG; expórtalos para usarlos como fondos.
Los fondos SVG son 1920 × 1080; monogramas 512 × 512. Las fuentes dependen del equipo.
La semilla del mosaico permite reproducir el diseño. La paleta mezcla el color base
con blanco/negro; comprueba su contraste antes de usarla para texto.

`galeria` integra hasta 24 imágenes del primer nivel en un HTML autónomo, por orden
de nombre: hasta 8 MiB por imagen y 32 MiB en total. No elimina metadatos de las
imágenes. No modifica originales y omite enlaces simbólicos; revisa el contenido
antes de compartir el HTML. La imagen se decodifica al abrirla en el navegador.

El cálculo WCAG evalúa solo colores sRGB opacos. Texto grande: 18 pt normal o
14 pt en negrita. No sustituye una auditoría completa de accesibilidad.
Los workflows compilan y prueban en Windows y Linux, y generan artefactos binarios.
