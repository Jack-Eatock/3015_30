#include "scenebasic_uniform.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>
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

int numParticles = 5;
// Starting positions
vec3 particlePositions[5] = 
{ 
	vec3(10.0f, 2.3f, -6.0),
	vec3(30.0f, 2.3f, -2.0),
	vec3(50.0f, 2.3f, -0.0),
	vec3(65.0f, 2.3f, 4.0),
	vec3(23.0f, 2.3f, 6.0),
};

float particleSpeeds[5] =
{
	5.5, 4, 4.5, 5, 5
};


SceneBasic_Uniform::SceneBasic_Uniform() :
	tPrev(0),
	skybox(100.0f),
	camera(),
	particleLifeTime(300.0f),
	nParticles(80000),
	emitterPos(0, 0, 0),
	emitterDir(0, 1, 0)

{
	boat = ObjMesh::load("media/Boat.obj", true);
	water = ObjMesh::load("media/Low Poly Water.obj", true);
}

void SceneBasic_Uniform::initBuffers()
{
	glGenBuffers(1, &initVel);
	glGenBuffers(1, &startTime);
	int size = nParticles * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size * 3, 0, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);

	glm::mat3 emitterBasis = ParticleUtils::makeArbitraryBasis(emitterDir);
	vec3 v(0.0f);

	float velocity, theta, phi;
	std::vector<GLfloat> data(nParticles * 3);

	for (uint32_t i = 0; i < nParticles; i++)
	{
		theta = glm::mix(0.0f, glm::pi<float>() / 20.0f, randFloat());
		phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());
		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.x = sinf(theta) * sinf(phi);
		velocity = glm::mix(1.25f, 1.5f, randFloat());
		v = glm::normalize(emitterBasis * v) * velocity;
		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size*3, data.data());
	float rate = particleLifeTime / nParticles;

	for(int i= 0; i < nParticles; i++)
	{
		data[i] = rate * i;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenVertexArrays(1, &particles);
	glBindVertexArray(particles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glBindVertexArray(0);
}

float SceneBasic_Uniform::randFloat()
{
	return random.nextFloat();
}

void SceneBasic_Uniform::initScene()
{
	glDebugMessageControl(
		GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE);

	glDebugMessageControl(
		GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, NULL, GL_TRUE);

    compile();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	
	// Particle properties
	initBuffers();
	glActiveTexture(GL_TEXTURE0);
	particlesTex = Texture::loadTexture("media/particles/bluewater.png");
	particleProg.use();
	particleProg.setUniform("ParticleTex", 0);
	particleProg.setUniform("ParticleLifeTime", particleLifeTime);
	particleProg.setUniform("ParticleSize", 0.4f);
	particleProg.setUniform("Gravity", vec3(0.0f, -.4f, .0f));
	particleProg.setUniform("EmitterPos", emitterPos);


	flatProg.use();
	flatProg.setUniform("Color", glm::vec4(.4f, .4f, .4f, 1.0f));
	prog.use();

	// Textures
	boatTexture = Texture::loadTexture("media/texture/Pallete.png");
	mossTexture = Texture::loadTexture("media/texture/moss.png");
	waterTextureDiffuse = Texture::loadTexture("media/texture/Water/Water.png");
	waterTextureNormal = Texture::loadTexture("media/texture/Water/Water_001_NORM.jpg");
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
	prog.setUniform("Fog.MaxDist", 70.0f);
	prog.setUniform("Fog.MinDist", 1.0f);
	prog.setUniform("Fog.Color", vec3(.16f, 0.1f, 0.1f));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, boatTexture);
	setupFBO();

	// Particles
	GLfloat verts[] = {
		-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	};

	GLfloat tc[] = {
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	};


	unsigned int handle[2];
	glGenBuffers(2, handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

	glGenVertexArrays(1, &fsQuad);
	glBindVertexArray(fsQuad);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0,0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Edge detection
	prog.setUniform("EdgeThreshold", 0.05f);

	// Blur
	float weights[5], sum, sigma2 = 3.0f;
	weights[0] = gauss(0, sigma2);
	sum = weights[0];
	for (int i = 0; i < 5; i++) {
		weights[i] = gauss(float(i), sigma2);
		sum += 1.5 * weights[i];
	}

	for (int i = 1; i < 5; i++) {
		std::stringstream uniName;
		uniName << "BlurWeight[" << i << "]";
		float val = weights[i] / sum;
		prog.setUniform(uniName.str().c_str(), val);
	}
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
		particleProg.compileShader("shader/Particles.vert");
		particleProg.compileShader("shader/Particles.frag");
		particleProg.link();
		particleProg.use();
		flatProg.compileShader("shader/solid.frag");
		flatProg.compileShader("shader/solid.vert");
		flatProg.link();
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
	deltaT = t - tPrev;  // Calculates time since last frame
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
	Time = t;
}

void SceneBasic_Uniform::render()
{
	pass1();

	//glFlush();
	pass2();
	pass3();
	pass4();
}

void SceneBasic_Uniform::resize(int w, int h)
{
	glViewport(0, 0, w, h);
    width = w;
    height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h, .3f, 100.0f);
}


float boatPosOffset;
float maxOffset = 6;
void SceneBasic_Uniform::CameraUpdate(glm::vec3 movement, glm::vec2 mouseMovement)
{
	// Move boat
	boatPosOffset += movement.x * .3f;
	if (boatPosOffset > maxOffset)
		boatPosOffset = maxOffset;
	else if (boatPosOffset < -maxOffset)
		boatPosOffset = -maxOffset;
}

void SceneBasic_Uniform::setMatrices(GLSLProgram &p)
{
	mat4 mv = view*model;
	p.use();
	p.setUniform("ModelViewMatrix", mv);
	p.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	p.setUniform("ModelViewPerspective", projection*mv);
	p.setUniform("model", model);
	p.setUniform("Projection", projection);
}

void SceneBasic_Uniform::setupFBO()
{
	glGenFramebuffers(1, &fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

	glGenTextures(1, &renderTex);
	glBindTexture(GL_TEXTURE_2D, renderTex);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER, depthBuf);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Frame buffer is complete!" << std::endl;
	}
	else {
		std::cout << "Frame buffer error! : " << result << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Blur
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

	glGenTextures(1, &intermediateTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, intermediateTex);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediateTex, 0);

	glDrawBuffers(1, drawBuffers);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void SceneBasic_Uniform::pass1()
{
	prog.use();
	prog.setUniform("Pass", 1);

	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);

	view = glm::lookAt(camera.position, camera.position + camera.Orientation, camera.Up);
	projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.3f, 100.0f);

	//vec3 cameraPos(3.0f, 1.5f, 3.0f);
	//view = glm::lookAt(cameraPos, vec3(0.0f, 1.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	//projection = glm::perspective(glm::radians(60.0f), (float)width / height, .03f, 100.0f);

	// Sky box
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	skyProg.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTex);
	model = mat4(1.0f);
	setMatrices(skyProg);
	skybox.render();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glEnable(GL_DEPTH_TEST);



	// Boat
	prog.use();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, boatTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mossTexture);

	prog.setUniform("Material.Diffuse", vec3(.2f, .2f, .2f));
	prog.setUniform("Material.Specular", vec3(0.95f, 0.95f, .95f));
	prog.setUniform("Material.Ambient", vec3(0.2f * .3f, 0.55f * .3f, .9f * .3f));
	prog.setUniform("Material.Shininess", 100.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(-42.0f, 9.3f, 0.0f + boatPosOffset));
	//model = glm::rotate(model, glm::radians(110.0f + (angle * 2.0f)), vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f + (angle * 1.5f)), vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-6.0f + (angle * 4.0f)), vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-.5f + (angle * 1.2f)), vec3(0.0f, 0.0f, 1.0f));
	setMatrices(prog);
	boat->render();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);


	// Water
	prog.use();
	prog.setUniform("Time", Time);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, waterTextureDiffuse);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, waterTextureNormal);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	prog.setUniform("Material.Diffuse", vec3(0.2, 0.5f, .2f));
	prog.setUniform("Material.Specular", vec3(0.f, 0.5f, .2f));
	prog.setUniform("Material.Ambient", vec3(0.05f, 0.05f, .05f));
	prog.setUniform("Material.Shininess", 0.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(-5.0f - (25 ), -0.5f, -5.0f + (4)));

	setMatrices(prog);

	water->render();
	prog.setUniform("Time", 0.0f);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);


	// Particle Effect
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, particlesTex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < numParticles; i++)
	{
		model = mat4(1.0f);

		
		vec3 pos = vec3(particlePositions[i].x - (deltaT * particleSpeeds[i] ), 2.3f, particlePositions[i].z);
	
		srand(time(0));
	
		if (pos.x < -55) {
			int max = 30, min = 5;
			int val = (rand() % (max - min)) + min;
			pos.x = val;
		
			//max = 6, min = 4;
			//particleSpeeds[i] = (rand() % (max - min)) + min;

			max = 6, min = -6;
			particlePositions[i].z = (rand() % (max - min)) + min;
		}

		particlePositions[i] = pos;
		model = glm::translate(model, particlePositions[i]);
		flatProg.use();
		setMatrices(flatProg);
		grid.render();
		glDepthMask(GL_FALSE);
		particleProg.use();
		particleProg.setUniform("Time", Time);
		setMatrices(particleProg);
		glBindVertexArray(particles);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
		glBindVertexArray(0);
	}
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	particleProg.setUniform("Time", 0.0f);
	prog.use();

}

void SceneBasic_Uniform::pass2()
{
	prog.setUniform("Pass", 2);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTex);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	
	model = mat4(1.0f);
	view = mat4(1.0f);
	projection = mat4(1.0f);

	setMatrices(prog);

	glBindVertexArray(fsQuad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}


void SceneBasic_Uniform::pass3()
{
	prog.setUniform("Pass", 3);

	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTex);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);

	model = mat4(1.0f);
	view = mat4(1.0f);
	projection = mat4(1.0f);

	setMatrices(prog);

	glBindVertexArray(fsQuad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glBindVertexArray(0);
}

void SceneBasic_Uniform::pass4()
{
	prog.setUniform("Pass", 4);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, intermediateTex);
	glClear(GL_COLOR_BUFFER_BIT);

	model = mat4(1.0f);
	view = mat4(1.0f);
	projection = mat4(1.0f);

	setMatrices(prog);

	glBindVertexArray(fsQuad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glBindVertexArray(0);
}


float SceneBasic_Uniform::gauss(float x, float sigma2)
{
	double coeff = 1.0 / (glm::two_pi<double>() * sigma2);
	double exponent = -(x * x) / ( 2.0 * sigma2 );
	return (float)(coeff * exp(exponent));
}

