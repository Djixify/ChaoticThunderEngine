#version 330 core
out vec4 FragColor;

in GS_OUT {
	vec3 position;
	vec3 normal;
} fs_in;

vec3 lightPos = vec3(10,0,0);
vec3 lightDir = vec3(-1,0,0);

uniform mat4 view;

uniform vec3 viewPos;
uniform vec3 viewDir;

bool blinn = false;

void main()
{
	vec3 color = vec3(1.0,0.0,0.0);
    // ambient
    vec3 ambient = 0.05 * color;
    // diffuse
    vec3 lightDir = normalize((view*vec4(lightPos,1.0)).xyz - fs_in.position);
    vec3 normal = normalize(fs_in.normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}