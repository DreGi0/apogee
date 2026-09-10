# Apogee

A [Kerbal Space Program](https://www.kerbalspaceprogram.com/) style like space flight game, built from scratch 
in C++20 with OpenGL and no game engine. It is a Personal project to explore real orbital mechanics and low-level 
graphics/physics programming, built as a portfolio piece.

## Status

Early development. Core windowing and OpenGL context are up (GLFW + a handwritten GL function loader), currently 
rendering a basic triangle. Next up: camera/transform pipeline, then orbital mechanics. Not playable yet, but I hope
:).

## Tech stack

C++20, OpenGL 4.6 core, GLFW, CMake, and Docker

## Installation

Requires [Docker](https://docs.docker.com/get-docker/).

```bash
sudo usermod -aG docker $USER   # optional: run docker without sudo (log out/in after)
```

## Usage

```bash
git clone https://github.com/DreGi0/apogee.git
cd apogee
./run.sh
```

Opens a window rendering the current OpenGL scene. `ESC` closes it.

## Contributing

Not accepting pull requests at this stage. This is a solo portfolio project. Bug reports and suggestions are welcome via Issues.

## License

[PolyForm Noncommercial License 1.0.0](https://polyformproject.org/licenses/noncommercial/1.0.0)