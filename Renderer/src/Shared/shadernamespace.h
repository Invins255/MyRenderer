#ifndef SHADERNAMESPACE_H
#define SHADERNAMESPACE_H

//DirectionalLight
#define DIRLIGHT_DIRECTION	"uDirLightDir"
#define DIRLIGHT_COLOR		"uDirLightColor"
#define DIRLIGHT_INTENSITY	"uDirLightIntensity"
#define LIGHTSPACE_MATRIX	"uLightSpaceMatrix"

//camera
#define CAMERA_POSITION		"uCameraPos"

//matrix
#define MODEL_MATRIX		"uModelMatrix"
#define VIEW_MATRIX			"uViewMatrix"
#define PROJECTION_MATRIX	"uProjectionMatrix"

//material
#define MATERIAL_AMBIENT	"material.ambient"
#define MATERIAL_DIFFUSE	"material.diffuse"
#define MATERIAL_SPECULAR	"material.specular"
#define MATERIAL_SHININESS	"material.shininess"

//texture
#define USE_DIFFUSEMAP		"useDiffuseTexture"
#define USE_SPECULARMAP		"useSpecularTexture"
#define USE_NORMALMAP		"useNormalTexture"
#define USE_HEIGHTMAP		"useHeightTexture"

#define DIFFUCEMAP			"uDiffuseTexture"
#define SPECULARMAP			"uSpecularTexture"
#define NORMALMAP			"uNormalTexture"
#define HEIGHTMAP           "uHeightTexture"
#define DEPTHMAP			"uDepthMap"

#endif
