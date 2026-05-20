{
  pkgs ? import <nixpkgs> { },
}:
with pkgs;
let
  buildBenchmarks = writeShellScriptBin "build-common" (builtins.readFile ./common/build.sh);
  hostBuildScript = writeShellScriptBin "build-host" (builtins.readFile ./host/build.sh);

  picoFlashScript = writeShellScriptBin "pico-flash" (builtins.readFile ./pico/flash.sh);
  picoRunScript = writeShellScriptBin "pico-run" (builtins.readFile ./pico/run.sh);

  my-pico-sdk = pkgs.pico-sdk.override {
    withSubmodules = true;
  };
in
mkShell {
  name = "pico-dev-shell";

  buildInputs = [
    nil
    nixd

    buildBenchmarks
    hostBuildScript
    picoFlashScript
    picoRunScript

    # Compiler and build tools
    gcc-arm-embedded
    cmake
    #ninja
    python3 # Required by the SDK for scripts like pioasm
    # (python3.withPackages (
    #   python-pkgs: with python-pkgs; [
    #     pyserial
    #     prompt-toolkit
    #   ]
    # ))

    eigen
    nlohmann_json
    #gbenchmark

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
