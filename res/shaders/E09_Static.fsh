#ifdef GL_ES
precision mediump float;
#endif

varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;
varying vec2 cc_FragTexCoord2;

uniform sampler2D u_NoiseTexture;
uniform float u_NoiseAmount;

void main()
{
	vec4 textureColor = texture2D(CC_Texture0, cc_FragTexCoord1);
	vec4 noiseColor = texture2D(u_NoiseTexture, cc_FragTexCoord2);
	
	gl_FragColor = cc_FragColor * textureColor;
	gl_FragColor.rgb += (textureColor.a * u_NoiseAmount) * (2.0 * noiseColor.rgb - 1.0);
}
