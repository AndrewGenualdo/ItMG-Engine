#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

using namespace std;

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform float time;

out vec3 pos;
out vec4 color;

vec2 rotatePoint(vec2 origin, float angleInRad, vec2 point) {

	vec2 rotatedPoint = point - origin;
	float s = sin(angleInRad);
	float c = cos(angleInRad);
	float xnew = rotatedPoint.x * c - rotatedPoint.y * s;
	float ynew = rotatedPoint.x * s + rotatedPoint.y * c;
	return vec2(xnew, ynew) + origin;

}

void main() {
	//vec4 posOut = vec4(aPos.x + sin(time) / 4.0f, aPos.y + cos(time) / 4.0f, aPos.z, 1.0);
	//gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);//posOut;
	pos = aPos;
	color = aColor;

	vec2 centerOfTriangle = vec2(0.0f, -0.167f);
	float adjustedTime = time * 2.0f * 3.141592653589f / 3.0f;
	float cosAdjTime = cos(adjustedTime);
	float sinAdjTime = sin(adjustedTime);
	float scale = ((sin(time * 2.5f + aPos.x + aPos.y) + 1) / 2.0f) / 2.0f + 0.2f;
	vec2 rotatedPoint = rotatePoint(centerOfTriangle, adjustedTime, vec2(aPos.x * scale, aPos.y * scale));
	//vec2 rotatedPoint = aPos.xy;
	gl_Position = vec4(rotatedPoint.x, rotatedPoint.y, aPos.z, 1.0f);
}
)";


const char *fragmentShaderSource = R"(
#version 330 core
in vec3 pos;
in vec4 color;

uniform float time;

out vec4 FragColor;


void main() {
	float b1 = (pos.y + 0.5f) * 2;
	float a1 = (-(pos.x - 0.5) * 2) - b1 / 2f;
	float c1 = ((pos.x + 0.5f) * 2) - b1 / 2f;

	float a = a1 / (a1 + b1 + c1) * 1.3333f;
	float b = b1 / (a1 + b1 + c1) * 1.3333f;
	float c = c1 / (a1 + b1 + c1) * 1.3333f;

	vec4 rgb;

	int timeStamp = 3; //must be multiple of 3

	int stage = int(time) % timeStamp;
	float fl = ((stage % (timeStamp / 3)) + (time - int(time))) / (timeStamp / 3);
	if(stage < timeStamp / 3) {
		rgb.r = mix(a, b, fl);
		rgb.g = mix(b, c, fl);
		rgb.b = mix(c, a, fl);
	} else if(stage >= timeStamp * 2 / 3) {
		rgb.r = mix(c, a, fl);
		rgb.g = mix(a, b, fl);
		rgb.b = mix(b, c, fl);
	} else {
		rgb.r = mix(b, c, fl);
		rgb.g = mix(c, a, fl);
		rgb.b = mix(a, b, fl);
	}


	//FragColor = vec4(r, g, b, 1.0f);
	//FragColor = vec4(sin(time), 0.0f, 0.0f, 1.0f);
	FragColor = vec4(rgb.r, rgb.g, rgb.b, 1.0f);
	//FragColor = color;
}
)";

const float vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
};

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello ∆", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	//Initialization goes here!
	glClearColor(0.0f, 0.1f, 0.2f, 1.0f);\

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}


	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 4 , GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);





	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Clear framebuffer
		glClear(GL_COLOR_BUFFER_BIT);
		//Drawing happens here!

		int timeLocation = glGetUniformLocation(shaderProgram, "time");
		glUseProgram(shaderProgram);
		glUniform1f(timeLocation, static_cast<float>(glfwGetTime()));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
