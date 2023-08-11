
#version 330 core

// en realidad es vec2, pero gl_Position es vec4 y queremos que la conversi�n sea impl�cita
// 0 es el �ndice de nuestro atributo (definido en glVertexAttribPointer)
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord; // v_ es por "varing", output que enviaremos al fragment shader

void main() {
    gl_Position = position;
    v_TexCoord = texCoord;
};
