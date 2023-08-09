
#version 330 core

// en realidad es vec2, pero gl_Position es vec4 y queremos que la conversi�n sea impl�cita
// 0 es el �ndice de nuestro atributo (definido en glVertexAttribPointer)
layout (location = 0) in vec4 position;

void main() {
    gl_Position = position;
};
