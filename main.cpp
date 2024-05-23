#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <GLM/mat4x4.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

// Window dimensions
const GLint WIDTH = 1920;
const GLint HEIGHT = 1080;

GLuint VAO, VBO, shader, uniformModel;
const float toRadians = 3.141592653589793f / 180.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.6f;
float triIncrement = 0.005f;
float curAngle = 0.0f;
float rotSpeed = 1.1f;

bool sizeDirection = true;
float curSize = 0.5f;
float maxSize = 0.5f;
float minSize = 0.1f;
float sizeIncr = 0.05f;

// Vertex Shader
static const char* vShader = "						    					\n\
#version 330									   	      					\n\
												    						\n\
layout(location = 0) in vec3 pos;				       						\n\
uniform mat4 model;								         					\n\
void main(){										      					\n\
	gl_Position = model * vec4(pos, 1.0);	\n\
}															    			\n\
";

static const char* fShader = "									\n\
#version 330													\n\
																\n\
out vec4 color;													\n\
void main(){													\n\
	color = vec4(1.0, 0.0, 0.0, 1.0);							\n\
}																\n\
";

void createTriangle() {
	GLfloat vertices[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// GL Dynamic Draw allows us to change the points on the array.

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// Don't Stride

}

void addShader(GLint program, const char* shaderCode, GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Errlor compiling the %d shader: %s\n", shaderType, eLog);
		return;
	}

	glAttachShader(program, theShader);
}

void compileShaders() {
	shader = glCreateProgram();

	if (!shader) {
		printf("An error occured while creating shader program!");
		return;
	}

	addShader(shader, vShader, GL_VERTEX_SHADER);
	addShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Errlor Linking Program: %s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Errlor Linking Program: %s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
}

int main() {
	// Init GLFW
	if (!glfwInit()) {
		printf("GLFW failed to initialize!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Initialize window
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW Window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Init GLEW

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW init failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	createTriangle();
	compileShaders();

	while (!glfwWindowShouldClose(mainWindow)) {
		// Get and handle user input events
		glfwPollEvents();

		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) > triMaxOffset) {
			direction = !direction;
		}

		curAngle += rotSpeed;
		if (curAngle >= 360) {
			curAngle -= 360;
		}

		if(sizeDirection) {
		    curSize += sizeIncr;
        }else {
            curSize -= sizeIncr;
		}

		if(curSize >= maxSize || curSize <= minSize){
		    sizeDirection = !sizeDirection;
		}

		// Clear window
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glm::mat4 model(1.0f);

		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(triOffset, triOffset, 1.0f));


		glUniform1f(uniformModel, triOffset);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}
	return 0;
}
