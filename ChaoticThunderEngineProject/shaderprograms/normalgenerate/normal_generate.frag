#version 330 core
out vec4 FragColor;


in GS_OUT {
    vec3 position;
    vec3 normal;
} fs_in;

/*
in vec3 position;
in vec3 normal;
*/

uniform sampler2D floorTexture;
uniform vec3 camerapos;
uniform float time;
uniform vec3 model_color = vec3(0.8,0.8,0.8);
uniform vec3 lightpos = vec3(10,0,0);
uniform bool blinnlighting = false;
uniform float ambientlight = 0.2;
uniform vec3 specular_color = vec3(0.2,0.2,0.2);
uniform float diffuse_cutoff = 0.0;
uniform float diffuse_falloff = 0.0001;

void main()
{           
    vec3 color = model_color;
    // ambient
    vec3 ambient = ambientlight * color;
    // diffuse
	//vec3 lightpos = vec3(cos(time)*10, 0.0, sin(time)*10);
    vec3 lightDir = normalize(lightpos - fs_in.position);
    vec3 normal = normalize(fs_in.normal);
	float b = max(min(dot(lightDir, normal) - diffuse_cutoff, 1.0), 0.0);
    float diff = (exp(b*diffuse_falloff) - 1) / (exp(diffuse_falloff) - 1);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(camerapos - fs_in.position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    if(blinnlighting)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    vec3 specular = specular_color * spec; // assuming bright white light color
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}