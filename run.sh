#!/usr/bin/env bash
set -euo pipefail

XAUTH=/tmp/.docker.xauth

rm -f "$XAUTH"
touch "$XAUTH"
xauth nlist "$DISPLAY" | sed -e 's/^..../ffff/' | xauth -f "$XAUTH" nmerge -
chmod 644 "$XAUTH"

echo "[run.sh] cookie X11 ready in $XAUTH for DISPLAY=$DISPLAY"

docker compose up --build