#version 150

uniform sampler2D sTexture;
uniform vec3 starColor;

uniform vec2 normTerminator;

in vec2 vTextureCoord;
in vec2 vTexPos;

float isOnLight(vec2 pixelPos)
{ 
  float dist =  -((-normTerminator.y) * pixelPos.x + (normTerminator.x) * pixelPos.y) + 0.01;
  return max(3.0 * dist, 0.0); 
}

void main(void)
{   
  vec4 terrainColor = texture2D(sTexture, vTextureCoord); 
  vec3 resultColor = terrainColor.rgb * (starColor + vec3(0.4));

  float alpha = (1.0 - step(0.5, length(vTexPos))) * terrainColor.a;
  gl_FragColor = vec4(resultColor * isOnLight(vTexPos), alpha);
}

