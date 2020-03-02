
varying vec4 v_color;
varying vec2 v_texCoord;

uniform sampler2D tex0;

void main(){
  //gl_FragColor=v_color * vec4(fragColor.rgb, 1.0);
    gl_FragColor = vec4(texture2D(tex0, v_texCoord).rgb, 1.0);
}
