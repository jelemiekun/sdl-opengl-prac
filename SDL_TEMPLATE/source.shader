#shader vertex
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCoord;

uniform vec3 u_ModifiedCoords;
uniform vec4 u_Color;
uniform float u_DimensionScalar;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 verticesColor;
out vec2 textCoord;

void main() {
    vec4 pos = vec4((aPos + (u_ModifiedCoords * 10)) * u_DimensionScalar, 1.0);
    pos = u_Projection * u_View * u_Model  * pos;
    gl_Position = pos;
    verticesColor = u_Color;
    textCoord = aTextCoord;
}



#shader fragment
#version 430 core

in vec4 verticesColor;
in vec2 textCoord;
out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    vec4 texColor = texture(texture1, textCoord);
    FragColor = verticesColor * texColor;
}