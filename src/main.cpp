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

bool RaycastChunk(const Chunk& chunk, glm::vec3 Borigin, glm::vec3 dir, float maxDistance, glm::ivec3& hitBlock, BlockFace& hitFace)
    {
        glm::vec3 origin = Borigin + 0.01f;

        glm::ivec3 block = glm::ivec3(
            floor(origin.x),
            floor(origin.y),
            floor(origin.z)
        );

        glm::vec3 deltaDist = glm::vec3(
            dir.x == 0 ? 1e30f : fabs(1.0f / dir.x),
            dir.y == 0 ? 1e30f : fabs(1.0f / dir.y),
            dir.z == 0 ? 1e30f : fabs(1.0f / dir.z)
        );

        glm::ivec3 step;
        glm::vec3 sideDist;

        for (int i = 0; i < 3; i++)
        {
            if (dir[i] < 0)
            {
                step[i] = -1;
                sideDist[i] = (origin[i] - block[i]) * deltaDist[i];
            }
            else
            {
                step[i] = 1;
                sideDist[i] = (block[i] + 1.0f - origin[i]) * deltaDist[i];
            }
        }

        float dist = 0.0f;

        for (int i = 0; i < 128; i++)
        {
            int axis;

            if (sideDist.x < sideDist.y && sideDist.x < sideDist.z)
                axis = 0;
            else if (sideDist.y < sideDist.z)
                axis = 1;
            else
                axis = 2;

            block[axis] += step[axis];
            dist = sideDist[axis];
            sideDist[axis] += deltaDist[axis];

            if (block.x < 0 || block.x >= CHUNK_SIZE_X ||
                block.y < 0 || block.y >= CHUNK_SIZE_Y ||
                block.z < 0 || block.z >= CHUNK_SIZE_Z)
                continue;

            Block b = chunk.GetBlock(block.x, block.y, block.z);

            if (b.id != BLOCK_AIR)
            {
                hitBlock = block;

                if (axis == 0)
                    hitFace = (step.x > 0) ? FACE_LEFT : FACE_RIGHT;
                else if (axis == 1)
                    hitFace = (step.y > 0) ? FACE_BOTTOM : FACE_TOP;
                else
                    hitFace = (step.z > 0) ? FACE_BACK : FACE_FRONT;

                return true;
            }
        }

        return false;
    }
    glm::ivec3 FaceToDirection(BlockFace face)
        {
            switch (face)
            {
                case FACE_FRONT:  return glm::ivec3(0, 0, 1);
                case FACE_BACK:   return glm::ivec3(0, 0, -1);
                case FACE_LEFT:   return glm::ivec3(-1, 0, 0);
                case FACE_RIGHT:  return glm::ivec3(1, 0, 0);
                case FACE_TOP:    return glm::ivec3(0, 1, 0);
                case FACE_BOTTOM: return glm::ivec3(0, -1, 0);
            }
            return glm::ivec3(0);
        }

int main(int argc, char* argv[]) {
    std::cout << "Starting main" << std::endl;

    SDL_Window *window;                    // Declare pointers
    SDL_GLContext context;
    bool done = false;
    int Width = 1920;
    int Height = 1080;
    
    //Create the camera object
    Camera camera(glm::vec3(0.0f, 65.0f, 21.0f), glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f));
    
    
    //Vertices for a basic cube (position + texcoords)
    float blockvertices[] = {
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
    unsigned int blockindices[] = {
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

    //Vertices for the outline cube
    float outlinevertices[] = {
        -0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,

        -0.5f,-0.5f, 0.5f,
        0.5f,-0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f
    };
    unsigned int outlineIndices[] = {
        0,1, 1,2, 2,3, 3,0,
        4,5, 5,6, 6,7, 7,4,
        0,4, 1,5, 2,6, 3,7
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

    //Create and compile the block shader
    Shader BLOCKSHADER("shaders/block.ver", "shaders/block.frag");
    Shader OUTLINESHADER("shaders/blockoutline.ver", "shaders/blockoutline.frag"); 


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
    unsigned int BLOCKVBO;
    glGenBuffers(1, &BLOCKVBO);  

    //Make the vertex array
    unsigned int BLOCKVAO;
    glGenVertexArrays(1, &BLOCKVAO);

    //Make the element buffer
    unsigned int BLOCKEBO;
    glGenBuffers(1, &BLOCKEBO);
    
    //1.Bind the vertex array so we can use it
    glBindVertexArray(BLOCKVAO);

    //2.Move the vertex data to the buffer
    glBindBuffer(GL_ARRAY_BUFFER, BLOCKVBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(blockvertices), blockvertices, GL_STATIC_DRAW);

    //3.Move the index data to the buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BLOCKEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(blockindices), blockindices, GL_STATIC_DRAW);

    //4.Define how OpenGL should interpret the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //5.Define how OpenGL should interpret the texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //same thing but for the block outline

    //Make the vertex buffer
    unsigned int OUTLINEVBO;
    glGenBuffers(1, &OUTLINEVBO);  

    //Make the vertex array
    unsigned int OUTLINEVAO;
    glGenVertexArrays(1, &OUTLINEVAO);

    //Make the element buffer
    unsigned int OUTLINEEBO;
    glGenBuffers(1, &OUTLINEEBO);
    
    //1.Bind the vertex array so we can use it
    glBindVertexArray(OUTLINEVAO);

    //2.Move the vertex data to the buffer
    glBindBuffer(GL_ARRAY_BUFFER, OUTLINEVBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(outlinevertices), outlinevertices, GL_STATIC_DRAW);

    //3.Move the index data to the buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OUTLINEEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(outlineIndices), outlineIndices, GL_STATIC_DRAW);

    //4.Define how OpenGL should interpret the vertex data
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);


    BLOCKSHADER.use();
    BLOCKSHADER.setInt("textureatlas", 0);

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

        glm::ivec3 hitBlock;
        BlockFace hitFace;
        glm::vec3 rayDir = glm::normalize(camera.Front);
        bool hit = RaycastChunk(testChunk, camera.Position, rayDir, 6.0f, hitBlock, hitFace);
        
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

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                testChunk.SetBlock(hitBlock.x, hitBlock.y, hitBlock.z, Block());
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_RIGHT)
            {
                glm::ivec3 dir = FaceToDirection(hitFace);
                glm::ivec3 placePos = hitBlock + dir;

                Block newBlock;
                newBlock.id = BLOCK_COBBLESTONE;
                SetBlockTexture(newBlock);

                testChunk.SetBlock(placePos.x, placePos.y, placePos.z, newBlock);
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

        BLOCKSHADER.use();

        //Send the matrices to the shader for that jucicy high-end 3D graphics
        int modelLoc = glGetUniformLocation(BLOCKSHADER.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(BLOCKSHADER.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(BLOCKSHADER.ID, "projection");
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
        glBindVertexArray(BLOCKVAO);
        for (int x = 0; x < CHUNK_SIZE_X; x++)
        for (int y = 0; y < CHUNK_SIZE_Y; y++)
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {

            Block& block = testChunk.blocks[x][y][z];
            if (block.id == BLOCK_AIR) continue;

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
            BLOCKSHADER.setMat4("model", model);

            for (int i = 0; i < 6; i++) {
                BlockFace face = faces[i];

                //Skip faces not exposed to air
                if (!testChunk.IsFaceVisible(x, y, z, face)) continue;

                //Send texture XY pos to vertex shader
                glm::ivec2 tex = block.GetFaceTexXY(face);
                BLOCKSHADER.setVec2("uvMin", block.GetUVMin(tex));
                BLOCKSHADER.setVec2("uvMax", block.GetUVMax(tex));
                //Apply Block Tint
                BLOCKSHADER.setVec4("BlockTint", block.faceTint[face]);

                glDrawElements(
                    GL_TRIANGLES,
                    6,
                    GL_UNSIGNED_INT,
                    (void*)(faceIndexOffsets[i] * sizeof(unsigned int))
                );
            }
        }
        //Draw the block outline now
        glBindVertexArray(OUTLINEVAO);

        glm::mat4 model = glm::translate(glm::mat4(1.0f),
                                        glm::vec3(hitBlock));

        model = glm::scale(model, glm::vec3(1.002f)); // slightly larger

        OUTLINESHADER.use();
        OUTLINESHADER.setMat4("model", model);
        OUTLINESHADER.setMat4("view", view);
        OUTLINESHADER.setMat4("projection", projection);

        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(window);
    }

    //Close and destroy the window and the OpenGL context
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);

    //Clean up
    SDL_Quit();
    return 0;
}