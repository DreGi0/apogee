#!/usr/bin/env bash
set -euo pipefail

XAUTH=/tmp/.docker.xauth

if [ -d "$XAUTH" ]; then
    echo "[run.sh] $XAUTH was used as directory by docker, cleaning up..."
    sudo rm -rf "$XAUTH"
fi

rm -f "$XAUTH"
touch "$XAUTH"
xauth nlist "$DISPLAY" | sed -e 's/^..../ffff/' | xauth -f "$XAUTH" nmerge -
chmod 644 "$XAUTH"

echo "[run.sh] cookie X11 ready in $XAUTH for DISPLAY=$DISPLAY"

docker compose up --build