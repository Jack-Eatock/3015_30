#include "scenebasic_uniform.h"
#include <cstdio>
#include <cstdlib>
#include <string>
using std::string;

#include <sstream>

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include "helper/texture.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;
bool forward = true;

SceneBasic_Uniform::SceneBasic_Uniform() :
	tPrev(0),
	plane(50.0f, 50.0f, 1, 1),
	teapot(14, glm::mat4(1.0f)),
	torus(1.75f * .75, .4f, 50, 50),
	cube(),
	skybox(100.0f),
	camera()
{
	boat = ObjMesh::load("media/Boat.obj", true);
	water = ObjMesh::load("media/Low Poly Water.obj", true);
}

void SceneBasic_Uniform::initScene()
{
	glDebugMessageControl(
		GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE);

	glDebugMessageControl(
		GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, NULL, GL_TRUE);

    compile();
	glEnable(GL_DEPTH_TEST);
	model = mat4(1.0f);
	view = glm::lookAt(vec3(5.0f, 5.0f, 7.5f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f,1.0f,0.0f));
	projection = mat4(1.0f);

	// Textures
	boatTexture = Texture::loadTexture("media/texture/Pallete.png");
	mossTexture = Texture::loadTexture("media/texture/moss.png");
	waterTextureDiffuse = Texture::loadTexture("media/texture/Water/Water_001_COLOR.jpg");
	skyBoxTex = Texture::loadCubeMap("media/texture/cube/Test/CloudyCrown_Daybreak");
	
	// Spot Light
	prog.setUniform("SpotLight.Colour", vec3(.9f));
	prog.setUniform("SpotLight.AmbientColour", vec3(.5f));
	prog.setUniform("SpotLight.Exponent", 100.0f);
	prog.setUniform("SpotLight.Cuttoff", glm::radians(15.0f));

	// Lights
	prog.setUniform("Lights[0].Position", vec3(5.0f, 3.0f, 7.5f));
	prog.setUniform("Lights[1].Position", vec3(9.0f, 5.0f, 19.0f));
	prog.setUniform("Lights[2].Position", vec3(-12.0f, 5.0f, 7.0f));

	prog.setUniform("Lights[0].Colour", vec3(.7f));
	prog.setUniform("Lights[1].Colour", vec3(0.3f));
	prog.setUniform("Lights[2].Colour", vec3(.3f));

	prog.setUniform("Lights[0].AmbientColour", vec3(.1f));
	prog.setUniform("Lights[1].AmbientColour", vec3(.1f));
	prog.setUniform("Lights[2].AmbientColour", vec3(.1f));

	// Fog
	prog.setUniform("Fog.MaxDist", 19.0f);
	prog.setUniform("Fog.MinDist", 1.0f);
	prog.setUniform("Fog.Color", vec3(.16f, 0.1f, 0.1f));
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		skyProg.compileShader("shader/skybox.vert");
		skyProg.compileShader("shader/skybox.frag");
		skyProg.link();
		skyProg.use();
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
	//update your angle here
	float deltaT = t - tPrev;  // Calculates time since last frame
	if (tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;

	waterPos += .02 * deltaT;
	
	if (waterPos >= 1.0f)
		waterPos = 0.0f;
	
	if (forward) 
	{
		angle += .2f * deltaT;
		if (angle >= 1.0f) {
			angle = 1.0f;
			forward = false;
		}
	}
	else 
	{
		angle -= .2f * deltaT;
		if (angle <= -1.0f) {
			angle = -1.0f;
			forward = true;
		}

	}
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//vec4 lightPos = vec4(10.0f * cos(angle), 10.0f, 10.0f * sin(angle), 1.0f);
	//prog.setUniform("SpotLight.Position", vec3(view*lightPos));
	//mat3 normalMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
	//prog.setUniform("SpotLight.Direction", normalMatrix * vec3(-lightPos));

	// Sky box

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTex);
	model = mat4(1.0f);
	setMatrices(skyProg);
	skybox.render();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// Boat

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, boatTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mossTexture);

	prog.setUniform("Material.Diffuse", vec3(.2f, .2f, .2f));
	prog.setUniform("Material.Specular", vec3(0.95f, 0.95f, .95f));
	prog.setUniform("Material.Ambient", vec3(0.2f * .3f, 0.55f * .3f, .9f * .3f));
	prog.setUniform("Material.Shininess", 100.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(-5.0f, 9.3f, -5.0f));
	model = glm::rotate(model, glm::radians(110.0f + (angle * 2.0f)), vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-6.0f + (angle * 4.0f) ), vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-.5f + (angle * 1.2f)), vec3(0.0f, 0.0f, 1.0f));
	setMatrices(prog);
	boat->render();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);


	// Water

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTextureDiffuse);

	prog.setUniform("Material.Diffuse", vec3(0.2, 0.5f, .2f));
	prog.setUniform("Material.Specular", vec3(0.f, 0.5f, .2f));
	prog.setUniform("Material.Ambient", vec3(0.05f, 0.05f, .05f));
	prog.setUniform("Material.Shininess", 0.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(-5.0f - (25 * waterPos), -0.5f, -5.0f + (4 * waterPos)));
	setMatrices(prog);
	
	water->render();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SceneBasic_Uniform::resize(int w, int h)
{
	glViewport(0, 0, w, h);
    width = w;
    height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h, .3f, 100.0f);
}

vec3 Position;

void SceneBasic_Uniform::CameraUpdate(glm::vec3 movement, glm::vec2 mouseMovement)
{
	camera.Inputs(movement, mouseMovement);

	//Position += cameraMovement;
	view = glm::lookAt(camera.position, camera.position + camera.Orientation, camera.Up);

}


void SceneBasic_Uniform::setMatrices(GLSLProgram &p)
{
	mat4 mv = view*model;
	p.use();
	p.setUniform("ModelViewMatrix", mv);
	p.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	p.setUniform("ModelViewPerspective", projection*mv);

	p.setUniform("model", model);
}
