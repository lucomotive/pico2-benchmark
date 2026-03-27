set -e

if [ -z "$1" ]; then
    echo "Usage: pico-flash <project>"
    exit 1
fi

PROJECT=$1
UF2=$(ls "build/$PROJECT/"*.uf2 2>/dev/null | head -1)

if [ -z "$UF2" ]; then
    echo "✗ No .uf2 found, run pico-build $PROJECT first"
    exit 1
fi


echo "→ Flashing $UF2..."
picotool load "$UF2" --force --execute
#picotool load "$UF2" --force
#picotool reboot
echo "✓ Flashed!"
