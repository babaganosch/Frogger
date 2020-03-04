varying vec4 v_color;
varying vec2 v_texCoord;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    v_color = gl_Color;
    v_texCoord = (vec2(gl_Vertex) + vec2(1.0)) * 0.5;
    v_texCoord.y = -v_texCoord.y;
}
