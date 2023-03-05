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

uniform vec4 u_Color;

void main()
{
   color = vec4(gl_FragCoord.x / 500 * u_Color.x, gl_FragCoord.y / 500 * (1 - u_Color.x), 0.0, 1.0);
};