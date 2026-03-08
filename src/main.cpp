#include <glad/glad.h> 
#include <SDL3/SDL.h>
#include <cmath>
#include <iostream>
#include <shader_class/shader_class.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <camera/camera.hpp>
#include <chunk/chunk.h>
#include <block/block.h>

// allocate chunk in static storage to avoid large stack allocation crash
static Chunk testChunk;

int main(int argc, char* argv[]) {
    std::cout << "Starting main" << std::endl;

    SDL_Window *window;                    // Declare pointers
    SDL_GLContext context;
    bool done = false;
    int Width = 1920;
    int Height = 1080;
    
    //Create the camera object
    Camera camera(glm::vec3(0.0f, 65.0f, 21.0f), glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f));
    
    
    // Cube vertices (position + texcoords)
    float vertices[] = {
        // positions          // texcoords
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // 0
        0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // 1
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 2
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // 3

        // Back face
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // 4
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 5
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // 6
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // 7

        // Left face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 8
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // 9
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 10
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // 11

        // Right face
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // 12
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // 13
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // 14
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // 15

        // Top face
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 16
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 17
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // 18
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // 19

        // Bottom face
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // 20
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // 21
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 22
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f  // 23
    };
    unsigned int indices[] = {
    // Front
    0, 1, 2,
    0, 2, 3,

    // Back
    4, 6, 5,
    4, 7, 6,

    // Left
    8, 9,10,
    8,10,11,

    // Right
    12,14,13,
    12,15,14,

    // Top
    16,17,18,
    16,18,19,

    // Bottom
    20,22,21,
    20,23,22
    };



    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL3
    std::cout << "SDL Init OK" << std::endl;

    //Configure the window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //Create an application window with the following settings:
    window = SDL_CreateWindow(
        "MineBox 0.0.1 alpha impl: Beta 1.7.3",                  // window title
        Width,                               // width, in pixels
        Height,                               // height, in pixels
        SDL_WINDOW_OPENGL                 // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }
    std::cout << "Window created OK" << std::endl;

    SDL_SetWindowResizable(window, true);
    context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cout << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        return 1;
    }
    std::cout << "OpenGL context created OK" << std::endl;


    //initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << "GLAD initialized OK" << std::endl;    

    //Create and compile OUR SHADER!
    Shader OURSHADER("shaders/vertex.shader", "shaders/fragment.shader"); 

    //make the texture object
    unsigned int textureatlas;
    glGenTextures(1, &textureatlas);  

    //load the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureatlas);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("terrain.png", &width, &height, &nrChannels, 0); 
    if (!data) {
        SDL_Log("Failed to load terrain.png");
        return -1;
    }
  

    //Configure our tex so we can use it
    glBindTexture(GL_TEXTURE_2D, textureatlas);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Generate the texture
    glBindTexture(GL_TEXTURE_2D, textureatlas); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);


    //Make the vertex buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);  

    //Make the vertex array
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    //Make the element buffer
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    
    //1.Bind the vertex array so we can use it
    glBindVertexArray(VAO);

    //2.Move the vertex data to the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //3.Move the index data to the buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //4.Define how OpenGL should interpret the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //5.Define how OpenGL should interpret the texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    OURSHADER.use();
    OURSHADER.setInt("textureatlas", 0);

    //Finally get 3D up and running
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 


    // note that we're translating the scene in the reverse direction of where we want to move
    glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)Width / (float)Height, 0.1f, 100.0f);


    //Enable depth testing and back-face culling
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // use global static chunk to avoid stack overflow crash
    testChunk.GenerateChunk();

    // We'll use this for rendering each block
    glm::vec3 blockOffset;

    //Set all the variables needed for delta time 
    float atime_elasped;
    float time_elasped;
    float deltaTime = 0.0f;	
    float lastFrame = 0.0f;
    float currentFrame;

    //Lock the mouse to the middle
    SDL_SetWindowRelativeMouseMode(window, true);

    //Main loop
    while (!done) {
        SDL_Event event;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
            //Change the viewport if the window is resized
            if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                int widthgl = event.window.data1;
                int heightgl = event.window.data2;
                glViewport(0, 0, widthgl, heightgl);
                glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)widthgl / (float)heightgl, 0.1f, 100.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }

            //Get mouse input
            if (event.type == SDL_EVENT_MOUSE_MOTION)
            {
                camera.ProcessMouseMotion(event.motion.xrel, event.motion.yrel);
            }
        }

        //Calulate delta time
        atime_elasped = SDL_GetTicks();
        time_elasped = atime_elasped / 1000;
        currentFrame = time_elasped;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        //Get keyboard input and process it
        const bool* keys = SDL_GetKeyboardState(NULL);
        camera.ProcessInput(keys, deltaTime);


        //Clear the screen with color
        glClearColor(0.482f, 0.643f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix(); 

        OURSHADER.use();

        //Send the matrices to the shader for that jucicy high-end 3D graphics
        int modelLoc = glGetUniformLocation(OURSHADER.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(OURSHADER.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(OURSHADER.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //Declare all the block faces
        static const BlockFace faces[6] = {
            FACE_FRONT,
            FACE_BACK,
            FACE_LEFT,
            FACE_RIGHT,
            FACE_TOP,
            FACE_BOTTOM
        };

        static const int faceIndexOffsets[6] = {
            0,   // front
            6,   // back
            12,  // left
            18,  // right
            24,  // top
            30   // bottom       
        };


        //DRAWING YAYAYAYAYA
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureatlas);
        glBindVertexArray(VAO);
        glBindVertexArray(VAO);
        for (int x = 0; x < CHUNK_SIZE_X; x++)
        for (int y = 0; y < CHUNK_SIZE_Y; y++)
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {

            Block& block = testChunk.blocks[x][y][z];
            if (block.id == BLOCK_AIR) continue;

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
            OURSHADER.setMat4("model", model);

            for (int i = 0; i < 6; i++) {
                BlockFace face = faces[i];

                //Skip faces not exposed to air
                if (!testChunk.IsFaceVisible(x, y, z, face)) continue;

                //Send texture XY pos to vertex shader
                glm::ivec2 tex = block.GetFaceTexXY(face);
                OURSHADER.setVec2("uvMin", block.GetUVMin(tex));
                OURSHADER.setVec2("uvMax", block.GetUVMax(tex));
                //Apply Block Tint
                OURSHADER.setVec4("BlockTint", block.faceTint[face]);

                glDrawElements(
                    GL_TRIANGLES,
                    6,
                    GL_UNSIGNED_INT,
                    (void*)(faceIndexOffsets[i] * sizeof(unsigned int))
                );
            }
        }
        SDL_GL_SwapWindow(window);
    }

    //Close and destroy the window and the OpenGL context
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);

    //Clean up
    SDL_Quit();
    return 0;
}