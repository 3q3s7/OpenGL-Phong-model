#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include "shader.h"

//��������
//������������
void genrateSphere();
//��Ӧ�����ƶ��¼�
void key_callback(GLFWwindow* window, int key);
//��������
void processInput(GLFWwindow* window);
//�������
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//���ڷֱ�������Ӧ
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//��������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//����������
void DrawCube(GLFWwindow* window);
//��������
void DrawSphere(GLFWwindow* window);

const GLfloat  PI = 3.14159265358979323846f;
glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);//��������屾�����ɫ
glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);//�Ƶ���ɫ
//������ݻ��ֳ�50X50������
const int Y_SEGMENTS = 200;
const int X_SEGMENTS = 200;
//��Ķ������ݺ�ƬԪ����
std::vector<float> sphereVertices;
std::vector<int> sphereIndices;

//����������
const float vertices[] = {                  //����������
	// ---- λ�� ----       ---- ���� ----   
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

float screen_width = 1280.0f;          //���ڿ��
float screen_height = 720.0f;          //���ڸ߶�

//�������
glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f);     //�����λ��
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, 0.0f);       //���������
glm::vec3 camera_up = glm::vec3(0.0f, 2.0f, 1.0f);           //�����������
GLfloat Yaw = -90.0f;;//ƫ����
GLfloat Pitch = 0.0f;//������
GLfloat cameraSpeed = 0.05f;//�ƶ��ٶ�
GLfloat MouseSensitivity = 0.1f;//���������
float fov = 45.0f;//��Ұ
bool firstMouse = true;
//���ָ������λ��
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;

//���ղ���
float ambientStrength = 0.1f;//������ǿ��
float specularStrength = 0.5f;//����ǿ��
glm::vec3 lightPos = glm::vec3(0.0f, 1.0f, 1.0f);//��Դ��λ��

int main() {
	bool isSphere;
	std::cout << "0.��ʾ����\n" << "1.��ʾ��������" << std::endl;
	std::cout << "������0��1��";
	std::cin >> isSphere;

	// ��ʼ��GLFW
	glfwInit();                                                     // ��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // OpenGL�汾Ϊ3.3�����ΰ汾�ž���Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // ʹ�ú���ģʽ(������������)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // ���ʹ�õ���Mac OS Xϵͳ�����������
	glfwWindowHint(GLFW_RESIZABLE, 0);						    // ���ɸı䴰�ڴ�С

																	// ��������(���ߡ���������)
	auto window = glfwCreateWindow(screen_width, screen_height, "Cube", nullptr, nullptr);
	if (window == nullptr) {                                        // ������ڴ���ʧ�ܣ����Failed to Create OpenGL Context
		std::cout << "Failed to Create OpenGL Context" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);                                 // �����ڵ�����������Ϊ��ǰ�̵߳���������

																	// ��ʼ��GLAD������OpenGL����ָ���ַ�ĺ���
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ָ����ǰ�ӿڳߴ�(ǰ��������Ϊ���½�λ�ã���������������Ⱦ���ڿ���)
	glViewport(0, 0, screen_width, screen_height);

	if (isSphere) {
		DrawCube(window);//����������
	}
	else {
		DrawSphere(window);//��������
	}

	// �������е���Դ����ȷ�˳�����
	glfwTerminate();
	return 0;
}


//��Ӧ�����ƶ��¼�
void key_callback(GLFWwindow* window, int key)
{
	float Speed = (cameraSpeed - 0.04);
	if (key == GLFW_KEY_Q)
		camera_position += cameraSpeed * camera_front;	//��ǰ
	if (key == GLFW_KEY_E)
		camera_position -= cameraSpeed * camera_front;	//���
	if (key == GLFW_KEY_W)
		camera_position += Speed * camera_up;			//����
	if (key == GLFW_KEY_S)
		camera_position -= Speed * camera_up;			//����
	if (key == GLFW_KEY_A)
		camera_position -= glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;	//����
	if (key == GLFW_KEY_D)
		camera_position += glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;	//����
}

//��������
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		key_callback(window, GLFW_KEY_W);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		key_callback(window, GLFW_KEY_S);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		key_callback(window, GLFW_KEY_A);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		key_callback(window, GLFW_KEY_D);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		key_callback(window, GLFW_KEY_Q);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		key_callback(window, GLFW_KEY_E);
}

//���ڷֱ�������Ӧ
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//�������
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)//��һ�εõ������Ϣʱֻ��Ҫ�������꼴��
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;//���X��ƫ����
	GLfloat yoffset = lastY - ypos;//���Y��ƫ����
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	Yaw += xoffset;//�ı�ƫ����
	Pitch += yoffset;//�ı丩����

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	glm::vec3 front;//���㷽����������������
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	camera_front = glm::normalize(front);//�������滯������ɵ�λ����
}

//�ӽ����ţ�ͨ�����ֿ��ƣ�
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

//������������
void genrateSphere() {
	// ������Ķ���
	for (int y = 0; y <= Y_SEGMENTS; y++)
	{
		for (int x = 0; x <= X_SEGMENTS; x++)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			sphereVertices.push_back(xPos);
			sphereVertices.push_back(yPos);
			sphereVertices.push_back(zPos);

		}
	}

	// �������Indices
	for (int i = 0; i < Y_SEGMENTS; i++)
	{
		for (int j = 0; j < X_SEGMENTS; j++)
		{
			sphereIndices.push_back(i * (X_SEGMENTS+1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);

			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
		}
	}
}

void DrawCube(GLFWwindow* window) {
	Shader shader("res/shader/task-cube.vs", "res/shader/task-cube.fs");//������ɫ��
	Shader lightShader("res/shader/task-light.vs", "res/shader/task-light.fs");//������ɫ��

	// ���ɲ���VAO��VBO
	GLuint vertex_array_object; // == VAO
	GLuint vertex_buffer_object; // == VBO
	GLuint lightVAO; // == VAO
	GLuint lightVBO; // == VBO
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	// ���������ݰ�����ǰĬ�ϵĻ�����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// ���ɲ��󶨵Ƶ�VAO��VBO
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	// ���������ݰ�����ǰĬ�ϵĻ�����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);//������Ȳ��ԣ�ʵ����ȷ���ڵ���ϵ
		
	// Render loop��ѭ��
	while (!glfwWindowShouldClose(window)) {
		//������ѭ����������ɫ������Ȼ���
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//������ɫ�������Ȼ���

		//�������������������ͼ
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		processInput(window);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//������������
		shader.Use();
		// Transform����任����
		glm::mat4 model(1);//model���󣬾ֲ�����任����������
		model = glm::translate(model, glm::vec3(0.0,0.0,0.0));
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f,1.0f,1.0f));
		glm::mat4 view(1);//view������������任���۲�����ϵ
		view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
		glm::mat4 projection(1);//projection����ͶӰ����
		projection = glm::perspective(glm::radians(fov), (float)screen_width / screen_height, 0.1f, 100.0f);
		// ����ɫ���д������
		int model_location = glGetUniformLocation(shader.ID, "model"); //��ȡ��ɫ����ĳ��������λ��
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));//д�����ֵ
		int view_location = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
		int projection_location = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
		shader.SetVec3("objectColor", objectColor);
		shader.SetVec3("lightColor", lightColor);
		shader.SetFloat("ambientStrength", ambientStrength);
		shader.SetFloat("specularStrength", specularStrength);
		shader.SetVec3("lightPos", lightPos);
		shader.SetVec3("viewPos", camera_position);
		//����������
		glBindVertexArray(vertex_array_object);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		//�ƻ������
		lightShader.Use();
		// Transform����任����
		glm::mat4 lightmodel(1);//model���󣬾ֲ�����任����������

		lightmodel = glm::translate(lightmodel, lightPos);
		//lightmodel = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		lightmodel = glm::scale(lightmodel, glm::vec3(0.2f));

		// ����ɫ���д������
		int _model_location = glGetUniformLocation(lightShader.ID, "model"); //��ȡ��ɫ����ĳ��������λ��
		glUniformMatrix4fv(_model_location, 1, GL_FALSE, glm::value_ptr(lightmodel));//д�����ֵ
		int _view_location = glGetUniformLocation(lightShader.ID, "view");
		glUniformMatrix4fv(_view_location, 1, GL_FALSE, glm::value_ptr(view));
		int _projection_location = glGetUniformLocation(lightShader.ID, "projection");
		glUniformMatrix4fv(_projection_location, 1, GL_FALSE, glm::value_ptr(projection));
		lightShader.SetVec3("lightColor", lightColor);
		//����
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//�ͷ�VAOVBO
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteBuffers(1, &vertex_buffer_object);
}

void DrawSphere(GLFWwindow* window) {
	Shader shader("res/shader/task3.vs", "res/shader/task3.fs");//������ɫ��
	Shader lightShader("res/shader/task-light.vs", "res/shader/task-light.fs");//������ɫ��

	genrateSphere();//������������

	// ���ɲ���VAO��VBO
	GLuint vertex_array_object; // == VAO
	GLuint vertex_buffer_object; // == VBO
	GLuint element_buffer_object; //EBO
	GLuint lightVAO; // == VAO
	GLuint lightVBO; // == VBO

	// ��
	glGenVertexArrays(1, &vertex_array_object);
	glGenBuffers(1, &vertex_buffer_object);
	//���ɲ��������VAO��VBO
	glBindVertexArray(vertex_array_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	// ���������ݰ�����ǰĬ�ϵĻ�����
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &element_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);
	// ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ���VAO��VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// ���ɲ��󶨵Ƶ�VAO��VBO
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	// ���������ݰ�����ǰĬ�ϵĻ�����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);//��Ȳ���

	while (!glfwWindowShouldClose(window)) {
		//������ѭ����������ɫ������Ȼ���
		// �����ɫ����
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthMask(GL_FALSE);

		////����������
		shader.Use();
		//�������������������ͼ
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		processInput(window);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// Transform����任����
		glm::mat4 model(1);//model���󣬾ֲ�����任����������
		model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 view(1);//view������������任���۲�����ϵ
		view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
		glm::mat4 projection(1);//projection����ͶӰ����
		projection = glm::perspective(glm::radians(fov), (float)screen_width / screen_height, 0.1f, 100.0f);
		// ����ɫ���д������
		int model_location = glGetUniformLocation(shader.ID, "model"); //��ȡ��ɫ����ĳ��������λ��
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));//д�����ֵ
		int view_location = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
		int projection_location = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
		shader.SetVec3("objectColor", objectColor);
		shader.SetVec3("lightColor", lightColor);
		shader.SetFloat("ambientStrength", ambientStrength);
		shader.SetFloat("specularStrength", specularStrength);
		shader.SetVec3("lightPos", lightPos);
		shader.SetVec3("viewPos", camera_position);
		//������
		//�������޳�(ֻ��Ҫչʾһ���棬��������غ�)
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);		//ʹ��˳ʱ���������ʱ������ʾ����
		glBindVertexArray(vertex_array_object);
		//ʹ���߿�ģʽ����
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
		//����ģʽ����
		//glPointSize(5);
		//glDrawElements(GL_POINTS, X_SEGMENTS*Y_SEGMENTS*6, GL_UNSIGNED_INT, 0);



		//�ƻ������
		lightShader.Use();
		// Transform����任����
		glm::mat4 lightmodel(1);//model���󣬾ֲ�����任����������
		lightmodel = glm::translate(lightmodel, lightPos);
		//lightmodel = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		lightmodel = glm::scale(lightmodel, glm::vec3(0.2f));
		// ����ɫ���д������
		int _model_location = glGetUniformLocation(lightShader.ID, "model"); //��ȡ��ɫ����ĳ��������λ��
		glUniformMatrix4fv(_model_location, 1, GL_FALSE, glm::value_ptr(lightmodel));//д�����ֵ
		int _view_location = glGetUniformLocation(lightShader.ID, "view");
		glUniformMatrix4fv(_view_location, 1, GL_FALSE, glm::value_ptr(view));
		int _projection_location = glGetUniformLocation(lightShader.ID, "projection");
		glUniformMatrix4fv(_projection_location, 1, GL_FALSE, glm::value_ptr(projection));
		lightShader.SetVec3("lightColor", lightColor);
		//����
		glDisable(GL_CULL_FACE);	//�ر����޳���ʹ����������������
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 36);

		//�������岢�Ҽ���Ƿ��д����¼�(����������롢����ƶ��ȣ�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//�ͷ�VAOVBO
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteBuffers(1, &vertex_buffer_object);
	glDeleteBuffers(1, &element_buffer_object);
	glDeleteBuffers(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);
}
