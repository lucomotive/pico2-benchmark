{
  pkgs ? import <nixpkgs> { },
}:
with pkgs;
let
  buildScript = writeShellScriptBin "pico-build" (builtins.readFile ./build.sh);
  flashScript = writeShellScriptBin "pico-flash" (builtins.readFile ./flash.sh);
  observeScript = writeShellScriptBin "pico-observe" (builtins.readFile ./observe.sh);

  my-pico-sdk = pkgs.pico-sdk.override {
    withSubmodules = true;
  };
in
mkShell {
  name = "pico-dev-shell";

  buildInputs = [
    buildScript
    flashScript
    observeScript

    # Compiler and build tools
    gcc-arm-embedded
    cmake
    #ninja
    python3 # Required by the SDK for scripts like pioasm
    eigen

    # Pico specific tools
    my-pico-sdk
    picotool

    # monitoring
    tio

    # Optional: C/C++ language server for your editor
    clang-tools
  ];

  shellHook = ''
    export PICO_SDK_PATH="${my-pico-sdk}/lib/pico-sdk"

    echo "======================================="
    echo "🍓 Raspberry Pi Pico SDK environment 🍓"
    echo "======================================="
    echo "PICO_SDK_PATH is set to: $PICO_SDK_PATH"

    echo "Build project with 'pico-build <project>'"
    echo "Flash project with 'sudo pico-flash <project>'"
  '';
}
