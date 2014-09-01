#ifdef GL_ES
precision mediump float;
#endif

varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;

void main()
{
	vec2 texCoord = cc_FragTexCoord1;
	
	float time = CC_Time[1];
	texCoord.x += 0.1 * sin(10.0 * texCoord.y + time);
	
	gl_FragColor = cc_FragColor * texture2D(CC_Texture0, texCoord);
}
