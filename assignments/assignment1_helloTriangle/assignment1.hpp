#pragma once
#include <iostream>
#include <cstdio>

#include "cobb/shader.hpp"
#include "cobb/window.hpp"
#include <ew/external/glad.h>
#include <GLFW/glfw3.h>



using namespace cobb;

const float vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
};

int assignment1() {
	const auto window = Window("Hello ∆");

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);


	auto triangleShader = Shader("assets/assignment1/triangle.vert", "assets/assignment1/triangle.frag");
    triangleShader.use();


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 4 , GL_FLOAT, GL_FALSE, 7 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);





	//Render loop
	while (!glfwWindowShouldClose(window.window)) {
		glfwPollEvents();

		//Clear framebuffer
		glClear(GL_COLOR_BUFFER_BIT);
		//Drawing happens here!
        triangleShader.setFloat("time", static_cast<float>(glfwGetTime()));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window.window);
	}
	printf("Shutting down...");
	return 0;
}
