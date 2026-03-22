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
          packages.default = pkgs.stdenv.mkDerivation {

            name = "pico-project";

            nativeBuildInputs = with pkgs; [
              cmake
              gcc-arm-embedded
              python3
              #ninja
            ];

            buildCommand = ''
              cd build
              cmake ../blink
            '';

          };
          devShells.default = import ./nix/shell.nix { inherit pkgs; };

        };
    };
}
