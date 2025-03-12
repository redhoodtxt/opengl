#include <iostream>
#include <cstdio>
#include <math.h>

#include "shaders.h"
#include "textures.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// OpenGL only processes 3D coordinates when they're in a specific range between -1.0 and 1.0
float vertices[] = {
	// parallelogram
	// vertice pos 		vertice colour
	 1.0f,  0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f, 1.0f,// top right , purple 
	 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,// bottom right, red 
	-1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom left, blue 
	 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f// top left, purple 
};  

unsigned int indices[] = {
	 1, 2, 3, // first triangle 
	 0, 1, 3 // second triangle
};
// create shader object 
Shader theShader;

std::pair<std::string, std::string> shader_sources = theShader.processShaderCode("../../res/shaders/vs.glsl", "../../res/shaders/fs.glsl");
std::string vertex_shader_source = shader_sources.first;
std::string fragment_shader_source = shader_sources.second;

unsigned int genBuffers(){
    unsigned int VBO, VAO, EBO;

	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);  
    glGenBuffers(1,&VBO);

	glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // bind the VBO to its coressponding buffer type
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copies the vertex data into the buffer's memory

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	return VBO, VAO, EBO;
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // give the location of the vertex position attribute 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1); // give the location of the vertex colour attribute 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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

		theShader.use(program);

		// passing value into uniform variable in the shader
		// float time_value = glfwGetTime();
		// float red_value = (sin(time_value) / 2.0f) + 0.5f; // use sin for gradient
		// int vertex_color_location = glGetUniformLocation(program, "new_colour"); // get the location oF uniform
		// glUniform4f(vertex_color_location, red_value, 0.0f, 0.0f, 1.0f); // set the values for the uniform. last 4 params are rgba

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
	GLFWwindow* window = createWindow(2560, 1440, "textures" );
	initGLAD();
	configureOpenGL();

	// create texture object 
	Texture doomTexture("../../res/textures/doom.png");
	doomTexture.bind();

	unsigned int VBO, VAO = genBuffers(); // generate and bind VBO
	linkVertexData();

	unsigned int program = compileShader(vertex_shader_source, fragment_shader_source);

	renderLoop(window, program, VAO);

	cleanup(window);

    return 0;
}