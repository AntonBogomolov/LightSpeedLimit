#version 400

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;

in vec3 aPosition;
out vec2 vTexPos;

void main(void)
{    
  vec4 mvPos = uMVMatrix * vec4(aPosition, 1.0); 
  gl_Position = uPMatrix * mvPos;       
 
  vTexPos = mvPos.xy; 
}

