#version 330 core

in vec3 normal;
out vec4 FragColor;
in vec2 TexPos;
uniform sampler2D textureatlas;
uniform vec4 BlockTint;
vec4 Texcolor;
float lighting;

void main()
{

   if (normal.y > 0.0f) {
      lighting = 1.0f;
   } else if (normal.y < 0.0f) {
      lighting = 0.5f;
   } else {
      lighting = (abs(normal.x) * 0.6) + (abs(normal.z) * 0.8);
   }

   Texcolor = texture(textureatlas, TexPos);
   FragColor = vec4((Texcolor.rgb * BlockTint.rgb) * lighting, Texcolor.a * BlockTint.a);
}