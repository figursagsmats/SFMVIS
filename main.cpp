//såhär kan man kompilera på en mac
//g++ main.cpp -framework OpenGl -framework CoreFoundation -lGLEW -lglfw3 -lSOIL -lCGAL
//denna är bra också
//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "shader.h"
#include "camera.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>

// Properties
GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
    
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth*2, screenHeight*2);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    // Setup and compile our shaders
    Shader ourShader("shaders/textures.vs", "shaders/textures.frag");
    Shader axisShader("shaders/axis.vert", "shaders/axis.frag");

    // Set up our vertex data (and buffer(s)) and attribute pointers
    GLfloat axis[] = {
        0.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        1.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        
        0.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        
        0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
    };
    
    GLfloat vertices[] = {
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
    
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3(2.0f, 5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3(2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f, 3.0f, -7.5f),  
        glm::vec3(1.3f, -2.0f, -2.5f),  
        glm::vec3(1.5f, 2.0f, -2.5f), 
        glm::vec3(1.5f, 0.2f, -1.5f), 
        glm::vec3(-1.3f, 1.0f, -1.5f)  
    };
    
    GLfloat coneVerts[] = {
        0.0f, 0.0f, 0.0f,           0.0f, 1.0f,
        1.0f, 1.0f, 2.0f,          0.0f, 1.0f,
        
        0.0f, 0.0f, 0.0f,           0.0f, 1.0f,
        1.0f, -1.0f, 2.0f,         0.0f, 1.0f,
        
        0.0f, 0.0f, 0.0f,           0.0f, 1.0f,
        -1.0f, -1.0f, 2.0f,           0.0f, 1.0f,
        
        0.0f, 0.0f, 0.0f,           0.0f, 1.0f,
        -1.0f, 1.0f, 2.0f,           0.0f, 1.0f,
        
        1.0f, 1.0f, 2.0f,          1.0f, 1.0f,
        1.0f, -1.0f, 2.0f,         1.0f, 0.0f,
        -1.0f, -1.0f, 2.0f,        0.0f, 0.0f,
        
        -1.0f, 1.0f, 2.0f,         0.0f, 1.0f,
        1.0f, 1.0f, 2.0f,          1.0f, 1.0f,
        -1.0f, -1.0f, 2.0f,        0.0f, 0.0f, 
    };
    
    //scale down the cameras
    for(int i = 0; i < 14*5; i+=5)
    {
        coneVerts[i + 0] *= 0.04125;
        coneVerts[i + 1] *= 0.04125;
        coneVerts[i + 2] *= 0.04125;
    }

    int noMatrices = 36;
    int matDim = 4;
    int matSize = matDim*matDim;
    float rotations[noMatrices*matSize];
    int idx = 0;
    std::ifstream fin("rotations.txt");
    if(fin){
        std::string item;
        for (std::string line; getline(fin, line); )
        {
            std::istringstream in(line);

            while (getline(in, item, ','))
            {
                rotations[idx++] = std::atof(item.c_str());
            }
        }
    } else { std:: cout << "Couldnt read file rotations\n"; }
    
    glm::mat4 rotationMatrices[36];
    
    for(int i = 0, j = 0; i < noMatrices*matSize; i+=matSize, j++)
    {
        float tmp[matSize];
        std::copy(rotations+i, rotations+i+matSize, tmp);
        rotationMatrices[j] = glm::make_mat4(tmp);
    }
    
    
    //TRANSLATIONS
    int vecDim = 3;
    float translations[noMatrices*vecDim];
    
    idx = 0;
    
    std::ifstream fin2("translations.txt");
    if(fin2){
        std::string item;
        for (std::string line; getline(fin2, line); )
        {
            std::istringstream in(line);

            while (getline(in, item, ','))
            {
                translations[idx++] = std::atof(item.c_str());
            }
        }
    } else { std:: cout << "Couldnt read file translations\n"; }
    std::cout << idx << std::endl;
    // std::cout << "Transformations\n";
    glm::mat4 translationMatrices[36];
    
    for(int i = 0, j = 0; i < noMatrices*vecDim; i+=vecDim, j++)
    {
        float tmp[vecDim];
        std::copy(translations+i, translations+i+vecDim, tmp);
        // std::cout << tmp[0]<< ", " <<tmp[1] << ", "<<   tmp[2] << std::endl;
        glm::vec3 tmp2 = glm::make_vec3(tmp);
        
        translationMatrices[j] = glm::translate(glm::mat4(1.0), tmp2);
    }
    

    GLuint VBO, VAO, VBO2, VAO2;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
    glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(axis), axis, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO
    
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);

        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coneVerts), coneVerts, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);


    glBindVertexArray(0); // Unbind VAO

    // Load and create a texture 
    GLuint texture1;
    GLuint texture2;
    // --== TEXTURE 1 == --
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    int width, height;
    unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    // --== TEXTURE 2 == --
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2); 
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); 
    
    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();

        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Draw our first triangle
        // ourShader.Use();

        // Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);
        
        // Create camera transformation
        glm::mat4 view;
        view = camera.GetViewMatrix();
        glm::mat4 projection;	
        projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 1000.0f);
        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        
        //for axis shader// Get the uniform locations
        GLint modelLoc2 = glGetUniformLocation(axisShader.Program, "model");
        GLint viewLoc2 = glGetUniformLocation(axisShader.Program, "view");
        GLint projLoc2 = glGetUniformLocation(axisShader.Program, "projection");
        // // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(projection));
        
        
        //should draw the axis system!!! :(
        glBindVertexArray(VAO);
            
            axisShader.Use();
            glm::mat4 model = glm::mat4(1.0);
            glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_LINES, 0, 36);
            			
        glBindVertexArray(0);
        
        //Draws the cameras!
        glBindVertexArray(VAO2);
            // ourShader.Use();
            // for(GLuint i = 0; i < 36; i++)
            // {
            //     glm::mat4 model = glm::mat4(1.0);
            //     model = glm::rotate(model, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
            //     model = model * translationMatrices[i];
            //     model = model * (rotationMatrices[i]);
            //     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            //     glDrawArrays(GL_LINES, 0, 8);
            //     glDrawArrays(GL_TRIANGLES, 8, 6);
            // }
        glBindVertexArray(0);
        
        // Swap the buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //cout << key << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;	
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}	


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}