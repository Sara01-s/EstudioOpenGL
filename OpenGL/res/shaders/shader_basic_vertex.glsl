
#version 330 core

// en realidad es vec2, pero gl_Position es vec4 y queremos que la conversi�n sea impl�cita
// 0 es el �ndice de nuestro atributo (definido en glVertexAttribPointer)
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord; // v_ es por "varing", output que enviaremos al fragment shader

uniform mat4 u_ModelViewProjection;

void main() {
    v_TexCoord = texCoord;

    // al multiplicar la posici�n del v�rtice con la matriz de proyecci�n
    // el v�rtice quedar� en el lugar correcto dentro del espacio
    gl_Position = u_ModelViewProjection * position;
};
