#version 400

uniform mat4 uMVPMatrix;

uniform float whRaito;
uniform float camPosX;
uniform float camPosY;

uniform float farTexX;
uniform float farTexY;
uniform float farTexW;
uniform float farTexH;

uniform float medTexX;
uniform float medTexY;
uniform float medTexW;
uniform float medTexH;

uniform float clTexX;
uniform float clTexY;
uniform float clTexW;
uniform float clTexH;

uniform float farScale;
uniform float MedScale;
uniform float ClScale;

in vec3 aPosition;
in vec2 aTextureCoord;
in vec3 aNormal;

out vec2 vTextureCoordFar;

out vec2 vTextureCoordMed;

out vec2 vTextureCoordCl;


void main(void)

{
		
	gl_Position = uMVPMatrix * vec4(aPosition, 1.0);   

	vec2 camPos = vec2(camPosX, camPosY);
	vec2 farTexWH = vec2(farTexW, farTexH);
	vec2 medTexWH = vec2(medTexW, medTexH);	
	vec2 clTexWH  = vec2(clTexW,  clTexH);
	aTextureCoord.x *= whRaito;

	vTextureCoordFar = vec2(farTexX, farTexY);	
	vec2 texOffset = (aTextureCoord - vTextureCoordFar) / farTexWH;
	vTextureCoordFar += (texOffset * farTexWH) / farScale;	
	vTextureCoordFar += camPos;
	vTextureCoordMed = vec2(medTexX, medTexY);
	vTextureCoordMed += (texOffset * medTexWH) / MedScale;
	vTextureCoordMed += camPos;
	vTextureCoordCl = vec2(clTexX, clTexY);
	vTextureCoordCl += (texOffset * clTexWH) / ClScale;	
	vTextureCoordCl += camPos;
}
