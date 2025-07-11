#shader vertex
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 textCoord;

out vec3 color;
out vec2 v_TextCoord;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    color = aColor;
    v_TextCoord = textCoord;
}



#shader fragment
#version 430 core

in vec3 color;
in vec2 v_TextCoord;
out vec4 FragColor;

uniform vec4 u_Color;
uniform sampler2D texture1;
    
void main() {
    vec4 textColor = texture(texture1, v_TextCoord);
    FragColor = textColor * vec4(color, 1.0f);
}