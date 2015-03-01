#version 330

// inputs to vertex shader are called vertex attributes
layout(location = 0) in vec4 position;

void main()
{
	// gl_Position is defined as:
	// out vec4 gl_Position;
	gl_Position = position;
}
