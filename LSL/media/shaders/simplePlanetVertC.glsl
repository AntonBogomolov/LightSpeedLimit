#version 150

uniform mat4 uMVPMatrix;

uniform float time;
uniform float rotSpeed;

uniform  float texSizeMod;
uniform vec2 texSize;
uniform vec2 texCenter;

uniform float cloudsSpeed;
uniform vec2 normTerminator;

in vec3 aPosition;
in vec2 aTextureCoord;

out vec2 vTexPos;
out vec2 vTextureCoord;
out vec2 vCloudTextureCoord;
out vec2 sunDirection;

void main(void)
{   
  float rotAngl = time * (-rotSpeed);
  mat2 RotationMatrix = mat2(cos( rotAngl ), -sin( rotAngl ),  
                             sin( rotAngl ),  cos( rotAngl ));

  rotAngl = time * (-rotSpeed - cloudsSpeed);
  mat2 CloudRotationMatrix = mat2(cos( rotAngl ), -sin( rotAngl ), 
                             	  sin( rotAngl ),  cos( rotAngl ));
  
  gl_Position = uMVPMatrix * vec4(aPosition, 1.0);  
 
  vec2 texRect  = aTextureCoord - texCenter;       
  texRect.x /= texSizeMod;  
  vec2 texRotRect   = RotationMatrix * texRect; 
  vec2 cloudRotRect = CloudRotationMatrix * texRect; 
   
  vCloudTextureCoord = cloudRotRect + texCenter + vec2(texSize.x / 2.0f, 0.0); 
  vTextureCoord = texRotRect + texCenter + vec2( -texSize.x / 2.0f, 0.0);
 
  vTexPos = texRect / texSize; 
  sunDirection = CloudRotationMatrix * vec2(normTerminator.y, normTerminator.x);
}
