uniform sampler2D u_Texture;

varying vec2 v_UV;
varying vec4 v_Color;

uniform vec2 u_Center;
uniform float u_RadiusAlpha;
uniform float u_Radius;
uniform vec4 u_BackColor;

void main()
{
	//NOTE: Vec4 letter coordinates x,y,z,w

	float aDistance = distance( gl_FragCoord, u_Center );
	float alpha = (aDistance - u_RadiusAlpha) / (u_Radius - u_RadiusAlpha);
	
	vec4 aTexture = texture2D( u_Texture, v_UV );

	if(aTexture.w != 0)
	{
		gl_FragColor = alpha * aTexture + (1 - alpha) * u_BackColor;
	}
}
