#version 400

uniform mat4 uMVPMatrix;

uniform float time;
uniform float rotSpeed;

uniform vec2 texSize;
uniform vec2 texCenter;
uniform vec2 texLeftCorner;

in vec3 aPosition;
in vec2 aTextureCoord;

out vec2 vTexPos;
out vec2 vTextureCoord;

void main(void)
{   
  float rotAngl = time * rotSpeed;
  mat3 RotationMatrix = mat3(cos( rotAngl ), -sin( rotAngl ), 0.0, 
                             sin( rotAngl ),  cos( rotAngl ), 0.0, 
                             0.0,           0.0,              1.0 );
  
  gl_Position = uMVPMatrix * vec4(aPosition, 1.0);  

  vec2 texRect  = aTextureCoord - texCenter;      
  vec2 texRotRect = (RotationMatrix * vec3(texRect,1.0)).xy; 
 
  vTextureCoord = texRotRect + texCenter; 
  vTexPos = texRect / texSize; 
}
