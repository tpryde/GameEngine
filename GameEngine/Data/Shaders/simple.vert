attribute vec2 a_UV;
attribute vec4 a_Color;
attribute vec3 a_Position;


varying vec2 v_UV;
varying vec4 v_Color;

void main()
{
    gl_Position = vec4( a_Position, 1.0 );

	v_UV = a_UV;
	v_Color = a_Color;
}
