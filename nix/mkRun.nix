{
  pkgs ? import <nixpkgs> { },
  name,
}:
toString (
  pkgs.writeShellScript "build-and-copy" ''
    set -e
    nix build .#${name} --no-link
    mkdir -p ./build/${name}
    cp -rL $(nix path-info .#${name})/* ./build/${name}
  ''
)
