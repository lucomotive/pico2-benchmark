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
            name = "pico-build";
            src = ./.;

            # 1. Add whatever packages your script needs here
            nativeBuildInputs = with pkgs; [
              cmake
              gcc-arm-embedded
              python3
              # pkgs.pico-sdk.override { withSubmodules = true; }
            ];

            # 2. Run your script
            buildPhase = ''
              # Run your custom script.
              # (Using 'bash' ensures it runs even if it lacks a +x permission bit)
              bash ./nix/build.sh
            '';

            # 3. Copy the results to $out so Nix can save them in the `result/` folder
            installPhase = ''
              mkdir -p $out

              # Example: If your script creates a "build" folder with .uf2 files,
              # copy them to $out. Adjust this to match what your script actually outputs!
              find . -name "*.uf2" -exec cp {} $out/ \;
            '';
          };

          #devShells.default = import ./nix/shell.nix { inherit pkgs; };

        };
    };
}
