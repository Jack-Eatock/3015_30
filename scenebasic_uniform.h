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

#include"CamController.h"
class SceneBasic_Uniform : public Scene
{
private:

    void compile();
    void setMatrices(GLSLProgram& p);
    void setupFBO();
    void pass1();
    void pass2();

    GLSLProgram prog, skyProg;
    Plane plane;
    std::unique_ptr<ObjMesh> boat, water;
    Teapot teapot;
    Torus torus;
    Cube cube;
    float tPrev;
    float angle;
    GLuint boatTexture, mossTexture, waterTextureDiffuse, waterTextureNormal, skyBoxTex;
    float waterPos;
    SkyBox skybox;
    CamController camera;
    GLuint fboHandle, fsQuad, renderTex;

public:
    SceneBasic_Uniform();


    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void CameraUpdate(glm::vec3 movement, glm::vec2 mouseMovement);

};

#endif // SCENEBASIC_UNIFORM_H
