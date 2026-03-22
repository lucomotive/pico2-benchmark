{
  description = "Raspberry Pi Pico 2 (RP2350) C/C++ Development Environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs =
    inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];

      perSystem =
        { pkgs, ... }:
        {
          packages.default = {

          };

          devShells.default = pkgs.mkShell {
            name = "pico-dev-shell";

            buildInputs = with pkgs; [
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
              export PICO_SDK_PATH="${pkgs.pico-sdk}/lib/pico-sdk"

              echo "======================================="
              echo "🍓 Raspberry Pi Pico SDK environment 🍓"
              echo "======================================="
              echo "PICO_SDK_PATH is set to: $PICO_SDK_PATH"
            '';
          };

        };
    };
}
