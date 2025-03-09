#include <iostream>
#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// OpenGL only processes 3D coordinates when they're in a specific range between -1.0 and 1.0
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};  

const char *vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragment_shader_source = "#version 330 core\n"
    "out vec4 frag_colour;\n"
    "void main()\n"
    "{\n"
    "   frag_colour = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\0";

unsigned int genBuffers(){
    unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);  
    glGenBuffers(1,&VBO);
	glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // bind the VBO to its coressponding buffer type
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copies the vertex data into the buffer's memory

	return VBO, VAO;
}

static unsigned int createShader(unsigned int type, const std::string& source){
	unsigned int id = glCreateShader(type); // create a shader object
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, NULL); // add the shader source code to the shader object created

	return id;
}

static int compileShader(const std::string& vertex_shader, const std::string& fragment_shader){ // TODO: change parameter type
	unsigned int program = glCreateProgram(); // sticking to cpp type instead of GLuint

	unsigned int vshader = createShader(GL_VERTEX_SHADER, vertex_shader); // create vertex shader
	glCompileShader(vshader); // compile fragment shader
 
	int isCompiled; // flag to check if compiled 
	char info_log[512]; // array to store the info log 

	// check if vertex shader compilation successful
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &isCompiled); // get the compile status 
	if(!isCompiled){
		glGetShaderInfoLog(vshader, 512, NULL, info_log);
		std::cout << "ERROR: vertex shader compilation failed\n" << info_log << std::endl; 
	}

	unsigned int fshader = createShader(GL_FRAGMENT_SHADER,fragment_shader); // create fragment shader
	glCompileShader(fshader); // compile fragment shader

	// check if fragment shader compilation successful 
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &isCompiled);
	if(!isCompiled){
		glGetShaderInfoLog(fshader, 512, NULL, info_log);
		std::cout << "ERROR: fragment shader compilation failed\n" << info_log << std::endl;
	}

	// attach the 2 shaders to the program
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program); // link the program


	glValidateProgram(program);

	glDeleteShader(vshader);
	glDeleteShader(fshader);

	return program; 

}

void linkVertexData(){
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // give the location of the vertex attribute 

}

void initGLFW(){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* createWindow(int width, int height, const char* title){
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
    }
    glfwMakeContextCurrent(window);
    return window; 
} 

bool initGLAD(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cout << "Failed to initialize GLAD" << std::endl;
    return false;
	}
	return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void configureOpenGL(){
	glViewport(0, 0, 2560, 1440);
	glfwSetFramebufferSizeCallback(glfwGetCurrentContext(), framebuffer_size_callback); // allow for resizing
	glClearColor(0.00f, 0.00f, 0.00f, 0.00f); // black bg
	glClear(GL_COLOR_BUFFER_BIT); // clear back buffer
	glfwSwapBuffers(glfwGetCurrentContext()); // swap the back buffer with the front
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // escape key pressed to exit program
        glfwSetWindowShouldClose(window, true);
}

void renderLoop(GLFWwindow* window, unsigned int program, unsigned int VAO){
	while(!glfwWindowShouldClose(window)){
		processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // bg colour
        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void cleanup(GLFWwindow* window){
	glfwDestroyWindow(window);
	glfwTerminate();
}

int main(){
    initGLFW();
	GLFWwindow* window = createWindow(2560, 1440, "hello-triangle" );
	initGLAD();
	configureOpenGL();

	unsigned int VBO, VAO = genBuffers(); // generate and bind VBO
	linkVertexData();

	unsigned int program = compileShader(vertex_shader_source, fragment_shader_source);

	renderLoop(window, program, VAO);

	cleanup(window);

    return 0;

}