uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;

attribute vec3 aPosition;
attribute vec2 aTextureCoord;
attribute vec3 aNormal;
varying vec2 vTextureCoord;

void main(void)

{
		
	vec4 mvPos = uMVMatrix * vec4(aPosition, 1.0); 
        gl_Position = uPMatrix * mvPos;       

	vTextureCoord = aTextureCoord;
}
