

#version 330 compatibility

in vec3 outColor;
out vec4 FragColor;

void main()
{

	if(gl_FrontFacing)
	{
		FragColor = vec4 ( outColor.rgb, 1.0f);
	}
	else
	{
		discard;
	}
}

