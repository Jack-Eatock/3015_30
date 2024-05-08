
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
- If you lose, the game will pause before restarting. 

### How to run the game:
Unzip the build.zip file, open up the folder and run the executable "ArghAGhost.exe".
The project will then open in a small window.

##### From the Git Repo:
 - Firstly download the repository.
 - Within the project, navigate to x64/Debug and run the executable "ArghAGhost.exe"
 
### Advanced Features:
 #### Gaussian Blur 
Gaussian Blur was used to create a gloomy affect for the game, in an attempt to make the scene appear more spooky. Two extra render passes were added, one to blur the pixels on the horizontal axis and the second to blur the pixels on the vertical axis. It  is done like this to save on execution time, apply blur to each pixel on both axis in one pass would be increadibly taxing on the CPU. The blur applied to hte pixel is dependant on the settings that are defined in the "scenebasic_uniform.cpp",  "InitScene()" function. 

#### Particle System
Particle Systems were used to create an interesting visual affect for the Ghost characters in the game. The particle system has its own shaders "Particles.frag" and "Particles.vert". Each individual particle is managed by these shaders, the vertex shader controls their position taking into account their emmiter velocity, direction, gravity and the time passed, and the frag shader applies a colour based on the texture set. These values are all set when the program first initiates, setting the uniforms for the various properties of the particle system and can be adjusted very easily. A Vec3 array stores the positions of all of the particle systems (Ghosts) active in the scene, within the render loop they are moved at a constant rate that is scaled with delta time to keep it frame independant. When the ghosts x coord reaches a certain value they are reset with a slightly random position and speed creating a constant wave of random ghosts.

#### Surface Animation
Previously to give the impression of waves, the water's mesh was translated at a constant rate which caused interesting lighting affects due to the textures normal map and cartoon shader applied. This isn't performant and can look unimpressive if the user sees where the mesh starts and finishes.  To improve this the mesh is no longer moved, instead the texture is moved using surface animations. This applies the same interesting lighting affects due to the normal map being shifted, but without the cost of moving the entire mesh. This has been implemented in the "basic_uniform_vert" shader, which now takes in time and adjusts the vertex position gradually.

#### Gamification

 - Music and Sound effects using IrrKlang.
 - Ghosts (that are particles) attack the ship. If they touch the ship, it is destroyed, and the game starts over.
 - Player can move the ship using WASD to avoid hitting the ghosts.
 - The ghosts respawn continuously with random positions and speeds, making the game endless.
 - When the player hits a ghost and loses the other ghosts despawn. The game then waits for about 1 second to show the collision before restarting.

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
You can find the video here:  https://www.youtube.com/watch?v=_A0EfwX1JLA&ab_channel=JackEatock

### Github
You can find the github repository here: https://github.com/Jack-Eatock/Argh-a-Ghost

### External Assets:
- Skybox: [https://assetstore.unity.com/packages/2d/textures-materials/sky/farland-skies-cloudy-crown-60004](https://assetstore.unity.com/packages/2d/textures-materials/sky/farland-skies-cloudy-crown-60004 "https://assetstore.unity.com/packages/2d/textures-materials/sky/farland-skies-cloudy-crown-60004")
- Water textures: [https://3dtextures.me/2017/12/28/water-001/](https://3dtextures.me/2017/12/28/water-001/ "https://3dtextures.me/2017/12/28/water-001/")
- Water model: [https://sketchfab.com/3d-models/low-poly-water-f3471d6d2e4447fc8b5cf837c963c1de](https://sketchfab.com/3d-models/low-poly-water-f3471d6d2e4447fc8b5cf837c963c1de "https://sketchfab.com/3d-models/low-poly-water-f3471d6d2e4447fc8b5cf837c963c1de")
- Pirate ship model: [https://www.turbosquid.com/3d-models/ship-low-poly-3d-model-3d-model-2015662](https://www.turbosquid.com/3d-models/ship-low-poly-3d-model-3d-model-2015662 "https://www.turbosquid.com/3d-models/ship-low-poly-3d-model-3d-model-2015662")
- Music: https://pixabay.com/music/folk-dread-pirate-roberts-sea-shanty-dance-edm-soundtrack-153022/
