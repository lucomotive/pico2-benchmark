set -e

if [ -z "$1" ]; then
    echo "Usage: pico-build <project>"
    echo "Projects:"
    echo "$(ls projects/)" | awk '{print "\t" $0}'
    #echo "Projects: $(ls -d */ | tr -d '/')"
    exit 1
fi

PROJECT=$1

if [ ! -d "projects/$PROJECT" ]; then
    echo "✗ Project '$PROJECT' not found"
    exit 1
fi

echo "→ Building $PROJECT..."
cmake -B "build/$PROJECT" -S "projects/$PROJECT"
cmake --build "build/$PROJECT" -j$(nproc)
echo "✓ Build complete!"
