

#include "../includes/zs-math.h"

#include "../includes/zs-camera.h"


ZSPROJECTIONTYPE projection_type;
float FOV;
float ZNearPlane;
float ZFarPlane;

float CAMERA_PROJ_WIDTH;
float CAMERA_PROJ_HEIGHT;

ZSVECTOR3 up;
ZSVECTOR3 camera_pos;
ZSVECTOR3 camera_target;

ZSMATRIX4x4 PROJECTION;
ZSMATRIX4x4 VIEW;

void ZSpire::InitializeCamera(){
	FOV = 45.0f;
	ZNearPlane = 0.3f;
	ZFarPlane = 1000.0f;
	projection_type = CAMERA_PROJECTION_ORTHOGRAPHIC;

	CAMERA_PROJ_WIDTH = 640;
	CAMERA_PROJ_HEIGHT = 480;
}

void ZSpire::setCameraProjectionType(ZSPROJECTIONTYPE type) {
	projection_type = type;
}

void ZSpire::setCameraZPlanes(float znear, float zfar) {
	ZNearPlane = znear;
	ZFarPlane = zfar;
}

void ZSpire::setCameraFOV(float nfov) {
	FOV = nfov;
}

void ZSpire::updateCameraMatrix() {
	if (projection_type == CAMERA_PROJECTION_ORTHOGRAPHIC) {
		PROJECTION = getOrthogonal(0, CAMERA_PROJ_WIDTH, 0, CAMERA_PROJ_HEIGHT);
	}
	else {
		PROJECTION = getPerspective(FOV, CAMERA_PROJ_WIDTH / CAMERA_PROJ_HEIGHT, ZNearPlane, ZFarPlane);
	}
}

void ZSpire::setCameraProjectionResolution(float WIDTH, float HEIGHT) {
	CAMERA_PROJ_WIDTH = WIDTH;
	CAMERA_PROJ_HEIGHT = HEIGHT;
}