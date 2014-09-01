uniform vec2 u_NoiseTexturePixelSize;

void main(){
    gl_Position = CC_PMatrix * a_position;
    cc_FragColor = clamp(a_color, 0.0, 1.0);
    cc_FragTexCoord1 = a_texCoord;

	vec2 screen01 = (0.5*gl_Position.xy/gl_Position.w + 0.5);
	cc_FragTexCoord2 = screen01*cc_ViewSizeInPixels/u_NoiseTexturePixelSize;
	cc_FragTexCoord2 += cc_Random01.xy;
}
