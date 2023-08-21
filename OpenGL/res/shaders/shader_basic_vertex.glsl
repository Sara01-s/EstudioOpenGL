#version 330 core

// en realidad es vec2, pero gl_Position es vec4 y queremos que la conversión sea implícita
// 0 es el índice de nuestro atributo (definido en glVertexAttribPointer)
layout (location = 0) in vec4 position;
//layout (location = 1) in vec2 texCoord;

// v_ es por "varying", palabra clave usada en GLSL 120, pero quedó como convención
// output que enviaremos al fragment shader
//out vec2 v_TexCoord; 

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_MVP;

void main() {
    //v_TexCoord = texCoord;

    // al multiplicar la posición del vértice con la matriz de proyección
    // el vértice quedará en el lugar correcto dentro del espacio
    gl_Position = u_MVP * position;
};
