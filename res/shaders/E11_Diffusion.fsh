uniform float u_Radius;
uniform vec2 CC_Texture0Size;
uniform sampler2D u_NoiseTexture;

varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;
varying vec2 cc_FragTexCoord2;

void main(){
	vec2 noise = 2.0*texture2D(u_NoiseTexture, cc_FragTexCoord2).xy - 1.0;
	vec2 distortionOffset = u_Radius * noise / CC_Texture0Size;
	
	gl_FragColor = cc_FragColor * texture2D(CC_Texture0, cc_FragTexCoord1 + distortionOffset);
}
