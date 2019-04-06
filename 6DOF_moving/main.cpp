//�������ó���������,��������С����6DOF�˶�
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// ����
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

float factor = 0.2f;	

//����õ�36�����㶼�г�����6���� * ÿ����2�������� * 
//ÿ��������3�����㣩��ÿ���涥�㶼��������������
//δʹ������EBO������
float vertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,//=
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,//==
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,//==
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,//=

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

//����10��λ��
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	//glm::vec3(2.0f,  5.0f, -15.0f),
	//glm::vec3(-1.5f, -2.2f, -2.5f),
	//glm::vec3(-3.8f, -2.0f, -12.3f),
	//glm::vec3(2.4f, -0.4f, -3.5f),
	//glm::vec3(-1.7f,  3.0f, -7.5f),
	//glm::vec3(1.3f, -2.0f, -2.5f),
	//glm::vec3(1.5f,  2.0f, -2.5f),
	//glm::vec3(1.5f,  0.2f, -1.5f),
	//glm::vec3(-1.3f,  1.0f, -1.5f)
};

float pitch = 0.0;
float yaw = 0.0;
float roll = 0.0;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//��ȡ�����������
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "���֧�ֵ���������Ϊ��" << nrAttributes << std::endl;

	//������Ȳ���
	//OpenGL�洢�����Ϣ��һ������Z����(Z-buffer)�Ļ����У�������OpenGL������ʱ����һ�����ض���ʱ�����ǡ�
	glEnable(GL_DEPTH_TEST);

	//���������ǵ���ɫ��
	Shader shader("Shader282.vs", "Shader282.fs");

	//��һ������====================================================
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//��������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��������
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//unsigned int EBO;//����δʹ��EBO����
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//����1
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//����������װ�͹��˵ķ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(/*"zhou_yu.bmp"*/"../container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "�޷��������⣬����������Դ�Ƿ�����" << std::endl;
	stbi_image_free(data);

	//����2
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);	//��������1����
	glBindTexture(GL_TEXTURE_2D, texture2);
	//�����������ƺ͹��˵ķ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//����ͼƬ
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data2 = stbi_load("../awesomeface.png", &width, &height, &nrChannels, 0);
	if (data2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "�޷��������⣬����������Դ�Ƿ�����" << std::endl;
	stbi_image_free(data2);

	//����OpenGL�ĸ������������ĸ�������Ԫ
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);


	//�۲����Z�����ƽ�ƣ���
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -8.0f));

	//ͶӰ����͸��ͶӰ��
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//��Ϊʹ������Ȳ��ԣ�Ҫ��ÿ����Ⱦ����֮ǰ�����Ȼ��壨����ǰһ֡�������Ϣ��Ȼ�����ڻ����У�
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader.use();
		glBindVertexArray(VAO);//==========

		//1�������ƶ�
		glm::mat4 model;
		model = glm::translate(model, cubePositions[0]);
		model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));//x ����
		model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));//y ����
		model = glm::rotate(model,glm::radians(roll), glm::vec3(0.0f, 0.0f, 0.1f));//z
		shader.setMat4("model", glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		shader.setMat4("view", glm::value_ptr(view));//�۲����
		shader.setMat4("projection", glm::value_ptr(projection));//ͶӰ����

		//����=========
		glm::mat4 model3;
		model3 = glm::translate(model3, glm::vec3(0.0f, -3.0f, -10.0f));
		model3 = glm::rotate(model3, glm::radians(-90.0f),glm::vec3(1.0f, 0.0f, 0.0f));
		model3 = glm::scale(model3, glm::vec3(25.5, 25.5, 1.0));
		shader.setMat4("model", glm::value_ptr(model3));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)//ǰ
		cubePositions[0].z -= 0.01;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)//��
		cubePositions[0].z += 0.01;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)//��
		cubePositions[0].x -= 0.01;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)//��
		cubePositions[0].x += 0.01;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)//��
		cubePositions[0].y += 0.01;

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)//��
		cubePositions[0].y -= 0.01;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)//
	{
		pitch += 0.1;
		if (pitch >= 35) pitch = 35;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)//
	{
		pitch -= 0.1;
		if (pitch <= -35) pitch = -35;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)//
	{
		yaw += 0.1;
		if (yaw >= 35) yaw = 35;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)//
	{
		yaw -= 0.1;
		if (yaw <= -35) yaw = -35;
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)//
	{
		roll += 0.1;
		if (roll >= 35) roll = 35;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)//
	{
		roll -= 0.1;
		if (roll <= -35) roll = -35;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}