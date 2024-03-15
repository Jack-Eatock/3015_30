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

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;
    void compile();
    void setMatrices();
    Plane plane;
    std::unique_ptr<ObjMesh> mesh;
    Teapot teapot;
    Torus torus;
    float tPrev;
    float angle;

public:
    SceneBasic_Uniform();


    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H