#version 150

uniform mat4 uMVPMatrix;

uniform float whRaito;
uniform vec2 camPos;

uniform vec2 farTexXY;
uniform vec2 farTexWH;

uniform vec2 medTexXY;
uniform vec2 medTexWH;

uniform vec2 clTexXY;
uniform vec2 clTexWH;

uniform float farScale;
uniform float MedScale;
uniform float ClScale;

in vec3 aPosition;
in vec2 aTextureCoord;

out vec2 vTextureCoordFar;

out vec2 vTextureCoordMed;

out vec2 vTextureCoordCl;


void main(void)

{
		
	gl_Position = uMVPMatrix * vec4(aPosition, 1.0);   

	vec2 texCoord = aTextureCoord;
	texCoord.x *= whRaito;

	vTextureCoordFar = farTexXY;	
	vec2 texOffset = (texCoord - vTextureCoordFar) / farTexWH;
	vTextureCoordFar += (texOffset * farTexWH) / farScale;	
	vTextureCoordFar += camPos;
	vTextureCoordMed = medTexXY;
	vTextureCoordMed += (texOffset * medTexWH) / MedScale;
	vTextureCoordMed += camPos;
	vTextureCoordCl = clTexXY;
	vTextureCoordCl += (texOffset * clTexWH) / ClScale;	
	vTextureCoordCl += camPos;
}
