set -e

cmake -B build -S .
cmake --build build -j$(nproc)

UF2=$(ls "build/"*.uf2 2>/dev/null | head -1)
sudo picotool load "$UF2" --force --execute

python console.py
