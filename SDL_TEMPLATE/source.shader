#shader vertex
#version 430 core
	
layout (location = 0) in vec3 L_coordinates;
layout (location = 1) in vec3 L_normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 FragPos;
out vec3 Normal;

void main() {
	vec4 pos = vec4(L_coordinates, 1.0f);
	pos = u_Projection * u_View * u_Model * pos;
	gl_Position = pos;

	Normal = mat3(transpose(inverse(u_Model))) * L_normal;
	FragPos = vec3(u_Model * vec4(L_coordinates, 1.0f));
}



#shader fragment
#version 430 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform float u_AmbientStrength;
uniform vec3 u_LightPos;

void main(){
	vec3 ambient = u_AmbientStrength * u_LightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_LightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * u_LightColor;

	vec3 finalColor = (diffuse + ambient) * u_ObjectColor;

	FragColor = vec4(finalColor, 1.0f);
}