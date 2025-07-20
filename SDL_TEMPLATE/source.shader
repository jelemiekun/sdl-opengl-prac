#shader vertex
#version 430 core
	
layout (location = 0) in vec3 L_coordinates;
layout (location = 1) in vec2 L_imgCoordinates;
layout (location = 2) in vec3 L_norm;

out vec2 imgCoordinates;
out vec3 normal;
out vec3 fragPos;

uniform mat3 u_NormalMatrix;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
	vec4 pos = vec4(L_coordinates, 1.0f);
	pos = u_Projection * u_View * u_Model * pos;
	gl_Position = pos;

	imgCoordinates = L_imgCoordinates;
	normal = u_NormalMatrix * L_norm;

	fragPos = vec3(u_Model * vec4(L_coordinates, 1.0f));
}



#shader fragment
#version 430 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 imgCoordinates;
in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 u_CameraPos;

void main(){
	// ambient	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, imgCoordinates));

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, imgCoordinates));

	// specular
	vec3 viewDir = normalize(u_CameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specular = light.specular * spec * vec3(texture(material.diffuse, imgCoordinates));

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}