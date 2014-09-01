#ifdef GL_ES
precision mediump float;
#endif

varying vec2 cc_FragTexCoord1;

void main(){
	float t = CC_Time[0];
	vec2 uv = cc_FragTexCoord1;
	float wave = 0.01;
	
	// Sample the same texture several times at different locations.
	vec4 r = texture2D(CC_Texture0, uv + vec2(wave*sin(1.0*t + uv.y*5.0), 0.0));
	vec4 g = texture2D(CC_Texture0, uv + vec2(wave*sin(1.3*t + uv.y*5.0), 0.0));
	vec4 b = texture2D(CC_Texture0, uv + vec2(wave*sin(1.6*t + uv.y*5.0), 0.0));
	
	// Combine the channels, average the alpha values.
	gl_FragColor = vec4(r.r, g.g, b.b, (r.a + b.a + g.a)/3.0);
}
