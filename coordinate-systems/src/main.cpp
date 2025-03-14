#include <iostream>
#include <cstdio>
#include <math.h>

#include "shaders.h"
#include "textures.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
int screen_width = 2560;
int screen_height = 1440;

// OpenGL only processes 3D coordinates when they're in a specific range between -1.0 and 1.0
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

// define 10 cubes 
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};


// create shader object 
Shader theShader;

std::pair<std::string, std::string> shader_sources = theShader.processShaderCode("../../res/shaders/vs.glsl", "../../res/shaders/fs.glsl");
std::string vertex_shader_source = shader_sources.first;
std::string fragment_shader_source = shader_sources.second;

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // give the location of the vertex position attribute 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1); // give the location of the vertex texture attribute 	

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

		// initialize model view projection matrix 
		glm::mat4 model = glm::mat4(1.0f); // model matrix 
		glm::mat4 view = glm::mat4(1.0f); // view matrix 
		glm::mat4 projection; // projection matrix 

		/*
		transofrmations:
		Tilted backwards to the floor.
		A bit farther away from us.
		Be displayed with perspective (it should get smaller, the further its vertices are).
		*/
		// model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f,1.0f,1.0f)); // rotate by speed of 50 deg over time
		view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f)); // move the scene backward (camera forward)
		projection = glm::perspective(glm::radians(45.0f), float(screen_width)/float(screen_height), 0.1f, 100.0f);
		theShader.use(program);

		
		unsigned int view_loc = glGetUniformLocation(program, "view");
		unsigned int projection_loc = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &projection[0][0]);

		// enable depth testing
		glEnable(GL_DEPTH_TEST);  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear depth buffer

		glBindVertexArray(VAO);
		for(unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle;
			if (i == 0){
				 angle = 20.0f;
			} else{
				angle = 20.0f * i; 
			}
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			unsigned int model_loc = glGetUniformLocation(program, "model");
			glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);
		
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// glDrawArrays(GL_TRIANGLES, 0, 36);

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
	GLFWwindow* window = createWindow(screen_width, screen_height, "coordinate-systems" );
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