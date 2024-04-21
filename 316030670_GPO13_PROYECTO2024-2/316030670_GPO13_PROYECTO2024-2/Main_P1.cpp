//Practica 1 - Tlaxcoapan Reyna Jonathan - Grupo 13 - Semestre 2024-2

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 800;


int main() {
	glfwInit();
	//Verificación de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 1 - Tlaxcoapan Reyna Jonathan - Grupo 13", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);

	//Verificación de errores de creacion  ventana
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	Shader OurShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		//Cuadrante (-,+); TRAPECIO
		-0.7f,  0.8f, 0.0f,   0.5648f,0.1259f,0.6158f,  // A 0
		-0.9f,  0.3f, 0.0f,   0.5648f,0.1259f,0.6158f,  // B
		-0.1f,  0.3f, 0.0f,   0.5648f,0.1259f,0.6158f,  // C
		-0.3f,  0.8f, 0.0f,   0.5648f,0.1259f,0.6158f,  // D
		//Cuadrante (+,+); ROMBO
		0.2f,  0.6f, 0.0f,   0.9845f,0.6254f,0.7321f,  // A 4
		0.5f,  0.2f, 0.0f,   0.9845f,0.6254f,0.7321f,  // B
		0.8f,  0.6f, 0.0f,   0.9845f,0.6254f,0.7321f,  // C
		0.5f,  0.8f, 0.0f,   0.9845f,0.6254f,0.7321f,  // D
		//Cuadrante (+,-); RECTÁNGULO		
		0.75f,  -0.9f, 0.0f,   0.123f,0.435f,0.734f,  // A 8
		0.75f,  -0.1f, 0.0f,   0.123f,0.435f,0.734f,  // B
		0.25f,  -0.1f, 0.0f,   0.123f,0.435f,0.734f,  // C
		0.25f,  -0.9f, 0.0f,   0.123f,0.435f,0.734f,  // D
		//Cuadrante (-,-); PENTÁGONO
		-0.50f,  -0.2f, 0.0f,    0.543f,0.394f,0.254f,  // A 12
		-0.85f,  -0.45f, 0.0f,   0.543f,0.394f,0.254f,  // B
		-0.70f,  -0.85f, 0.0f,   0.543f,0.394f,0.254f,  // C
		-0.30f,  -0.85f, 0.0f,   0.543f,0.394f,0.254f,  // D
		-0.15f,  -0.45f, 0.0f,   0.543f,0.394f,0.254f,  // D
	};
	unsigned int indices[] = {  // note that we start from 0!
		0,1,3,    // TRAPECIO
		4,5,7,    // ROMBO
		8,9,11,   // RECTANGULO
		13,14,15  // PENTAGONO
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Desenlazamos de memoria el VAO



	while (!glfwWindowShouldClose(window))
	{
		// Checa si un eveneto ha sido activado y manda a llamar al callback correspondiente
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Dibujar aquí
		OurShader.Use();
		glBindVertexArray(VAO);  //LLAVE QUE ABRE

		//glPointSize(10);
		//glDrawArrays(GL_LINES, 0, 4);       //Donde inicio y cuantos tomo (# de arrays de vertice) 
		//glDrawArrays(GL_POINTS, 0, 4);
		//glDrawElements(GL_TRIANGLES,3, GL_UNSIGNED_INT, (GLvoid*)(0*sizeof(GLfloat)));           // Cantidad de puntos que tomo, donde inicio (indice)

		//TRAPECIO
		glDrawArrays(GL_LINES, 1, 2);   
		glDrawArrays(GL_LINES, 2, 2);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)(0 * sizeof(GLfloat)));

		//ROMBO
		glDrawArrays(GL_LINE_STRIP, 5, 3);   
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)(3 * sizeof(GLfloat)));

		//RECTÁNGULO
		glDrawArrays(GL_LINE_STRIP, 9, 3);   
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)(6 * sizeof(GLfloat)));

		//PENTÁGONO
		glDrawArrays(GL_LINE_LOOP, 12, 5);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)(9 * sizeof(GLfloat)));

		glBindVertexArray(0);  //LLAVE QUE CIERRA

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}
