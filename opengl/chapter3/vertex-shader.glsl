#version 330

// inputs to vertex shader are called vertex attributes
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

// uniform variables only change between executions of
// rendering calls;
uniform float loopDuration;
uniform float time;

smooth out vec4 theColor;

void main()
{
	float fScale = 3.14159f * 2.0f / loopDuration;
    	float fCurrentTimeInLoop = mod(time, loopDuration);

	vec4 offset = vec4(
		    cos(fCurrentTimeInLoop * fScale) * 0.5f,
		    sin(fCurrentTimeInLoop * fScale) * 0.5f,
		    0.0f,
		    0.0f);

	gl_Position = position + offset;

	theColor = color;
}
