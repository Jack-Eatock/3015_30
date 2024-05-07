#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/gtc/matrix_transform.hpp>
#include "helper/torus.h"
#include "helper/teapot.h"
#include <glm/glm.hpp>
#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/cube.h"
#include "helper/skybox.h"
#include "helper/random.h"
#include "helper/grid.h"
#include "helper/particleutils.h"
#include<irrKlang.h>
using namespace irrklang;

#include"CamController.h"
class SceneBasic_Uniform : public Scene
{
private:
    void compile();
    void setMatrices(GLSLProgram& p);
    void setupFBO();

    void pass1();
    void pass2();
    void pass3();
    void pass4();
    float gauss(float,float);

    ISoundEngine* soundEngine;
    glm::vec3 keyboardMovement;
    float Time; float timeLost;
    bool running = true;
    // Particles
    GLSLProgram flatProg, particleProg;
    Random random;
    float deltaT;
    GLuint initVel, startTime, particles, nParticles;
    Grid grid;
    glm::vec3 emitterPos, emitterDir;
    float particleLifeTime;


    // Water
    std::unique_ptr<ObjMesh> boat, water;
    float tPrev;
    float angle;
    GLuint boatTexture, mossTexture, waterTextureDiffuse, waterTextureNormal, skyBoxTex,particlesTex;
    float waterPos;
    SkyBox skybox;
    CamController camera;
    GLuint fboHandle, fsQuad, renderTex, intermediateTex,  renderFBO, intermediateFBO;
    GLSLProgram prog, skyProg;

public:
    SceneBasic_Uniform();

    void initBuffers();
    float randFloat();
    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void CameraUpdate(glm::vec3 movement, glm::vec2 mouseMovement);

};

#endif // SCENEBASIC_UNIFORM_H
