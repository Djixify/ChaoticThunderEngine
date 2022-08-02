#version 330 core
out vec4 FragColor;

in GS_OUT {
	vec3 position;
	vec3 normal;
} fs_in;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform vec3 lightPos;
//vec3 lightPos = vec3(10.0,0.0,4.0);
uniform vec3 color;

bool blinn = false;

void main()
{
	//vec3 lightDir = (view * vec4(-1,0,0,1)).xyz;

	//vec3 color = vec3(1.0,0.0,0.0);
    // ambient
    vec3 ambient = 0.2 * color;
    // diffuse
    vec3 _lightPos = (projection * view * vec4(lightPos,1.0)).xyz;
    vec3 lightDir = normalize(_lightPos - fs_in.position);
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
    //FragColor = vec4(ambient, 1.0);
    FragColor = vec4(diffuse, 1.0);
    //FragColor = vec4(specular, 1.0);
    //FragColor = vec4(ambient + diffuse + specular, 1.0);
}