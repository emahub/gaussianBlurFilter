#version 150

uniform sampler2DRect tex0;
uniform float blurAmnt;

in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{
    //http://izmiz.hateblo.jp/entry/2014/10/09/002932
    //vec4 color;
    vec4 color = vec4(0);
	
    color += 1.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * -4.0, 0.0));
    color += 2.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * -3.0, 0.0));
    color += 3.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * -2.0, 0.0));
    color += 4.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * -1.0, 0.0));
    
    color += 5.0 * texture(tex0, texCoordVarying + vec2(blurAmnt, 0));
	
    color += 4.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * 1.0, 0.0));
    color += 3.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * 2.0, 0.0));
    color += 2.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * 3.0, 0.0));
    color += 1.0 * texture(tex0, texCoordVarying + vec2(blurAmnt * 4.0, 0.0));
    
    color /= 25.0;
    
    outputColor = color;
}