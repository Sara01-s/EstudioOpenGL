#include "Shader.h"
#include "Renderer.h"
#include "Utils.h"

#include <GLM/glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) 
	: m_RendererID(0), m_VertexFilePath(vertexPath), m_FragmentFilePath(fragmentPath)
{
    std::string vertexSource = Utils::ParseFile(vertexPath);
    std::string fragmentSource = Utils::ParseFile(fragmentPath);

    m_RendererID = CreateShader(vertexSource, fragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::SetUniformInt(const std::string& propertyName, int value) {
    GLCall(glUniform1i(GetUniformLocation(propertyName), value));
}

void Shader::SetUniformFloat(const std::string& propertyName, float value) {
    GLCall(glUniform1f(GetUniformLocation(propertyName), value));
}

void Shader::SetUniformFloat4(const std::string& propertyName, float v0, float v1, float v3, float v4) {
    GLCall(glUniform4f(GetUniformLocation(propertyName), v0, v1, v3, v4));
}

void Shader::SetUniformMat4F(const std::string& propertyName, const glm::mat4& matrix) {
    // 'v' es porque es un array
    GLCall(glUniformMatrix4fv(GetUniformLocation(propertyName), 1, GL_FALSE, glm::value_ptr(matrix)));
}

int Shader::GetUniformLocation(const std::string& propertyName) {
    // Para evitar que se llame a la función glGetUniformLocation en un loop
    // creamos un caché (hashmap, diccionario, unordered_map)
    // que guarde los nombres ya encontrados y los devuelva en tiempo constante (O(1))
    if (m_UniformLocationCache.find(propertyName) != m_UniformLocationCache.end())
        return m_UniformLocationCache[propertyName];

    GLCall(int location = glGetUniformLocation(m_RendererID, propertyName.c_str()));
    m_UniformLocationCache[propertyName] = location;

    return location;
}

// source string DEBE existir, estamos pasando un puntero, si se va de scope o pasa algo raro, F.
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {

    // desc: https://docs.gl/gl4/glCreateShader
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // same as &source[0];

    // id: shader que creamos, 1: cuantos shaders son, &src: puntero al puntero del src, nullptr: solo hay 1 src, sino lo usariamos para dividir
    // desc: https://docs.gl/gl4/glShaderSource (útil para saber más sobre el nullptr)
    GLCall(glShaderSource(id, 1, &src, nullptr));
    // desc: https://docs.gl/gl4/glCompileShader
    // can be trackked with: https://docs.gl/gl4/glGetShader
    GLCall(glCompileShader(id));

    // Chequear errores de compilación.
    // IV porque = i: integer (shader id), v: vector (result data, un puntero X).
    // el 2do parametro es la data que queremos consultar.
    // desc: https://docs.gl/gl4/glGetShader
    int result{};
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE) {
        // El Shader no compiló bien.
        int length{};
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        // Hack para tener length stack-allocated (_malloca: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/malloca?view=msvc-170)
        char* errorMessage = static_cast<char*>(_malloca(length * sizeof(char)));

        // Guardamos el registro de lo que sucedió en el shader en nuestro mensaje
        // desc: https://docs.gl/gl4/glGetShaderInfoLog
        GLCall(glGetShaderInfoLog(id, length, &length, errorMessage));

        std::string shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        std::cout << "Failed to compile " << shaderType << " shader!" << '\n';
        std::cout << errorMessage << '\n';

        // Chao chao shader malito.
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;

}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

    // Creamos programa principal
    GLCall(unsigned int program = glCreateProgram());

    // Compilamos los 2 shaders principales
    unsigned int vertShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Ahora queremos "linkearlos" en un mismo programa.
    // Para ello primero unimos los shaders a nuestro programa.
    // desc: https://docs.gl/gl4/glAttachShader
    GLCall(glAttachShader(program, vertShader));
    GLCall(glAttachShader(program, fragShader));
    // Linkeamos los shaders unidos al programa especificado.
    // desc: https://docs.gl/gl4/glLinkProgram
    GLCall(glLinkProgram(program));
    // Chequeamos que el programa linkeado es apto para ejecutarse en el estado actual de OpenGL
    // La info se guardará en el registro de "program" (puede ser un string vacío)
    // desc: https://docs.gl/gl4/glValidateProgram
    GLCall(glValidateProgram(program));

    // Ahora que tenemos el programa completo, podemos eliminar los restantes de los shaders
    // esto es como eliminar los archivos ".o" que genera C++.
    // Sin embargo, este pequeño source, es esencial para debuggear en el futuro, por lo que es opcional.
    // desc: https://docs.gl/gl4/glDeleteShader;
    // -> glDeleteShader(vertShader);
    // -> glDeleteShader(fragShader);

    return program;
}
