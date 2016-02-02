#version 150

uniform sampler2D sTexture;
uniform vec3 starColor;
uniform vec3 lumColor;

uniform vec2 normTerminator;
uniform vec2 texSize;

in vec2 vTextureCoord;
in vec2 vTexPos;
in vec2 sunDirection;

float isOnLight(vec2 pixelPos)
{ 
  float dist =  -((-normTerminator.y) * pixelPos.x + (normTerminator.x) * pixelPos.y) + 0.01;
  return max(3.0 * dist, 0.0); 
}

void main(void)
{ 
  float poleDistance = length(vTexPos);
  vec2 starVec = sunDirection * (0.5 - poleDistance);

  vec4 terrainColor = texture2D(sTexture, vTextureCoord); 
  vec4 modColor  = texture2D(sTexture, vTextureCoord + vec2(texSize.x, 0.0) );     
  terrainColor.rgb =  terrainColor.rgb * isOnLight(vTexPos);
  vec3 additionalColor = terrainColor.rgb;

  float isWater = step(1.0, modColor.r);
  float waterShine = dot(vec2(-normTerminator.y,normTerminator.x), vTexPos) * isWater * 2.0; 
  additionalColor *= 0.5 + waterShine*waterShine*waterShine*waterShine*(1.0 + poleDistance);

  float isLum = step(1.0, modColor.g);
  additionalColor = additionalColor + lumColor * isLum;

  vec3 resultColor = mix(terrainColor.rgb, additionalColor, modColor.a);  
  resultColor = resultColor * (starColor + vec3(0.4));

  float alpha = (1.0 - step(0.5, poleDistance)) * terrainColor.a;
  gl_FragColor = vec4(resultColor, alpha);
}