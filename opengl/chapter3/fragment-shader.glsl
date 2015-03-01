#version 330

// smooth is the default
// smooth means the vertex data will be interpolated across
// the surface being shaded.
//
// other possible values:
// flat, noperspective
//
smooth in vec4 theColor;

uniform float loopDuration;
uniform float time;

out vec4 outputColor;

const vec4 white = vec4(1.0f, 1.0f, 1.0f, 1.0f);

void main()
{
	float currTime = mod(time, loopDuration);
    	float currLerp = currTime / loopDuration;

	outputColor = mix(theColor, white, currLerp);


//	outputColor = theColor;

/*
	// x,y coordinates are window coordinates
	// bottom-left coordinate system
	
	float lerpValue = gl_FragCoord.y / 500.0f;

	// linear interpolation between two colors
	// third parameter must be 0..1 otherwise it's undefined
	outputColor = mix(
		    vec4(1.0f, 1.0f, 0.5f, 1.0f),
		    vec4(0.1f, 0.3f, 0.1f, 1.0f),
		    lerpValue);
*/
}