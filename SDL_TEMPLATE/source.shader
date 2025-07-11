#shader vertex
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    color = aColor;
}



#shader fragment
#version 430 core

in vec3 color;
out vec4 FragColor;

uniform vec4 u_Color;
    
void main() {
    FragColor = vec4(color, 1.0f);
}