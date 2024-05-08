
# Argh a Ghost!
## Comp 3015 70% - Jack Eatock

"Argh a Ghost!" - Play as a lost pirate in the depths of the Pacific Ocean and survive the horrors of the haunted seas.

### Tools:
- Visual Studio 2022
- Windows 10
- Blender for editing models
- Affinity Photo for tweaking textures
- Git for source control

### Controls:
- A or D to move the ship.

### How to run the game:
Unzip the build.zip file, open up the folder and run the executable "Pirate.exe".
The project will then open in a small window.

##### From the Repo:
 - Firstly download the repository.
 - Within the project, navigate to x64/Debug and run the executable "Pirate.exe"
 - The game should start and you will already be able to interact! 
 
#### Advanced Features:
 - List item

#### Architecture:
- Skybox rendered in the background using a separate shader and cube map. Using a cube map is more efficient as it avoids having to load 6 individual textures for each face. Also, it is super handy because of how easily it can be sampled using a direction, ideal for a camera.
- Lighting is calculated using BlingPhong.
- Fog has been created using mixing. Depending on the depth of the vertex point, the fog will be mixed over the colour of the pixel.
- Toon Shading effect created by flooring the diffuse light values.
- Multiple lights are present in the scene with their own positions and colours. There is an array of lights in the frag shader. When rendering the lighting it iterates over each light in the array adding up their lighting values.
- The ability to use a spotlight is available, however, they are not used in this scene because they did not have a good situation to be used.
- Textures are mixed to create more unique visuals, for example, the ship has 2 textures a diffuse and a moss texture that is overlayed using its alpha channel.
- Normal maps are used to make simple low poly meshes appear far more detailed such as the water. It makes the mesh look more complex by increasing the level of detail produced by the lighting.
- Objects within the scene are animated, such as the boat and water. This was done by keeping track of the delta time, the time between the last frame and current, this was then used to adjust values and move objects while remaining frame consistent. Not using delta time would cause different machines to render the objects at different speeds.
- Edge detection is used to create a feint black outline for all objects, this was achieved by using two render passes, one renders the scene and the other takes that scene and draws lines around the edges before outputting it to the screen.
- Keyboard and mouse inputs are used to move the camera. The scene runner checks for inputs and passes in a vector3 movement and a vector 2 mouse movement to the scene. The scene then uses these values to move the camera as expected.

#### Dependencies:
 In order to achieve a lot of the functionality required for this project. Quite a few libraries were used:
- Glad - A great library that manages function pointers for OpenGL.
- GLFW - Used for context and window management.
- GLM - A mathematics library.
- Stb - Used for generating and loading texturing.
- IrrKlang - Music and Sound effects

### Video
You can find the video here: 

### External Assets:
- Skybox: [https://assetstore.unity.com/packages/2d/textures-materials/sky/farland-skies-cloudy-crown-60004](https://assetstore.unity.com/packages/2d/textures-materials/sky/farland-skies-cloudy-crown-60004 "https://assetstore.unity.com/packages/2d/textures-materials/sky/farland-skies-cloudy-crown-60004")
- Water textures: [https://3dtextures.me/2017/12/28/water-001/](https://3dtextures.me/2017/12/28/water-001/ "https://3dtextures.me/2017/12/28/water-001/")
- Water model: [https://sketchfab.com/3d-models/low-poly-water-f3471d6d2e4447fc8b5cf837c963c1de](https://sketchfab.com/3d-models/low-poly-water-f3471d6d2e4447fc8b5cf837c963c1de "https://sketchfab.com/3d-models/low-poly-water-f3471d6d2e4447fc8b5cf837c963c1de")
- Pirate ship model: [https://www.turbosquid.com/3d-models/ship-low-poly-3d-model-3d-model-2015662](https://www.turbosquid.com/3d-models/ship-low-poly-3d-model-3d-model-2015662 "https://www.turbosquid.com/3d-models/ship-low-poly-3d-model-3d-model-2015662")
- Music: https://pixabay.com/music/folk-dread-pirate-roberts-sea-shanty-dance-edm-soundtrack-153022/
