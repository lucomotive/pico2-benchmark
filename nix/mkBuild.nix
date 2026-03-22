{
  pkgs ? import <nixpkgs> { },
  project,
}:
with pkgs;
stdenv.mkDerivation {
  name = project;
  system = stdenv.system;
  src = "./" + project;

  nativeBuildInputs = [
    cmake
    gcc-arm-embedded
    pico-sdk
    picotool
    python3
  ];

  cmakeFlags = [
    "-DPICO_SDK_PATH=${pico-sdk}/lib/pico-sdk"
    "-DCMAKE_C_COMPILER=${gcc-arm-embedded}/bin/arm-none-eabi-gcc"
    "-DCMAKE_CXX_COMPILER=${gcc-arm-embedded}/bin/arm-none-eabi-g++"
    "-DCMAKE_ASM_COMPILER=${gcc-arm-embedded}/bin/arm-none-eabi-gcc"
  ];

  installPhase = ''
    mkdir -p $out
    cp *.uf2 $out/
    cp *.elf $out/
    cp *.bin $out/
  '';
}
