#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL3/SDL.h>

class Camera {
public:
    // Camera vectors
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Down;

    // Euler angles
    float Pitch;
    float Yaw;

    // Mouse sensitivity
    float Sensitivity;

    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::vec3 down)
        : Position(position), Front(front), Up(up), Down(down), Pitch(0.0f), Yaw(-90.0f), Sensitivity(0.2f) {}

    // Call this every frame to handle keyboard and mouse input
    void ProcessInput(const bool* keys, float deltaTime) {
        float cameraSpeed = 4.5f * deltaTime;
        if (keys[SDL_SCANCODE_W])
            Position += cameraSpeed * Front;
        if (keys[SDL_SCANCODE_S])
            Position -= cameraSpeed * Front;
        if (keys[SDL_SCANCODE_A])
            Position -= glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
        if (keys[SDL_SCANCODE_D])
            Position += glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
        if (keys[SDL_SCANCODE_SPACE])
            Position += cameraSpeed * Up;
        if (keys[SDL_SCANCODE_LSHIFT])
            Position += cameraSpeed * Down;
    }

    void ProcessMouseMotion(float dx, float dy) {
        Yaw   += dx * Sensitivity;
        Pitch -= dy * Sensitivity;

        //Clamp pitch
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;

        //Update Front vector
        glm::vec3 direction;
        direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        direction.y = sin(glm::radians(Pitch));
        direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(direction);
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }
};
