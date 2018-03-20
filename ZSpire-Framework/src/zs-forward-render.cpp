#include <vector>

#include "../includes/zs-math.h"

#include "../includes/zs-transform.h"

#include "../includes/zs-camera.h"

#include "../includes/zs-shader.h"

#include "../includes/zs-mesh.h"

#include "../includes/zs-texture.h"

#include "../includes/zs-game-object.h"

#include "../includes/zs-light.h"

#include "../includes/zs-scene.h"

#include "../includes/zs-forward-render.h"

ZSpire::Shader* object_shader;

void ZSpire::setForwardObjectShader(Shader* shader) {
	object_shader = shader;
}

void ZSpire::RenderSceneForward(Scene* scene) {
	object_shader->Use();

	object_shader->updateCamera();

	for (unsigned int obj = 0; obj < scene->getObjectsCount(); obj ++) {
		Transform * tr = scene->getObjectAt(obj)->getTransform();
	
		object_shader->setTransform(tr);
		
		scene->getObjectAt(obj)->Draw();
	}
}