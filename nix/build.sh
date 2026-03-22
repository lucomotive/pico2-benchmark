set -e

if [ -z "$1" ]; then
    echo "Usage: pico-build <project>"
    echo "Projects: $(ls -d */ | tr -d '/')"
    exit 1
fi

PROJECT=$1

if [ ! -d "$PROJECT" ]; then
    echo "✗ Project '$PROJECT' not found"
    exit 1
fi

echo "→ Building $PROJECT..."
cmake -B "build/$PROJECT" -S "$PROJECT"
cmake --build "build/$PROJECT" -j$(nproc)
echo "✓ Build complete!"
