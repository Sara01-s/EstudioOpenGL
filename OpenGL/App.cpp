#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream> // Para loggear
#include <fstream>  // Para traducir shaders (ifstream)
#include <string>   // Para leer archivos (getline)
#include <sstream>  // Para leer y guardar los vertex y fragment (stringstream) (es como StringBuilder en C#)

// Macros para expandir el "debugger" de OpenGL
#ifdef _DEBUG
    #define ASSERT(x) if (!(x)) __debugbreak();
    #define GLCall(x) GLClearError();\
                      x;\
                      ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    #define GLCall(x) x
#endif

struct ShaderSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static void GLClearError() {
    // tambien se puede escribir como
    // while (!glGetError());
    // ya que está garantizado que glGetError retorne 0 o 1
    // desc: https://docs.gl/gl4/glGetError
    while (glGetError() != GL_NO_ERROR);
}

_NODISCARD
static bool GLLogCall(const char* function, const char* file, int line) {
    // Mientras que glGetError no retorne 0 (o sea un error)
    while (GLenum error = glGetError()) {
        std::cout << "\n[OpenGL Error] (" << error << "): "
                  << function << " " << file << ":" << line << '\n';
        return false;
    }

    return true;
}

static std::string ParseFile(const std::string filePath) {
    std::ifstream file(filePath);
    std::string line;
    std::string content;

    while (std::getline(file, line)) {
        content.append(line + "\n");
    }

    file.close(); // Creo que ifstream cierra automáticamente los archivos fuera de scope. Creo.

    return content;
}

// source string DEBE existir, estamos pasando un puntero, si se va de scope o pasa algo raro, F.
static unsigned int CompileShader(unsigned int type, const std::string& source) {
    
    // desc: https://docs.gl/gl4/glCreateShader
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // same as &source[0];

    // id: shader que creamos, 1: cuantos shaders son, &src: puntero al puntero del src, nullptr: solo hay 1 src, sino lo usariamos para dividir
    // desc: https://docs.gl/gl4/glShaderSource (útil para saber más sobre el nullptr)
    glShaderSource(id, 1, &src, nullptr);
    // desc: https://docs.gl/gl4/glCompileShader
    // can be trackked with: https://docs.gl/gl4/glGetShader
    glCompileShader(id);

    // Chequear errores de compilación.
    // IV porque = i: integer (shader id), v: vector (result data, un puntero X).
    // el 2do parametro es la data que queremos consultar.
    // desc: https://docs.gl/gl4/glGetShader
    int result {};
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    
    if (result == GL_FALSE) {
        // El Shader no compiló bien.
        int length {};
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        
        // Hack para tener length stack-allocated (_malloca: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/malloca?view=msvc-170)
        char* errorMessage = static_cast<char*>(_malloca(length * sizeof(char)));

        // Guardamos el registro de lo que sucedió en el shader en nuestro mensaje
        // desc: https://docs.gl/gl4/glGetShaderInfoLog
        glGetShaderInfoLog(id, length, &length, errorMessage);

        std::string shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        std::cout << "Failed to compile " << shaderType << " shader!" << '\n';
        std::cout << errorMessage << '\n';

        // Chao chao shader malito.
        glDeleteShader(id);
        return 0;
    }
    
    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

    // Creamos programa principal
    unsigned int program = glCreateProgram();

    // Compilamos los 2 shaders principales
    unsigned int vertShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Ahora queremos "linkearlos" en un mismo programa.
    // Para ello primero unimos los shaders a nuestro programa.
    // desc: https://docs.gl/gl4/glAttachShader
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    // Linkeamos los shaders unidos al programa especificado.
    // desc: https://docs.gl/gl4/glLinkProgram
    glLinkProgram(program);
    // Chequeamos que el programa linkeado es apto para ejecutarse en el estado actual de OpenGL
    // La info se guardará en el registro de "program" (puede ser un string vacío)
    // desc: https://docs.gl/gl4/glValidateProgram
    glValidateProgram(program);

    // Ahora que tenemos el programa completo, podemos eliminar los restantes de los shaders
    // esto es como eliminar los archivos ".o" que genera C++.
    // Sin embargo, este pequeño source, es esencial para debuggear en el futuro, por lo que es opcional.
    // desc: https://docs.gl/gl4/glDeleteShader;
    // -> glDeleteShader(vertShader);
    // -> glDeleteShader(fragShader);

    return program;
}

int main(void) {

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Mi primera ventana de OpenGL owo", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Esta función sincroniza el frame rate de nuestra aplicacion con el VSync
    glfwSwapInterval(1);

    // Podemos inicializar glew solo en un contexto de renderizado válido de OpenGL
    if (glewInit() != GLEW_OK)
        std::cout << "Failed to initialize GLEW!" << '\n';

    std::cout << "Using OpenGL version " << glGetString(GL_VERSION) << '.\n';

    // Vengo del futuro y el core de OpenGL no nos da un Vertex Array Object (VAO) por defecto
    // por lo que crearemos uno antes de crear el buffer
    unsigned int vertexArrayObj;
    // desc: https://docs.gl/gl4/glGenVertexArrays
    GLCall(glGenVertexArrays(1, &vertexArrayObj));
    // luego lo bindeamos
    // desc: https://docs.gl/gl4/glBindVertexArray
    GLCall(glBindVertexArray(vertexArrayObj));

    // Ok, primero enviaremos un buffer de datos a la GPU.
    // glGenBuffer(numero de buffers, ID del buffer)
    // genera n buffers de datos.
    // &buffer actúa como el ID del buffer, con el que será reconocido por OpenGL.
    // Si quisiera "seleccionar" (bindear) este buffer, debo hacerlo con su ID.
    // desc: https://docs.gl/gl4/glGenBuffers
    unsigned int bufferID;
    glGenBuffers(1, &bufferID);

    // Aquí "seleccionamos" el buffer que acabamos de hacer (Bind).
    // GL_ARRAY_BUFFER nos dice que este buffer simplemente es un array de 1 dimensión.
    // buffer es el buffer ID que le asignaremos a este array.
    // Es como un "mappeo" (bufferID => GL_ARRAY_BUFFER)
    // desc: https://docs.gl/gl4/glBindBuffer
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);

    // Ponemos data en nuestro GL_ARRAY_BUFFER con ID bufferID.
    // Para eso tenemos que especificar su tamaño.
    // En este caso queremos guardar 6 vértices de 2 dimensiones (6 vector2) => (3x, 3y)
    // El tamaño de un GL_ARRAY_Buffer está en bytes.
    float dataToGPU[] = {
         -0.5f, -0.5f, // index 0
         0.5f,  -0.5f, // index 1
         0.5f,  0.5f, // index 2

         -0.5f,  0.5f, // index 3
    };

    // Recordar que los triángulos se dibujan en sentido antehorario
    unsigned int indices[] = {
        0, 1, 2, // Primer triángulo (que consiste de indices 0, 1, 2)
        2, 3, 0  // Segundo triángulo (que consiste de indices 2, 3, 0)
    };

    unsigned int indexBufferObj;
    glGenBuffers(1, &indexBufferObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // GLenum usages:
    // GL_STREAM = La data no se modificará y será usada un par de veces.
    // GL_STATIC = La data no se modificará y será usada muchas veces (como dibujar en el game loop).
    // GL_DYNAMIC = La data se modificará y se usará muchas veces (animar).
    // DRAW = Quieres dibujar el buffer de data.
    // READ = La data se usará para leer información de OpenGL y obtenerla luego.
    // COPY = La data se usará para leer información y dibujar en pantalla.
    // 
    // en este caso la data que le estamos pasando es un void* por lo que solo
    // es un montón de bytes, GL sabe que son floats, pero no sabe aún como interpretarlos, 
    // (podrían ser cualquier cosa, vértices, texturas, colores, normales, etc...
    // desc: https://docs.gl/gl4/glBufferData
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, &dataToGPU[0], GL_STATIC_DRAW);

    // Ahora indicaremos a OpenGL como interpretar la data que le dimos,
    // podemos hacer esto en cualquier momento,
    // pero afectará al buffer arbitrario que tengamos seleccionado.

    // Ok, aquí se pone loco:
    // index      : donde comienza el buffer de vertex/s
    // size       : de que tamaño son los atributos de datos que le doy? (ej: positions = 2)
    // normalized : remapea el valor a [0, 1]. ej: un color [0, 255] pasa a ser [0, 1], podemos hacerlo nosotros, pero OpenGL puede hacerlo por nosotros :)
    // stride     : representa cada cuantas bytes están separados los vertex (vertex = posicion + color + normal + etc... de 1 "objeto")
    // pointer    : es el puntero que salta entre los datos relevantes de nuestro vertex, (de la posición al color a la normal del vertex), luego se puede usar el macro offsetof.
        
    // Ej: 32 bytes
    //            * ->  *    ->     * ->    (* es el puntero que salta entre pos, col y tan
    //            | pos | col (red) | tan |
    // vertex 1 = |A5|B3|FF|00|00|FF|5F|9C| |00|00|00|00|00|00|00|00| (vertex 1 size = 1 stride = 16 bytes)
    // vertex 2 = |00|00|00|00|00|00|00|00| |00|00|00|00|00|00|00|00| 
    
    // Hay que activar el atributo primero eso si XD
    // desc: https://docs.gl/gl4/glEnableVertexAttribArray
    glEnableVertexAttribArray(0);
    // desc: https://docs.gl/gl4/glVertexAttribPointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);


    ShaderSource shaderSource = ShaderSource {
        ParseFile("res/shaders/shader_basic_vertex.glsl"),
        ParseFile("res/shaders/shader_basic_fragment.glsl")
    };
    unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    
    // Este programa será usado al hacer un draw call!
    // desc: https://docs.gl/gl4/glUseProgram
    glUseProgram(shader);

    // Obtenemos el lugar en donde está nuestro uniform en el shader
    // desc: https://docs.gl/gl4/glGetUniformLocation <- importante leer las restricciones
    int uniformLocation = glGetUniformLocation(shader, "u_Color");
    ASSERT(uniformLocation != -1);
    // Especificar el valor de un vec4 (4 floats)
    // desc: (de todos los posibles): https://docs.gl/gl4/glUniform
    glUniform4f(uniformLocation, 0.f, 1.0f, 1.0f, 1.0f);
    float red = 0.0f;
    float increment = 0.01f;

    // Mantener un loop hasta que la ventana se cierre
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // GL_TRIANGLES es un primitivo
        // glDrawArrays es un draw call!! owo
        // ¿Cómo sabe que buffer dibujar? Pues arriba lo seleccionamos con glBindBuffer!
        // Es como Photoshop, donde seleccionas una capa y luego el programa sabe
        // donde pintar, pues claramente es en la capa que tienes seleccionada :)
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        GLCall(glUniform4f(uniformLocation, red, 1.0f, 1.0f, 1.0f));

        // Draw call para dibujar elementos usando index buffers
        // el último parámetro es el puntero al index buffer
        // pero ya que está bindeado, no es necesario pasarlo.
        // desc: https://docs.gl/gl4/glDrawElements
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        // Animar una variable uniforme :)
        if      (red > 1.0f) increment = -0.01f;
        else if (red < 0.0f) increment =  0.01f;
        red += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}