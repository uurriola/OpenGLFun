#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform float u_Color;
uniform float u_time;
uniform float u_alpha;
uniform sampler2D u_Texture;

void main()
{
    float red = sin(gl_FragCoord.x / 500) * u_Color;
    float blue = sin(gl_FragCoord.y / 500) * (1 - u_Color);
    float c = 0.5 + sin((gl_FragCoord.x * gl_FragCoord.x + gl_FragCoord.y * gl_FragCoord.y) / (1000 * u_time)) / 2;
    float x = red + c * (1 - red);
    float y = c;
    float z = blue + c * (1 - blue);

    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = u_alpha * texColor + (1 - u_alpha) * vec4(x, y, z, 1.0);
};