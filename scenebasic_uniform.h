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

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog, skyProg;
    void compile();
    void setMatrices(GLSLProgram& p);
    Plane plane;
    std::unique_ptr<ObjMesh> boat, water;
    Teapot teapot;
    Torus torus;
    Cube cube;
    float tPrev;
    float angle;
    GLuint boatTexture, mossTexture, waterTextureDiffuse, skyBoxTex;
    float waterPos;
    SkyBox skybox;

public:
    SceneBasic_Uniform();


    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
