attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;

void main()
{
    gl_Position = CC_PMatrix * a_position;
    cc_FragColor = a_color;
    cc_FragTexCoord1 = a_texCoord;
}
