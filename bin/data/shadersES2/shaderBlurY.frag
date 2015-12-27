
precision highp float;

uniform sampler2D tex0;

uniform float blurAmnt;

varying vec2 texCoordVarying;

void main()
{
    //http://izmiz.hateblo.jp/entry/2014/10/09/002932
    //vec4 color;
    vec4 color = vec4(0);
	
	color += 1.0 * texture2D(tex0, texCoordVarying + vec2(0.0, blurAmnt * 4.0));
	color += 2.0 * texture2D(tex0, texCoordVarying + vec2(0.0, blurAmnt * 3.0));
	color += 3.0 * texture2D(tex0, texCoordVarying + vec2(0.0, blurAmnt * 2.0));
	color += 4.0 * texture2D(tex0, texCoordVarying + vec2(0.0, blurAmnt * 1.0));
    
	color += 5.0 * texture2D(tex0, texCoordVarying + vec2(0.0, blurAmnt));
	
	color += 4.0 * texture2D(tex0, texCoordVarying + vec2(0.0, blurAmnt * -1.0));
	color += 3.0 * texture2D(tex0, texCoordVarying + vec2(0.0, blurAmnt * -2.0));
	color += 2.0 * texture2D(tex0, texCoordVarying + vec2(0.0, blurAmnt * -3.0));
	color += 1.0 * texture2D(tex0, texCoordVarying + vec2(0.0, blurAmnt * -4.0));
	
    color /= 25.0;
    
    gl_FragColor = color;
}