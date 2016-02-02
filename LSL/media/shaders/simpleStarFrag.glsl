#version 150

uniform vec3 starColor;
uniform vec2 starPos;

in vec2 vTexPos;

void main(void)
{
  float factor = length(vTexPos);
  float scale  = 0.45;
  float c = min(scale / factor, 4.0);

  gl_FragColor = vec4(starColor * c , (1.0 - smoothstep(scale * 0.5, 0.5, factor))  );
}
