#version 150

uniform sampler2D sTexture;
uniform vec3 starColor;

uniform vec2 normTerminator;

in vec2 vTextureCoord;
in vec2 vCloudTextureCoord;
in vec2 vTexPos;
in vec2 sunDirection;

float isOnLight(vec2 pixelPos)
{ 
  float dist =  -((-normTerminator.y) * pixelPos.x + (normTerminator.x) * pixelPos.y) + 0.01;
  return max(3.0 * dist, 0.0); 
}

void main(void)
{ 
  vec2 starVec = sunDirection * 0.006 * (0.5 - length(vTexPos));

  vec4 terrainColor = texture2D(sTexture, vTextureCoord); 
  vec4  cloudColor  = texture2D(sTexture, vCloudTextureCoord);   
  float cloudShadow = texture2D(sTexture, vCloudTextureCoord + starVec).a;
 
  vec3 resultColor = mix(terrainColor.rgb, cloudColor.rgb, cloudColor.a);  
  resultColor = mix(resultColor, vec3(0.0), cloudShadow*(1.0 - cloudColor.a));
  resultColor = resultColor * (starColor + vec3(0.4));

  float alpha = (1.0 - step(0.5, length(vTexPos))) * terrainColor.a;
  gl_FragColor = vec4(resultColor * isOnLight(vTexPos), alpha);
}

