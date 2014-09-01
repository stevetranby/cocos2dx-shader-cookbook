uniform vec2 u_NoiseTextureSize;
uniform vec2 cc_ViewSizeInPixels;

attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
varying mediump vec4 cc_FragColor;
varying mediump vec2 cc_FragTexCoord1;
varying mediump vec2 cc_FragTexCoord2;
#else
varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;
varying vec2 cc_FragTexCoord2;
#endif

void main(){
    gl_Position = CC_PMatrix * a_position;
	cc_FragColor = clamp(a_color, 0.0, 1.0);
	cc_FragTexCoord1 = a_texCoord;
	
    vec2 distortionScroll = vec2(CC_Time[1], 0.0);
    vec2 screen_0_to_1 = (0.5 * gl_Position.xy / gl_Position.w + 0.5);
	cc_FragTexCoord2 = screen_0_to_1 * cc_ViewSizeInPixels / u_NoiseTextureSize + distortionScroll;

    //cc_FragTexCoord2 = gl_Position.xy / gl_Position.w + distortionScroll;
}
