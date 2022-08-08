#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

uniform sampler2D floorTexture;
uniform vec3 camerapos;
uniform vec3 lightpos;
uniform float time;
uniform bool blinn;

void main()
{           
    vec3 color = vec3(0.8, 0.8, 0.8);
    // ambient
    vec3 ambient = 0.05 * color;
    // diffuse
	//vec3 lightpos = vec3(cos(time)*10, 0.0, sin(time)*10);
    vec3 lightDir = normalize(lightpos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(camerapos - fs_in.FragPos);
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