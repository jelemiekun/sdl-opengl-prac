#shader vertex
#version 430 core
	
layout (location = 0) in vec3 L_coordinates;
layout (location = 1) in vec3 L_normal;

uniform mat3 u_NormalMatrix;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 FragPos;
out vec3 Normal;

void main() {
	vec4 pos = vec4(L_coordinates, 1.0f);
	pos = u_Projection * u_View * u_Model * pos;
	gl_Position = pos;

	Normal = u_NormalMatrix * L_normal;
	FragPos = vec3(u_Model * vec4(L_coordinates, 1.0f));
}



#shader fragment
#version 430 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 color;
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 u_ObjectColor;
uniform vec3 u_CameraPos;

void main(){
	// ambient
	vec3 ambient = light.ambient * material.ambient;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// specular
	vec3 viewDir = normalize(u_CameraPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specular = light.specular * (spec * material.specular);

	// final
	vec3 finalColor = (ambient + diffuse + specular) * u_ObjectColor;

	FragColor = vec4(finalColor, 1.0f);
}