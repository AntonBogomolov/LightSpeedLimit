#version 150

uniform mat4 uMVPMatrix;
uniform vec3 orbitColor;

in vec3 aPosition;
out vec4 vColor;

const int vertCnt = 100;

void main(void)

{
		
	gl_Position = uMVPMatrix * vec4(aPosition, 1.0);
        float alpha = 1.0 - float(gl_VertexID) / vertCnt; 
	vColor = vec4(orbitColor.r, orbitColor.g, orbitColor.b, alpha);
}
