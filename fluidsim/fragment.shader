#version 330 core

out vec4 FragColor;
in vec3 outColor;
in vec2 texCoord;

uniform vec4 color;
uniform sampler2D pointTexture;

uniform int xWorld;
uniform int yWorld;
uniform int xDivisions;
uniform int yDivisions;

void main()
{
    int x = int(floor(texCoord.x * xWorld));
    int y = int(floor(texCoord.y * yWorld));

    int division = x%xDivisions + (y%yDivisions)*xDivisions;

    float t = 0;
    
    for (int i = 0; i < 100; i++) {
        vec4 tex = texture(pointTexture, vec2(float(i)/100, division));
        if (tex.z == 0)
            break;
        if (pow(tex.x - texCoord.x, 2) + pow(tex.y - texCoord.y, 2) < pow(0.1, 2)) {
            t = 1;
        }
    }


    //
    FragColor = vec4(t, 0, 0, 1);
}