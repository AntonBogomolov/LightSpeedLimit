#version 400

in vec2 vTextureCoordFar;

in vec2 vTextureCoordMed;

in vec2 vTextureCoordCl;


uniform sampler2D sTexture;

void main(void)

{

	vec4 texFar = texture2D(sTexture, vTextureCoordFar);
	vec4 texMed = texture2D(sTexture, vTextureCoordMed);
	vec4 texCl  = texture2D(sTexture, vTextureCoordCl);
	gl_FragColor = texFar + texMed + texCl;
}

