#version 330 core

struct Light {
    vec3 pos;
    vec3 color;
};

uniform Light light;
uniform sampler2D tex;
uniform vec3 cameraPos;
uniform vec3 ambientSpecularShininess;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
in vec3 viewPos;

out vec4 FragColor;

void main() {

    bool blinn = true;

    vec4 texColor = texture(tex, TexCoord);

    //lighting variables
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.pos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * light.color;

    float specularStrength = ambientSpecularShininess.y;
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = diff == 0.0f ? 0.0f : pow(max(dot(norm, blinn ? halfwayDir : reflectDir), 0.0f), ambientSpecularShininess.z);
    vec3 specular = specularStrength * light.color * spec;

    float ambientStrength = ambientSpecularShininess.x;
    vec3 ambient = ambientStrength * light.color;

    vec3 result = texColor.rgb * (ambient + diffuse + specular);

    FragColor = vec4(result, texColor.a);//vec4(abs(normalize(Normals)), 1.0f);

}