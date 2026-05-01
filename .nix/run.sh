set -e

cmake -B build -S .
cmake --build build -j$(nproc)

UF2=$(ls "build/"*.uf2 2>/dev/null | head -1)
sudo picotool load "$UF2" --force --execute

# wait for pico to start
echo "Waiting for pico to start..."
sleep 3

# run console script
python console.py
