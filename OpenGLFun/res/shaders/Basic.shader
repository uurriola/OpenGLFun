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

void main()
{
   color = vec4(gl_FragCoord.xy / 500., 0.0, 1.0);
};