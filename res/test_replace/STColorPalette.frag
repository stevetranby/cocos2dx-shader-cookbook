#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 v_ColorFrom;
uniform vec4 v_ColorTo;

varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;

void main()
{
    vec4 textureColor = texture2D(CC_Texture0, cc_FragTexCoord1);
    float dist = abs(textureColor.r - textureColor.g) + abs(textureColor.g - textureColor.b) + abs(textureColor.r - textureColor.b);
    if(dist < 0.001)
        gl_FragColor = cc_FragColor * textureColor;
    else
        gl_FragColor = textureColor;
}
