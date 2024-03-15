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

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

SceneBasic_Uniform::SceneBasic_Uniform() :
	tPrev(0),
	plane(50.0f, 50.0f, 1, 1), 
	teapot(14, glm::mat4(1.0f)), 
	torus(1.75f * .75, .4f, 50,50)
{
	//mesh = ObjMesh::load("media/pig_triangulated.obj", true);
}

void SceneBasic_Uniform::initScene()
{
    compile();
	glEnable(GL_DEPTH_TEST);
	model = mat4(1.0f);
	view = glm::lookAt(vec3(5.0f, 5.0f, 7.5f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f,1.0f,0.0f));

	//model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(15.0f), vec3(0.0f, 1.0f, 0.0f));

	projection = mat4(1.0f);

	prog.setUniform("SpotLight.Colour", vec3(.9f));
	prog.setUniform("SpotLight.AmbientColour", vec3(.5f));
	prog.setUniform("SpotLight.Exponent", 100.0f);
	prog.setUniform("SpotLight.Cuttoff", glm::radians(15.0f));

	//float x, z;
	//for (int i = 0; i < 3; i++) {
	//	std::stringstream name;
	//	name << "Lights[" << i << "].Position";
	//	x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
	//	z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
	//	prog.setUniform(name.str().c_str(), view * glm::vec4(x,1.2f, z + 1.0f, 1.0f));
	//}
	//
	//prog.setUniform("Lights[0].Colour", vec3(0.0f, 0.0f, 0.8f));
	//prog.setUniform("Lights[1].Colour", vec3(0.0f, 0.8f, 0.0f));
	//prog.setUniform("Lights[2].Colour", vec3(0.8f, 0.0f, 0.0f));

	//prog.setUniform("Lights[0].AmbientColour", vec3(0.0f, 0.0f, 0.2f));
	//prog.setUniform("Lights[1].AmbientColour", vec3(0.0f, 0.2f, 0.0f));
	//prog.setUniform("Lights[2].AmbientColour", vec3(0.2f, 0.0f, 0.0f));
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
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

	angle += 0.25 * deltaT;
	if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	vec4 lightPos = vec4(10.0f * cos(angle), 10.0f, 10.0f * sin(angle), 1.0f);
	prog.setUniform("SpotLight.Position", vec3(view*lightPos));
	mat3 normalMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
	prog.setUniform("SpotLight.Direction", normalMatrix * vec3(-lightPos));


	// teaPot

	prog.setUniform("Material.Diffuse", vec3(0.2f, 0.55f, .9f));
	prog.setUniform("Material.Specular", vec3(0.95f, 0.95f, .95f));
	prog.setUniform("Material.Ambient", vec3(0.2f * .3f, 0.55f * .3f, .9f * .3f));
	prog.setUniform("Material.Shininess", 100.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, .0f, -2.0f));
	model = glm::rotate(model, glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
	setMatrices();
	teapot.render();

	// Torus

	prog.setUniform("Material.Diffuse", vec3(0.2f, 0.55f, .9f));
	prog.setUniform("Material.Specular", vec3(0.95f, 0.95f, .95f));
	prog.setUniform("Material.Ambient", vec3(0.2f * .3f, 0.55f * .3f, .9f * .3f));
	prog.setUniform("Material.Shininess", 100.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(-1.0f, .75f, 3.0f));
	model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
	setMatrices();
	torus.render();

	// Plane

	prog.setUniform("Material.Diffuse", vec3(0.7f, 0.7f, .7f));
	prog.setUniform("Material.Specular", vec3(0.9f, 0.9f, .9f));
	prog.setUniform("Material.Ambient", vec3(0.2f, 0.2f, .2f));
	prog.setUniform("Material.Shininess", 180.0f);

	model = mat4(1.0f);

	setMatrices();
	plane.render();

}

void SceneBasic_Uniform::resize(int w, int h)
{
	glViewport(0, 0, w, h);
    width = w;
    height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h, .3f, 100.0f);
}

void SceneBasic_Uniform::setMatrices()
{
	mat4 mv = view*model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("ModelViewPerspective", projection*mv);
}