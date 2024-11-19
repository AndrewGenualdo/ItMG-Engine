#version 330 core

struct Light {
    vec3 pos;
    vec3 color;
};

uniform Light light;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 normal;

out vec4 FragColor;

uniform vec3 cameraPos;

/*uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal3;
uniform sampler2D texture_specular2;
uniform sampler2D texture_height5;
uniform sampler2D texture_roughness4;*/

uniform sampler2D sphereMapTex;

uniform mat3 transposeInverseModel;

uniform int type;

void main() {
    /*bool blinn = true;

    vec4 texColor = texture(texture_diffuse1, TexCoord);

    //lighting variables
    vec3 norm = normalize(transposeInverseModel * texture(texture_normal3, TexCoord).rgb);
    vec3 lightDir = normalize(light.pos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * light.color;

    float specularStrength = texture(texture_specular2, TexCoord).r;
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = diff == 0.0f ? 0.0f : pow(max(dot(norm, blinn ? halfwayDir : reflectDir), 0.0f), 32.0f);
    vec3 specular = specularStrength * light.color * spec;

    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * light.color;

    vec3 result = texColor.rgb * (ambient + diffuse + specular);

    FragColor = vec4(result, texColor.a);*/

    //FragColor = texture(texture_diffuse1, TexCoord);

    //float squishMult = TexCoord.y > 0.5f ? abs(0.75f - TexCoord.y) * 4.0f : abs(0.25f - TexCoord.y) * 4.0f;

    //float height = sin(TexCoord.x * 100 + 13) + cos(TexCoord.y * 94 + 52);
    //height = max(0, height - 1.8f) * 5.0f;
    //FragColor = vec4(height, height, height, 1.0f);
    //FragColor = vec4(0.0f, TexCoord.x, TexCoord.y, 1.0f);
    /*if(abs((0.5f - TexCoord.x) + (0.5f - TexCoord.y)) < 0.1f) {
        FragColor = vec4(1);
    } else {
        FragColor = vec4(vec3(0), 1.0f);
    }*/
    /*vec2 coord = TexCoord.xy;
    //coord.x *= 0.5f;
    //coord.y *= 5.0f;
    if(length(coord - vec2(0.5f)) < 0.1f) {
        FragColor = vec4(1);
    } else {
        FragColor = vec4(vec3(0), 1.0f);
    }*/

    vec4 outColor = texture(sphereMapTex, TexCoord);
    //outColor = vec4(0, TexCoord.x, TexCoord.y, 1.0f);
    /*float e = 2.71828182846f;


    vec3 coords[26] = vec3[26](
    vec3(-1, -1, -1),
    vec3(0, -1, -1),
    vec3(1, -1, -1),
    vec3(-1, 0, -1),
    vec3(0, 0, -1),
    vec3(1, 0, -1),
    vec3(-1, 1, -1),
    vec3(0, 1, -1),
    vec3(1, 1, -1),
    vec3(-1, -1, 0),
    vec3(0, -1, 0),
    vec3(1, -1, 0),
    vec3(-1, 0, 0),
    vec3(1, 0, 0),
    vec3(-1, 1, 0),
    vec3(0, 1, 0),
    vec3(1, 1, 0),
    vec3(-1, -1, 1),
    vec3(0, -1, 1),
    vec3(1, -1, 1),
    vec3(-1, 0, 1),
    vec3(0, 0, 1),
    vec3(1, 0, 1),
    vec3(-1, 1, 1),
    vec3(0, 1, 1),
    vec3(1, 1, 1));

    for(int i=0;i<26;i++) {
        float dist = length(normal - normalize(coords[i])) * 10.0f;
        float color = 1.0f / (1.0f + pow(e, -50 * (dist - 0.2f)));
        float blue = float(i) / 26.0f;
        float green = 1.0f - blue;
        if(color < 1.0f) outColor = vec4(color, green, blue, 1.0f);
    }*/




    FragColor = outColor;
}