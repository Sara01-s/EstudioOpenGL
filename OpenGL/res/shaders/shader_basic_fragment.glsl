#version 330 core

out vec4 fragColor;

// un "uniform" es un valor que se actualiza antes de cada draw y es accesible desde la CPU
uniform vec4 u_Color;

void main() {
    fragColor = u_Color;
};
