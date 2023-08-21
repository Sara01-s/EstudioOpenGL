#version 330 core

out vec4 fragColor;

// un "uniform" es un valor que se actualiza antes de cada draw y es accesible desde la CPU
uniform vec4 u_Color;
// por aqui entra la textura
uniform sampler2D u_Texture;

//in vec2 v_TexCoord; // esta data nos llegará del out v_TexCoord del Vertex Shader

void main() {
    
    //vec4 texColor = texture(u_Texture, v_TexCoord);

    fragColor = vec4(1.0);

};
