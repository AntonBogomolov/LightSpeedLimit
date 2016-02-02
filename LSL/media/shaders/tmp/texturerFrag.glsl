precision lowp float;

varying vec2 vTextureCoord;

uniform sampler2D sTexture;

void main(void)

{

	gl_FragColor = texture2D(sTexture, vTextureCoord);
}

