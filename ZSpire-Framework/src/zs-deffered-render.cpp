#ifdef _WIN32
#include <glew.h>
#endif

#ifdef __linux__
#include <GL/glew.h>
#endif

#include <vector>

#include "../includes/zs-base-structs.h"

#include "../includes/zs-camera.h"

#include "../includes/zs-light.h"

#include "../includes/zs-shader.h"

#include "../includes/zs-mesh.h"

#include "../includes/zs-texture.h"

#include "../includes/zs-game-object.h"

#include "../includes/zs-scene.h"

#include "../includes/zs-deffered-render.h"

ZSpire::Shader* deff_obj_shader;
ZSpire::Shader* light_shader;


unsigned int SCR_WIDTH;
unsigned int SCR_HEIGHT;

unsigned int gBufferFBO;

unsigned int gBufferDiffuseTextureBuffer;
unsigned int gBufferNormalTextureBuffer;
unsigned int gBufferPositionTextureBuffer;

unsigned int gBufferDepthBuffer;

bool isActive = false;

ZSpire::ZSRENDERRULE renderRulesDef;
GLbitfield clearModeDef = GL_COLOR_BUFFER_BIT;

void ZSpire::DefferedRender::setDefferedShaders(Shader* obj_shader, Shader* lighting_shader) {
	deff_obj_shader = obj_shader;
	light_shader = lighting_shader;
}

void ZSpire::DefferedRender::setRenderRule(ZSRENDERRULE rule) {
	renderRulesDef = rule;

	if (rule.depthTest == true) {
		clearModeDef = clearModeDef | GL_DEPTH_BUFFER_BIT;
	}
}

void ZSpire::DefferedRender::RenderScene(Scene* scene) {

	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	glClear(clearModeDef);
	
	if (renderRulesDef.depthTest == true) {
		glEnable(GL_DEPTH_TEST);
	}
	if (renderRulesDef.cullFaces == true) {
		glEnable(GL_CULL_FACE);
	}

	deff_obj_shader->Use();

	Camera::setCameraMode(CAMERA_MODE_SCENE);

	deff_obj_shader->updateCamera();

	
	for (unsigned int obj = 0; obj < scene->getObjectsCount(); obj++) {
		Transform * tr = scene->getObjectAt(obj)->getTransform();

		deff_obj_shader->setTransform(tr);

		scene->getObjectAt(obj)->Draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	light_shader->Use();
	light_shader->setUniformInt("lights_amount", scene->getLightsCount());
	for (unsigned int light = 0; light < scene->getLightsCount(); light++) {
		light_shader->setLight(light, scene->getLightAt(light));
	}

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D ,gBufferDiffuseTextureBuffer);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, gBufferNormalTextureBuffer);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, gBufferPositionTextureBuffer);

	glDisable(GL_DEPTH_TEST);
	
	glDisable(GL_CULL_FACE);
	getPlaneMesh2D()->Draw();
}

void ZSpire::DefferedRender::set_gBufferSize(unsigned int WIDTH, unsigned int HEIGHT) {
	SCR_WIDTH = WIDTH;
	SCR_HEIGHT = HEIGHT;
}

void ZSpire::DefferedRender::Init_gBuffer() {
	isActive = true;

	glGenFramebuffers(1, &gBufferFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);

	glGenRenderbuffers(1, &gBufferDepthBuffer);

	//Generate Diffuse Albedo texture
	glGenTextures(1, &gBufferDiffuseTextureBuffer);
	glBindTexture(GL_TEXTURE_2D, gBufferDiffuseTextureBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gBufferDiffuseTextureBuffer, 0);

	//Generate Normal texture
	glGenTextures(1, &gBufferNormalTextureBuffer);
	glBindTexture(GL_TEXTURE_2D, gBufferNormalTextureBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gBufferNormalTextureBuffer, 0);

	glGenTextures(1, &gBufferPositionTextureBuffer);
	glBindTexture(GL_TEXTURE_2D, gBufferPositionTextureBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gBufferPositionTextureBuffer, 0);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glBindRenderbuffer(GL_RENDERBUFFER, gBufferDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gBufferDepthBuffer);
}

void ZSpire::DefferedRender::destroy_gBuffer() {
	if (isActive == true) {
		glDeleteFramebuffers(1, &gBufferFBO);
		glDeleteRenderbuffers(1, &gBufferDepthBuffer);
		//Remove all textures
		glDeleteTextures(1, &gBufferDiffuseTextureBuffer);
		glDeleteTextures(1, &gBufferNormalTextureBuffer);
		glDeleteTextures(1, &gBufferPositionTextureBuffer);
	}
}

void ZSpire::DefferedRender::resize_gBuffer(unsigned int W, unsigned int H) {
	//Destroy gBuffer Textures first
	destroy_gBuffer();
	
	SCR_WIDTH = W;
	SCR_HEIGHT = H;
	if(isActive == true)
	Init_gBuffer();
}