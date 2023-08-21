#version 330 core

// en realidad es vec2, pero gl_Position es vec4 y queremos que la conversi�n sea impl�cita
// 0 es el �ndice de nuestro atributo (definido en glVertexAttribPointer)
layout (location = 0) in vec4 position;
//layout (location = 1) in vec2 texCoord;

// v_ es por "varying", palabra clave usada en GLSL 120, pero qued� como convenci�n
// output que enviaremos al fragment shader
//out vec2 v_TexCoord; 

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_MVP;

void main() {
    //v_TexCoord = texCoord;

    // al multiplicar la posici�n del v�rtice con la matriz de proyecci�n
    // el v�rtice quedar� en el lugar correcto dentro del espacio
    gl_Position = u_MVP * position;
};
