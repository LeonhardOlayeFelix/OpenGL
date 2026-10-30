# OpenGL Renderer

A C++ OpenGL renderer built to learn real-time computer graphics, organised as a menu of interactive demo scenes. Each scene covers one technique, from drawing a first triangle to multi-light shadow mapping.

This project was the groundwork for my [SPH fluid simulation](https://github.com/LeonhardOlayeFelix/Fluid-Simulation-SPH).

![Menu](https://github.com/user-attachments/assets/1ba731ae-8dca-4822-85f7-6862d4966528)

## Highlights

- **Shadow mapping for three light types**: directional and spot lights use 2D depth maps; the point light uses an omnidirectional cube-map depth texture covering all six directions
- **Blinn-Phong lighting** with diffuse and specular maps and distance attenuation
- **Instanced rendering**: 1,000 asteroids drawn in a single draw call
- **Model loading** of textured meshes with Assimp
- **Geometry shaders**: an exploding-mesh effect and normal visualisation
- **Skybox** with environment reflection and refraction
- **Live tweaking**: every scene has an ImGui panel for adjusting lights, materials and settings at runtime

## Scenes

| Scene | What it demonstrates |
| --- | --- |
| Triangle | Vertex buffers, vertex arrays and a first shader |
| Textures | Texture loading and sampling |
| Transformations / Transformations in 3D | Model, view and projection matrices |
| Camera in 3D | A free-fly camera |
| Lighting | Phong lighting with materials and multiple light types |
| Assimp | Loading and rendering 3D models |
| Instancing / Asteroid | Instanced rendering of many objects |
| Depth Testing | The depth buffer and visualising depth |
| Blending | Transparency and draw order |
| Face Culling | Skipping back faces |
| Frame Buffer | Rendering to an off-screen texture |
| UBO Scene | Sharing matrices between shaders with uniform buffer objects |
| Geometry Shader | Generating geometry on the GPU |
| Cube Map | Skybox, reflection and refraction |
| Advanced Lighting | Blinn-Phong lighting with shadows from directional, point and spot lights |

## Screenshots

![Scene 1](https://github.com/user-attachments/assets/1d478338-032a-47de-8b7e-2ca761a20233)
![Scene 2](https://github.com/user-attachments/assets/aa1e4f29-ea2c-42b5-849c-77b2ae901ec1)
![Scene 3](https://github.com/user-attachments/assets/55b3b715-e5c8-451f-9f91-559933d909a6)

## Architecture

```
OpenGL/
├── src/
│   ├── Application.cpp      # Window setup, main loop, scene registration
│   ├── abstraction/         # Wrappers around OpenGL objects
│   │   ├── buffers/         # Vertex, index and uniform buffers; vertex arrays; framebuffers
│   │   ├── textures/        # 2D textures and cube maps
│   │   ├── lights/          # Directional, point and spot lights
│   │   ├── materials/       # Material maps and properties
│   │   ├── Camera, Mesh, Model, ShaderProgram, Renderer
│   │   └── ErrorHandling    # GLCall macro: checks for OpenGL errors after each call
│   ├── scene/               # One class per demo scene
│   └── vendor/              # GLM, Dear ImGui, stb_image
└── res/
    ├── shaders/             # Each .shader file holds its vertex, geometry and fragment stages
    ├── models/
    └── textures/
```

- **Resource ownership:** every class that holds a GPU resource is move-only and frees that resource in its destructor, so resources never leak or get deleted twice.
- **Scene system:** each scene inherits from a common `Scene` base class with update, render and GUI hooks. Scenes are registered by name with the menu, which creates them on demand.

## Controls

| Key | Action |
| --- | --- |
| W / A / S / D | Move |
| Space / Left Shift | Move up / down |
| I / J / K / L | Look around |
| Esc | Quit |

## Building

Windows only. Requires **Visual Studio 2026** (toolset v145) and **CMake**.

1. Clone with submodules:
   ```bash
   git clone --recursive https://github.com/LeonhardOlayeFelix/OpenGL.git
   ```
2. Build GLFW and Assimp in Debug mode, each into a `build` folder inside its own directory:
   ```bash
   cmake -S Dependencies/glfw -B Dependencies/glfw/build
   cmake --build Dependencies/glfw/build --config Debug

   cmake -S Dependencies/assimp -B Dependencies/assimp/build
   cmake --build Dependencies/assimp/build --config Debug
   ```
3. Copy `assimp-vc145-mtd.dll` from `Dependencies/assimp/build/bin/Debug` to the folder the executable runs from.
4. Open `OpenGL.slnx`, select the **x64** platform, and run. The working directory must be the `OpenGL/` project folder so that the `res/` paths resolve.

GLEW is included in the repo, so it doesn't need building.

## Acknowledgements

- [LearnOpenGL](https://learnopengl.com) by Joey de Vries: the main learning resource, and the source of the backpack, planet and rock models
- [GLFW](https://www.glfw.org), [GLEW](https://glew.sourceforge.net), [GLM](https://github.com/g-truc/glm), [Assimp](https://github.com/assimp/assimp), [Dear ImGui](https://github.com/ocornut/imgui), [stb_image](https://github.com/nothings/stb)
