#shader vertex
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCoord;
layout (location = 2) in vec3 i_Color;

uniform vec3 u_ModifiedCoords;
out vec3 verticesColor;
out vec2 textCoord;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0) * vec4(u_ModifiedCoords, 1.0);
	verticesColor = i_Color;
	textCoord = aTextCoord;
}



#shader fragment
#version 430 core

in vec3 verticesColor;
in vec2 textCoord;
out vec4 FragColor;

uniform sampler2D texture1;

void main() {
	vec4 texColor = texture(texture1, textCoord);
	FragColor = vec4(verticesColor, 1.0f) * texColor;
}