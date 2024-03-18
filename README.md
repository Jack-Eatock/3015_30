# Pirate
## Comp 3015 30% - Jack Eatock

Pirate is a small visual showcase of my experience using OpenGL. It presents you with a ship floating at sea in what appears to be an a surreal planet. I have used toon shading, along with edge detection to create a unique look to the experience.

### Tools:
- Visual Studio 2022
- Windows 10
- Blender for editing models
- Affinity Photo for tweaking textures
- Git for source controls

### Controls:
- WASD to move the camera
- CTRL orSpace to move the camera up or down
- Left click and move mouse to look around

### How to run the game:
Unzip the build.zip file, open up the folder and run the executable "Pirate.exe".
The project will then open in a small window.

##### From the Repo:
 1. Firstly download the repository.
 2. Within the project, navigate to x64/Debug and run the executable "COMP3016_CW2.exe"
 3. The game should start and you will already be able to interact! 

### Architecture:
- Skybox rendered in the background using a seperate shader
- Lighting is calculated using BlingPhong
- Fog has been created using mixing
- Toon Shading effect created by flooring the diffuse light values
- Multiple lights are present in the scene with their own positions and colours
- The ability to use a spot light is available, however not used in this scene
- Textures are mixed to create more unique visuals
- Normal maps are used to make simple low poly meshes appear far more detailed
- Objects within the scene are animated
- Edge detection is used to create a feint black outline to all objects
- Keyboard and mouse inputs are used to move the camera

## Dependencies:
####  In order to achieve a lot of the functionality required for this project. Quite a few libraries were used:
- Glad - A great library that manages function pointers for OpenGL.
- GLFW - Used for context and window management.
- GLM - A mathematics library.
- Stb - Used for generating and loading texturing.

### Video
You can find the video here: https://www.youtube.com/watch?v=5bG0rqyviw0&ab_channel=JackEatock

### External Assets:
- Skybox: [https://assetstore.unity.com/packages/2d/textures-materials/sky/farland-skies-cloudy-crown-60004](https://assetstore.unity.com/packages/2d/textures-materials/sky/farland-skies-cloudy-crown-60004 "https://assetstore.unity.com/packages/2d/textures-materials/sky/farland-skies-cloudy-crown-60004")
- Water textures: [https://3dtextures.me/2017/12/28/water-001/](https://3dtextures.me/2017/12/28/water-001/ "https://3dtextures.me/2017/12/28/water-001/")
- Water model: [https://sketchfab.com/3d-models/low-poly-water-f3471d6d2e4447fc8b5cf837c963c1de](https://sketchfab.com/3d-models/low-poly-water-f3471d6d2e4447fc8b5cf837c963c1de "https://sketchfab.com/3d-models/low-poly-water-f3471d6d2e4447fc8b5cf837c963c1de")
- Pirate ship model: [https://www.turbosquid.com/3d-models/ship-low-poly-3d-model-3d-model-2015662](https://www.turbosquid.com/3d-models/ship-low-poly-3d-model-3d-model-2015662 "https://www.turbosquid.com/3d-models/ship-low-poly-3d-model-3d-model-2015662")
