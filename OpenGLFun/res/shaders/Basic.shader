#shader vertex
#version 330 core

// Load in attribute at index 0 as a vec4
layout(location = 0) in vec4 position;

void main()
{
   gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform float u_Color;
uniform float u_time;

void main()
{
    float red = sin(gl_FragCoord.x / 500)* u_Color;
    float blue = sin(gl_FragCoord.y / 500) * (1 - u_Color);
    float c = 0.5 + sin((gl_FragCoord.x * gl_FragCoord.x + gl_FragCoord.y * gl_FragCoord.y) / (1000 * u_time)) / 2;
    float x = red + c * (1 - red);
    float y = c;
    float z = blue + c * (1 - blue);
    color = vec4(x, y, z, 1.0);
};