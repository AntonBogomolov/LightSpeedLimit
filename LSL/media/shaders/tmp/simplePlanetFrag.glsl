#version 400

uniform sampler2D sTexture;
uniform vec3 starColor;

uniform vec2 normTerminator;

in vec2 vTextureCoord;
in vec2 vTexPos;

const float eps = 0.001;

float isOnLight(vec2 pixelPos)
{ 
  float dist =  -((-normTerminator.y) * pixelPos.x + (normTerminator.x) * pixelPos.y) + 0.01;
  return max(8.0 * dist, 0.0); 
}

void main(void)
{
  vec4 texColor = texture2D(sTexture, vTextureCoord.xy);
  texColor.rgb = texColor.rgb * (starColor.rgb + vec3(0.6));

  float alpha = (1.0 - step(0.5, length(vTexPos))) * texColor.a;
  gl_FragColor = vec4(texColor.rgb * isOnLight(vTexPos), alpha);
}

