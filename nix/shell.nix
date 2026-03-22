{
  pkgs ? import <nixpkgs> { },
}:
with pkgs;
mkShell {
  name = "pico-dev-shell";

  buildInputs = [
    # Compiler and build tools
    gcc-arm-embedded
    cmake
    #ninja
    python3 # Required by the SDK for scripts like pioasm

    # Pico specific tools
    pico-sdk
    picotool

    #minicom

    # Optional: C/C++ language server for your editor
    clang-tools
  ];
  shellHook = ''
    export PICO_SDK_PATH="${pico-sdk}/lib/pico-sdk"

    echo "======================================="
    echo "🍓 Raspberry Pi Pico SDK environment 🍓"
    echo "======================================="
    echo "PICO_SDK_PATH is set to: $PICO_SDK_PATH"
  '';
}
