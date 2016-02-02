#version 150

uniform mat4 uMVPMatrix;

in vec3 aPosition;
in vec2 aTextureCoord;

out vec2 vTextureCoord;

void main(void)

{
		
	gl_Position = uMVPMatrix * vec4(aPosition, 1.0);   
	vTextureCoord = aTextureCoord;
}
