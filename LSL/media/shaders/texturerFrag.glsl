#version 150

in vec2 vTextureCoord;

uniform sampler2D sTexture;

void main(void)

{

	gl_FragColor = texture2D(sTexture, vTextureCoord);
}

