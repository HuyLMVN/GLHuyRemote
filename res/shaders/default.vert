#version 460 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec2 aTex;
// Texture Coordinates
layout (location = 3) in vec4 aColor;


// Outputs the current position for the Fragment Shader
out vec3 crntPos;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;
// Outputs the color for the Fragment Shader
out vec4 color;


// Uniform for camera matrtix and scaling
uniform mat4 camMatrix;
uniform mat4 uPos;
uniform mat4 scale;


void main()
{
	// calculates current position
	crntPos = vec3(uPos * vec4(aPos, 1.0f));
	// Assigns the normal from the Vertex Data to "Normal"
	Normal = aNormal;
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = aTex;
	// Output the positions/coordinates of all vertices
	gl_Position = camMatrix * scale * vec4(crntPos, 1.0f);
}
