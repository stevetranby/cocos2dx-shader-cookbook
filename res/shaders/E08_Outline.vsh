attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
varying mediump vec4 cc_FragColor;
varying mediump vec2 cc_FragTexCoord1;
#else
varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;
#endif

uniform vec2 CC_Texture0Size;
uniform float u_OutlineWidth;

const int SAMPLES = 6;
varying vec2 v_OutlineSamples[SAMPLES];

void main(){
    gl_Position = CC_PMatrix * a_position;
    cc_FragColor = clamp(a_color, 0.0, 1.0);
    cc_FragTexCoord1 = a_texCoord;

	vec2 outlineSize = u_OutlineWidth/CC_Texture0Size;
	for(int i=0; i<SAMPLES; i++){
		float angle = 2.0*3.14159*float(i)/float(SAMPLES);
		v_OutlineSamples[i] = a_texCoord + outlineSize*vec2(cos(angle), sin(angle));
	}
}
