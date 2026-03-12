#version 460 core

out vec4 FragColor;
in vec2 TexPos;
uniform sampler2D textureatlas;
uniform vec4 BlockTint;
vec4 Texcolor;

void main()
{
   Texcolor = texture(textureatlas, TexPos);
   FragColor = Texcolor * BlockTint;
}