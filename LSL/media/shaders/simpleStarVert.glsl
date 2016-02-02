#version 150

uniform mat4 uMVPMatrix;
uniform vec2 texSize;
uniform vec2 texLeftCorner;

in vec3 aPosition;
in vec2 aTextureCoord;

out vec2 vTexPos;

void main(void)
{      
  gl_Position = uMVPMatrix * vec4(aPosition, 1.0);
  vTexPos = (aTextureCoord - texLeftCorner - texSize / 2.0) / texSize; 
}

