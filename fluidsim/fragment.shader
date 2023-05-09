#version 330 core

out vec4 FragColor;
in vec3 outColor;
in vec2 texCoord;

uniform vec4 color;
uniform sampler1D pointTexture;

uniform int width;
uniform float radiusSquared;

void main()
{
    float t = 0;
    float r = 0;

    for (int i = 0; i < width; i++) {
        float c = float(i) / width;
        vec4 pos = texture(pointTexture, c);
        float d = pow(pos.x - texCoord.x, 2) + pow(pos.y - texCoord.y, 2);
        t += 1 / ((1 + pow(d*10, 10)) * width);
        if (d < 0.0001)
            r = 1;
        //100*0.01/(1-0)=1
    }

    FragColor = vec4(t,t,t, 1);
}