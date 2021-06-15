#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include "shader.h"

//函数声明
//生成球体数据
void genrateSphere();
//对应键盘移动事件
void key_callback(GLFWwindow* window, int key);
//键盘输入
void processInput(GLFWwindow* window);
//鼠标输入
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//窗口分辨率自适应
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//滑轮输入
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//绘制正方体
void DrawCube(GLFWwindow* window);
//绘制球面
void DrawSphere(GLFWwindow* window);

const GLfloat  PI = 3.14159265358979323846f;
glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);//球和立方体本身的颜色
glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);//灯的颜色
//将球横纵划分成50X50的网格
const int Y_SEGMENTS = 200;
const int X_SEGMENTS = 200;
//球的顶点数据和片元数据
std::vector<float> sphereVertices;
std::vector<int> sphereIndices;

//立方体数据
const float vertices[] = {                  //立方体数组
	// ---- 位置 ----       ---- 法线 ----   
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

float screen_width = 1280.0f;          //窗口宽度
float screen_height = 720.0f;          //窗口高度

//相机参数
glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f);     //摄像机位置
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, 0.0f);       //摄像机方向
glm::vec3 camera_up = glm::vec3(0.0f, 2.0f, 1.0f);           //摄像机上向量
GLfloat Yaw = -90.0f;;//偏航角
GLfloat Pitch = 0.0f;//俯仰角
GLfloat cameraSpeed = 0.05f;//移动速度
GLfloat MouseSensitivity = 0.1f;//鼠标灵敏度
float fov = 45.0f;//视野
bool firstMouse = true;
//鼠标指针最后的位置
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;

//光照参数
float ambientStrength = 0.1f;//环境光强度
float specularStrength = 0.5f;//镜面强度
glm::vec3 lightPos = glm::vec3(0.0f, 1.0f, 1.0f);//光源的位置

int main() {
	bool isSphere;
	std::cout << "0.显示球面\n" << "1.显示正方体面" << std::endl;
	std::cout << "请输入0或1：";
	std::cin >> isSphere;

	// 初始化GLFW
	glfwInit();                                                     // 初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // OpenGL版本为3.3，主次版本号均设为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 使用核心模式(无需向后兼容性)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 如果使用的是Mac OS X系统，需加上这行
	glfwWindowHint(GLFW_RESIZABLE, 0);						    // 不可改变窗口大小

																	// 创建窗口(宽、高、窗口名称)
	auto window = glfwCreateWindow(screen_width, screen_height, "Cube", nullptr, nullptr);
	if (window == nullptr) {                                        // 如果窗口创建失败，输出Failed to Create OpenGL Context
		std::cout << "Failed to Create OpenGL Context" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);                                 // 将窗口的上下文设置为当前线程的主上下文

																	// 初始化GLAD，加载OpenGL函数指针地址的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 指定当前视口尺寸(前两个参数为左下角位置，后两个参数是渲染窗口宽、高)
	glViewport(0, 0, screen_width, screen_height);

	if (isSphere) {
		DrawCube(window);//绘制正方体
	}
	else {
		DrawSphere(window);//绘制球面
	}

	// 清理所有的资源并正确退出程序
	glfwTerminate();
	return 0;
}


//对应键盘移动事件
void key_callback(GLFWwindow* window, int key)
{
	float Speed = (cameraSpeed - 0.04);
	if (key == GLFW_KEY_Q)
		camera_position += cameraSpeed * camera_front;	//向前
	if (key == GLFW_KEY_E)
		camera_position -= cameraSpeed * camera_front;	//向后
	if (key == GLFW_KEY_W)
		camera_position += Speed * camera_up;			//向上
	if (key == GLFW_KEY_S)
		camera_position -= Speed * camera_up;			//向下
	if (key == GLFW_KEY_A)
		camera_position -= glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;	//向左
	if (key == GLFW_KEY_D)
		camera_position += glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;	//向右
}

//键盘输入
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

//窗口分辨率自适应
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//鼠标输入
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)//第一次得到鼠标信息时只需要保存坐标即可
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;//获得X轴偏移量
	GLfloat yoffset = lastY - ypos;//获得Y轴偏移量
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	Yaw += xoffset;//改变偏航角
	Pitch += yoffset;//改变俯仰角

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	glm::vec3 front;//计算方向向量的三个分量
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	camera_front = glm::normalize(front);//进行正规化，即变成单位向量
}

//视角缩放（通过滚轮控制）
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

//生成球体数据
void genrateSphere() {
	// 生成球的顶点
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

	// 生成球的Indices
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
	Shader shader("res/shader/task-cube.vs", "res/shader/task-cube.fs");//加载着色器
	Shader lightShader("res/shader/task-light.vs", "res/shader/task-light.fs");//加载着色器

	// 生成并绑定VAO和VBO
	GLuint vertex_array_object; // == VAO
	GLuint vertex_buffer_object; // == VBO
	GLuint lightVAO; // == VAO
	GLuint lightVBO; // == VBO
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	// 将顶点数据绑定至当前默认的缓冲中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// 生成并绑定灯的VAO和VBO
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	// 将顶点数据绑定至当前默认的缓冲中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);//开启深度测试，实现正确的遮挡关系
		
	// Render loop主循环
	while (!glfwWindowShouldClose(window)) {
		//进入主循环，清理颜色缓冲深度缓冲
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清理颜色缓冲和深度缓冲

		//键盘与鼠标控制摄像机视图
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		processInput(window);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//正方体绘制相关
		shader.Use();
		// Transform坐标变换矩阵
		glm::mat4 model(1);//model矩阵，局部坐标变换至世界坐标
		model = glm::translate(model, glm::vec3(0.0,0.0,0.0));
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f,1.0f,1.0f));
		glm::mat4 view(1);//view矩阵，世界坐标变换至观察坐标系
		view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
		glm::mat4 projection(1);//projection矩阵，投影矩阵
		projection = glm::perspective(glm::radians(fov), (float)screen_width / screen_height, 0.1f, 100.0f);
		// 向着色器中传入参数
		int model_location = glGetUniformLocation(shader.ID, "model"); //获取着色器内某个参数的位置
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));//写入参数值
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
		//绘制正方体
		glBindVertexArray(vertex_array_object);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		//灯绘制相关
		lightShader.Use();
		// Transform坐标变换矩阵
		glm::mat4 lightmodel(1);//model矩阵，局部坐标变换至世界坐标

		lightmodel = glm::translate(lightmodel, lightPos);
		//lightmodel = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		lightmodel = glm::scale(lightmodel, glm::vec3(0.2f));

		// 向着色器中传入参数
		int _model_location = glGetUniformLocation(lightShader.ID, "model"); //获取着色器内某个参数的位置
		glUniformMatrix4fv(_model_location, 1, GL_FALSE, glm::value_ptr(lightmodel));//写入参数值
		int _view_location = glGetUniformLocation(lightShader.ID, "view");
		glUniformMatrix4fv(_view_location, 1, GL_FALSE, glm::value_ptr(view));
		int _projection_location = glGetUniformLocation(lightShader.ID, "projection");
		glUniformMatrix4fv(_projection_location, 1, GL_FALSE, glm::value_ptr(projection));
		lightShader.SetVec3("lightColor", lightColor);
		//绘制
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//释放VAOVBO
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteBuffers(1, &vertex_buffer_object);
}

void DrawSphere(GLFWwindow* window) {
	Shader shader("res/shader/task3.vs", "res/shader/task3.fs");//加载着色器
	Shader lightShader("res/shader/task-light.vs", "res/shader/task-light.fs");//加载着色器

	genrateSphere();//生成球体数据

	// 生成并绑定VAO和VBO
	GLuint vertex_array_object; // == VAO
	GLuint vertex_buffer_object; // == VBO
	GLuint element_buffer_object; //EBO
	GLuint lightVAO; // == VAO
	GLuint lightVBO; // == VBO

	// 球
	glGenVertexArrays(1, &vertex_array_object);
	glGenBuffers(1, &vertex_buffer_object);
	//生成并绑定球体的VAO和VBO
	glBindVertexArray(vertex_array_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	// 将顶点数据绑定至当前默认的缓冲中
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &element_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);
	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 解绑VAO和VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 生成并绑定灯的VAO和VBO
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	// 将顶点数据绑定至当前默认的缓冲中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);//深度测试

	while (!glfwWindowShouldClose(window)) {
		//进入主循环，清理颜色缓冲深度缓冲
		// 清空颜色缓冲
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthMask(GL_FALSE);

		////球体绘制相关
		shader.Use();
		//键盘与鼠标控制摄像机视图
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		processInput(window);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// Transform坐标变换矩阵
		glm::mat4 model(1);//model矩阵，局部坐标变换至世界坐标
		model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 view(1);//view矩阵，世界坐标变换至观察坐标系
		view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
		glm::mat4 projection(1);//projection矩阵，投影矩阵
		projection = glm::perspective(glm::radians(fov), (float)screen_width / screen_height, 0.1f, 100.0f);
		// 向着色器中传入参数
		int model_location = glGetUniformLocation(shader.ID, "model"); //获取着色器内某个参数的位置
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));//写入参数值
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
		//绘制球
		//开启面剔除(只需要展示一个面，否则会有重合)
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);		//使用顺时针而不是逆时针来表示正面
		glBindVertexArray(vertex_array_object);
		//使用线框模式绘制
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
		//点阵模式绘制
		//glPointSize(5);
		//glDrawElements(GL_POINTS, X_SEGMENTS*Y_SEGMENTS*6, GL_UNSIGNED_INT, 0);



		//灯绘制相关
		lightShader.Use();
		// Transform坐标变换矩阵
		glm::mat4 lightmodel(1);//model矩阵，局部坐标变换至世界坐标
		lightmodel = glm::translate(lightmodel, lightPos);
		//lightmodel = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		lightmodel = glm::scale(lightmodel, glm::vec3(0.2f));
		// 向着色器中传入参数
		int _model_location = glGetUniformLocation(lightShader.ID, "model"); //获取着色器内某个参数的位置
		glUniformMatrix4fv(_model_location, 1, GL_FALSE, glm::value_ptr(lightmodel));//写入参数值
		int _view_location = glGetUniformLocation(lightShader.ID, "view");
		glUniformMatrix4fv(_view_location, 1, GL_FALSE, glm::value_ptr(view));
		int _projection_location = glGetUniformLocation(lightShader.ID, "projection");
		glUniformMatrix4fv(_projection_location, 1, GL_FALSE, glm::value_ptr(projection));
		lightShader.SetVec3("lightColor", lightColor);
		//绘制
		glDisable(GL_CULL_FACE);	//关闭面剔除，使得正方体正常绘制
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 36);

		//交换缓冲并且检查是否有触发事件(比如键盘输入、鼠标移动等）
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//释放VAOVBO
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteBuffers(1, &vertex_buffer_object);
	glDeleteBuffers(1, &element_buffer_object);
	glDeleteBuffers(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);
}
