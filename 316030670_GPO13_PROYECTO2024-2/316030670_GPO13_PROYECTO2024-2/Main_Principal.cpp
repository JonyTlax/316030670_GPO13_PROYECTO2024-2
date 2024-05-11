#include <iostream>
#include <cmath>

// GLEW y GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 lightPos2(0.0f, 0.0f, 0.0f);
bool active;
bool active2;

// Variables para las animaciones
float tiempo;
float rot = 0.0f;
float rot2 = 0.0f;
bool anim = false;
bool anim2 = false;
float speed = 1.0f;

// Variables para la animacion simple 1 de la puerta Izquierda
float angle_offset = 0.0f;
float angular_speed = 1.0f;
// Variables para la animacion simple 2 de la puerta Derecha
float angle_offset2 = 0.0f;
float angular_speed2 = 1.0f;
// Variables para la animacion simple 3 de la camara de seguridad
float angle_offset3 = 0.0f;
float angular_speed3 = 1.0f;
// Variables para la animacion compleja
float angle_offset4 = 0.0f;
float angular_speed4 = 1.0f;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(2.0f, 17.0f, -34.5f),  //0 Luces superiores
	glm::vec3(2.0f, 17.0f, -18.7f),  //1
	glm::vec3(2.0f, 17.0f,  -2.4f),  //2
	glm::vec3(2.0f, 5.615f, 6.36f),  //3
	glm::vec3(2.0f,  7.5f, -38.5f),  //4 Luces inferiores
	glm::vec3(2.0f,  7.5f, -30.5f),  //5
	glm::vec3(2.0f,  7.5f, -22.5f),  //6
	glm::vec3(2.0f,  7.5f, -14.5f)   //7
};

float vertices[] = {
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

glm::vec3 Light1 = glm::vec3(0);
glm::vec3 Light2 = glm::vec3(0);
glm::vec3 Light3 = glm::vec3(0);
glm::vec3 Light4 = glm::vec3(0);
glm::vec3 Light5 = glm::vec3(0);
glm::vec3 Light6 = glm::vec3(0);
glm::vec3 Light7 = glm::vec3(0);
glm::vec3 Light8 = glm::vec3(0);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Tlaxcoapan Reyna Jonathan - Proyecto FInal LAB-CGeIHC - Gpo 13", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	//Carga de los siete Modelos principales
	Model Banca((char*)"Models/1_Banca/bancas.obj");
	Model Atril((char*)"Models/2_Atril/atril.obj");
	Model Atril2((char*)"Models/2_Atril/atrilresp.obj");       //Transparencia y canal alpha
	Model Librero((char*)"Models/3_Librero/librero.obj");
	Model Palmera((char*)"Models/4_Palmera/palmeras.obj");
	Model SmartTv((char*)"Models/5_SmartTv/smartvs.obj");
	Model Comedor((char*)"Models/6_Silla/comedor.obj");
	Model Comedor2((char*)"Models/6_Silla/comedor2.obj");
	Model Comedor3((char*)"Models/6_Silla/comedor3.obj");
	Model Comedor4((char*)"Models/6_Silla/comedor4.obj");
	Model Ride((char*)"Models/7_Bateria/1_ride.obj");
	Model Hithat((char*)"Models/7_Bateria/2_hithat.obj");
	Model Banquillo((char*)"Models/7_Bateria/4_banco.obj");
	Model Tarola((char*)"Models/7_Bateria/5_tarola.obj");
	Model Toms((char*)"Models/7_Bateria/6_toms.obj");
	Model Tom2((char*)"Models/7_Bateria/7_tompiso.obj");
	Model Aros((char*)"Models/7_Bateria/8_aros.obj");          //Transparencia y canal alpha
	Model LampBase((char*)"Models/Lamps/base.obj");
	Model LampInf((char*)"Models/Lamps/lampInf.obj");          //Transparencia y canal alpha
	Model LampSup((char*)"Models/Lamps/lampSup.obj");          //Transparencia y canal alpha
	Model CamBas((char*)"Models/8_Camara/bascam.obj");
	Model Cam((char*)"Models/8_Camara/cam.obj");

	//Carga de los Modelos/objetos correspondientes a la estructura y la fachada
	Model Fachada((char*)"ModelsEstructura/Fachada/fachada.obj");
	Model Varilla((char*)"ModelsEstructura/Muros/Varillas/varfrente.obj");
	Model Techo((char*)"ModelsEstructura/Muros/Varillas/vartecho.obj");   
	Model MuroExt1((char*)"ModelsEstructura/Muros/Ext1/ext1.obj");
	Model MuroExt2((char*)"ModelsEstructura/Muros/Ext2/ext2.obj");
	Model Calle((char*)"ModelsEstructura/Pisos/Frente/calle.obj");
	Model PisoFrente((char*)"ModelsEstructura/Pisos/Frente/frente.obj");
	Model PisoInt((char*)"ModelsEstructura/Pisos/Int1/int1.obj");
	Model PisoLateral((char*)"ModelsEstructura/Pisos/Lateral/lat.obj");
	Model P1izq((char*)"ModelsEstructura/Puertas/P1/p1Izq.obj");
	Model P1der((char*)"ModelsEstructura/Puertas/P1/p1Der.obj");
	Model P2izq((char*)"ModelsEstructura/Puertas/P2/p2Izq.obj");
	Model P2der((char*)"ModelsEstructura/Puertas/P2/p2Der.obj");
	Model Ventanasup((char*)"ModelsEstructura/Ventanas/ventanaSup.obj");  //Transparencia y canal alpha
	Model Ventanainf((char*)"ModelsEstructura/Ventanas/ventanaInf.obj");  //Transparencia y canal alpha

	//Carga de modelos hecho por AI (Luma AI, Genie)
	Model People((char*)"Models/Luma_AI/personas.obj");
	Model SV((char*)"Models/Luma_AI/dron.obj");

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right_day.tga");
	faces.push_back("SkyBox/left_day.tga");
	faces.push_back("SkyBox/up_day.tga");
	faces.push_back("SkyBox/down_day.tga");
	faces.push_back("SkyBox/back_day.tga");
	faces.push_back("SkyBox/front_day.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        //glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f); //  -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.5f, 0.5f, 0.5f);        //Deja los mismos valores para 
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);       //ambient y diffuse
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);       //Para la intensidad

		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= Light1.x;//abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= Light1.y;//abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= Light1.z;//sin(glfwGetTime() *Light1.z);		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.2f, 0.2f, 0.2f); //1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.7f);//0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 1.0f); //0.075f);
		
		// Point light 2
		glm::vec3 lightColor2;
		lightColor2.x = Light2.x;
		lightColor2.y = Light2.y;
		lightColor2.z = Light2.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 1.0f);

		// Point light 3
		glm::vec3 lightColor3;
		lightColor3.x = Light3.x;
		lightColor3.y = Light3.y;
		lightColor3.z = Light3.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor3.x, lightColor3.y, lightColor3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor3.x, lightColor3.y, lightColor3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 1.0f);

		// Point light 4
		glm::vec3 lightColor4;
		lightColor4.x = Light4.x;
		lightColor4.y = Light4.y;
		lightColor4.z = Light4.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), lightColor4.x, lightColor4.y, lightColor4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), lightColor4.x, lightColor4.y, lightColor4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.9f, 0.9f, 0.9f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.35f);       //0.7
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.44f);    //1.0

		// Point light 5
		glm::vec3 lightColor5;
		lightColor5.x = Light5.x;
		lightColor5.y = Light5.y;
		lightColor5.z = Light5.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), lightColor5.x, lightColor5.y, lightColor5.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), lightColor5.x, lightColor5.y, lightColor5.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 0.2f, 2.0f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 0.44f);

		// Point light 6
		glm::vec3 lightColor6;
		lightColor6.x = Light6.x;
		lightColor6.y = Light6.y;
		lightColor6.z = Light6.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].ambient"), lightColor6.x, lightColor6.y, lightColor6.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].diffuse"), lightColor6.x, lightColor6.y, lightColor6.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].quadratic"), 1.0f);
				
		// Point light 7
		glm::vec3 lightColor7;
		lightColor7.x = Light7.x;
		lightColor7.y = Light7.y;
		lightColor7.z = Light7.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].position"), pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].ambient"), lightColor7.x, lightColor7.y, lightColor7.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].diffuse"), lightColor7.x, lightColor7.y, lightColor7.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].quadratic"), 1.0f);

		// Point light 8
		glm::vec3 lightColor8;
		lightColor8.x = Light8.x;
		lightColor8.y = Light8.y;
		lightColor8.z = Light8.z;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].position"), pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].ambient"), lightColor8.x, lightColor8.y, lightColor8.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].diffuse"), lightColor8.x, lightColor8.y, lightColor8.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].quadratic"), 1.0f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);	

		//+++++++++++++++++++++Carga de modelos correspondientes a la fachada y estructura.++++++++++++++
        view = camera.GetViewMatrix();	
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Calle.Draw(lightingShader);
		Fachada.Draw(lightingShader);
		Varilla.Draw(lightingShader);
		Techo.Draw(lightingShader);
		MuroExt1.Draw(lightingShader);
		MuroExt2.Draw(lightingShader);
		PisoFrente.Draw(lightingShader);
		PisoInt.Draw(lightingShader);
		PisoLateral.Draw(lightingShader);
		People.Draw(lightingShader);

		//*********************************************************************************ANIMACIONES SIMPLES 1 Y 2************************
		//Calcula el �ngulo de rotaci�n basado en el tiempo
		float oscillation_angle = sin(glfwGetTime() * angular_speed) * rot; // rot es el �ngulo m�ximo de oscilaci�n
		float oscillation_angle2 = sin(glfwGetTime() * angular_speed2) * rot2;

		// Aplica el desplazamiento inicial al �ngulo de rotaci�n
		float rotation_angle = oscillation_angle + angle_offset;
		float rotation_angle2 = oscillation_angle2 + angle_offset2;

		// Limita el �ngulo de rotaci�n para que est� dentro del rango
		rotation_angle = glm::clamp(rotation_angle, 0.0f, 120.0f);
		rotation_angle2 = glm::clamp(rotation_angle2, -90.0f, 0.0f);                             

		if (rotation_angle >= 0.0f || rotation_angle <= 120.0f) {
			angle_offset = -angle_offset; // Invierte la direcci�n de la oscilaci�n
		}

		if (rotation_angle2 >= -90.0f || rotation_angle2 <= 0.0f) {
			angle_offset2 = -angle_offset2; // Invierte la direcci�n de la oscilaci�n
		}

		glm::mat4 model_Puerta1(1);
		model_Puerta1 = glm::translate(model_Puerta1, glm::vec3(-0.65f, 0.0f, 1.0f));
		model_Puerta1 = glm::rotate(model_Puerta1, glm::radians(rotation_angle), glm::vec3(0.0f, 1.0f, 0.0f)); 
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_Puerta1));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		P1izq.Draw(lightingShader);
		
		model_Puerta1 = glm::mat4(1);
		model_Puerta1 = glm::translate(model_Puerta1, glm::vec3(4.65f, 0.0f, 1.0f));
		model_Puerta1 = glm::rotate(model_Puerta1, glm::radians(rotation_angle2), glm::vec3(0.0f, 1.0f, 0.0f)); 
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_Puerta1));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		P1der.Draw(lightingShader);
		
		glm::mat4 model_Puerta2(1);
		model_Puerta2 = glm::translate(model_Puerta2, glm::vec3(0.1083f, -0.932f, 11.648f));
		model_Puerta2 = glm::rotate(model_Puerta2, glm::radians(rotation_angle), glm::vec3(0.0f, 1.0f, 0.0f)); // Aplicar rotación a la cola  
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_Puerta2));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		P2izq.Draw(lightingShader);

		model_Puerta2 = glm::mat4(1);
		model_Puerta2 = glm::translate(model_Puerta2, glm::vec3(3.899f, -0.932f, 11.648f));
		model_Puerta2 = glm::rotate(model_Puerta2, glm::radians(rotation_angle2), glm::vec3(0.0f, 1.0f, 0.0f)); 
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_Puerta2));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		P2der.Draw(lightingShader);

		//*********************************************************************************ANIMACION SIMPLE 3************************

		float oscillation_angle3 = sin(glfwGetTime() * angular_speed3) * 45.0f; 
		float rotation_angle3 = oscillation_angle3 + angle_offset;
		rotation_angle3 = glm::clamp(rotation_angle3, -45.0f, 45.0f);

		if (rotation_angle3 >= 45.0f || rotation_angle3 <= -45.0f) {
			angle_offset3 = -angle_offset3; 
		}

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.046f, 5.469f, 2.462f));
		model = glm::rotate(model, glm::radians(oscillation_angle3), glm::vec3(0.0f, 1.0f, 0.0f)); 
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Cam.Draw(lightingShader);


		// ++++++++++++++++++++++++++++++++++++++++Carga de los modelos principales (Sin transparencia)+++++++++++++++++++++++++++
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		
		Banca.Draw(lightingShader);
		Atril.Draw(lightingShader);
		Librero.Draw(lightingShader);
		Palmera.Draw(lightingShader);
		SmartTv.Draw(lightingShader);
		Comedor.Draw(lightingShader);
		Comedor2.Draw(lightingShader);
		Comedor3.Draw(lightingShader);
		Comedor4.Draw(lightingShader);
		Ride.Draw(lightingShader);
		Hithat.Draw(lightingShader);
		Banquillo.Draw(lightingShader);
		Tarola.Draw(lightingShader);
		Toms.Draw(lightingShader);
		Tom2.Draw(lightingShader);
		LampBase.Draw(lightingShader);
		CamBas.Draw(lightingShader);

			
		//----------------------------------------------------------------------------------Para la transparencia

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.5f);  //Color de la transparencia y valor de transparencia de alpha
		
		//Carga de objetos principales con transparencia
		Atril2.Draw(lightingShader);
		Aros.Draw(lightingShader);
		Ventanasup.Draw(lightingShader);
		LampInf.Draw(lightingShader);
		LampSup.Draw(lightingShader);

		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.85f);
		Ventanainf.Draw(lightingShader);

		glDisable(GL_BLEND);  //Desactiva el canal alfa 

		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);  //Para reiniciar el color alpha

		glBindVertexArray(0);

	    // ---------------------------------------------------------------------------------------Iluminacion-----------------

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 8; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


		//----------------------------------------------------------------------------Animaci�n COMPLEJA------------
		Anim.Use();
		model = glm::mat4(1);
		tiempo = glfwGetTime() * speed;
		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		float oscillation_angle4 = sin(glfwGetTime() * angular_speed4) * 45.0f;
		float rotation_angle4 = oscillation_angle4 + angle_offset;
		rotation_angle4 = glm::clamp(rotation_angle4, -270.0f, 270.0f);

		if (rotation_angle4 >= 270.0f || rotation_angle4 <= -270.0f) {
			angle_offset4 = -angle_offset4;
		}

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 17.0f, 0.0f));
		model = glm::rotate(model, glm::radians(oscillation_angle4), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
		SV.Draw(Anim);
		glBindVertexArray(0);


		// ------------------------------------------------------------------------------------------------------------Skybox-------

		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Moves/alters the camera positions based on user input     ---------  Tu animacion no se detiene hasta finalizar el programa
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{	camera.ProcessKeyboard(FORWARD, deltaTime);	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{	camera.ProcessKeyboard(BACKWARD, deltaTime);}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{	camera.ProcessKeyboard(LEFT, deltaTime);	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{	camera.ProcessKeyboard(RIGHT, deltaTime);	}

	// Para mover la luz
	/*if (keys[GLFW_KEY_T])
	{	pointLightPositions[0].x += 0.1f;	}

	if (keys[GLFW_KEY_G])
	{	pointLightPositions[0].x -= 0.1f;	}

	if (keys[GLFW_KEY_Y])
	{	pointLightPositions[0].y += 0.01f;	}

	if (keys[GLFW_KEY_H])
	{	pointLightPositions[0].y -= 0.01f;	}

	if (keys[GLFW_KEY_U])
	{	pointLightPositions[0].z -= 0.01f;	}

	if (keys[GLFW_KEY_J])
	{	pointLightPositions[0].z += 0.01f;	}*/


	//Animacion de la puerta Izq
	if (anim) {
		if (rot < 120.0f) {
			rot -= 4.0f;
		}		
	}

	//Animacion de la puerta Der
	if (anim2) {
		if (rot2 > 0.0f) {
			rot2 -= 4.0f;
		}
	}

}

// Is called whenever a key is pressed/released via GLFW  ---     Tu animacion se puede detener con una tecla
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	// ACTIVACION DE LAS POINTLIGTHS
	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
			Light2 = glm::vec3(1.0f, 1.0f, 1.0f);
			Light3 = glm::vec3(1.0f, 1.0f, 1.0f);
			Light4 = glm::vec3(1.0f, 1.0f, 1.0f);	

			Light5 = glm::vec3(1.0f, 0.0f, 1.0f);
			Light6 = glm::vec3(1.0f, 1.0f, 0.0f);
			Light7 = glm::vec3(1.0f, 1.0f, 1.0f);
			Light8 = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else
		{
			Light1 = glm::vec3(0);
			Light2 = glm::vec3(0);
			Light3 = glm::vec3(0);
			Light4 = glm::vec3(0);		

			Light5 = glm::vec3(0);
			Light6 = glm::vec3(0);
			Light7 = glm::vec3(0);
			Light8 = glm::vec3(0);
		}
	}


	//Animacion puerta izquierda
	if (keys[GLFW_KEY_M]) {
		anim = !anim;
		if (anim) {
			rot = 0.0f;
		}
		else { rot = 120.0f; }
	}

	//Animacion Puerta DEr
	if (keys[GLFW_KEY_N]) {
		anim2 = !anim2;
		if (anim2) {
			rot2 = -90.0f;
		}

		else { rot2 = 0.0f; }
	}

}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}