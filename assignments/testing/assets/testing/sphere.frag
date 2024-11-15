#version 330 core

struct Light {
    vec3 pos;
    vec3 color;
};

uniform Light light;

in vec2 TexCoord;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 cameraPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_height1;
uniform sampler2D texture_roughness1;


void main() {

    /*bool blinn = true;

    vec4 texColor = texture(texture_diffuse1, TexCoord);

    //lighting variables
    vec3 norm = normalize(texture(texture_normal1, TexCoord).rgb);
    vec3 lightDir = normalize(light.pos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * light.color;

    float specularStrength = texture(texture_specular1, TexCoord).r;
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = diff == 0.0f ? 0.0f : pow(max(dot(norm, blinn ? halfwayDir : reflectDir), 0.0f), 0.0f);
    vec3 specular = specularStrength * light.color * spec;

    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * light.color;

    vec3 result = texColor.rgb * (ambient + diffuse + specular);

    FragColor = vec4(result, texColor.a);*/
    FragColor = texture(texture_diffuse1, TexCoord);
    //FragColor = vec4(0.0f, TexCoord.x, TexCoord.y, 1.0f);

}