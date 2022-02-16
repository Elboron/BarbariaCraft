#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "input_handling.h"

#define RENDER_TICK_SPEED 60
#define INPUT_TICK_SPEED 30
#define COMPUTE_TICK_SPEED 20 

GLFWwindow* game_window;
pthread_mutex_t game_window_mutex = PTHREAD_MUTEX_INITIALIZER;
int close_game = 0;
pthread_mutex_t close_game_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t init_thread;

void check_thread_creation_error(int status) {
	if(status == EAGAIN) {
		printf("Insufficient resources to create thread\n");
	} else if(status == EINVAL) {
		printf("Invalid thread settings\n");
	} else if(status == EPERM) {
		printf("No permission to create thread\n");
	}
}

void* compute_loop(void *arg) {
	int compute_timer = glfwGetTime();
	int old_timer = glfwGetTime();

	float compute_intervall = 1.0 / COMPUTE_TICK_SPEED;
	float thread_sleep_time = (compute_intervall / 2.0) * 1000;

	while(1) {
		if(compute_timer >= compute_intervall) {
			old_timer += compute_intervall;
		}
		compute_timer = glfwGetTime() - old_timer;
#ifdef _WIN32
		Sleep(thread_sleep_time);
#else
		usleep(thread_sleep_time);
#endif
	}
	return (void*)0;
}

void* render_loop(void* arg) {	
	glfwMakeContextCurrent(game_window);
	glewExperimental = 1;
	if(glewInit() != GLEW_OK) {
		close_game = 1;
		return (void*)-1;
	}

	double render_timer = glfwGetTime();
	double old_timer = glfwGetTime();

	float render_intervall = 1.0 / RENDER_TICK_SPEED;
	float thread_sleep_time = (render_intervall / 2.0) * 1000;
	
	while(1) {
		if((render_timer - old_timer) >= render_intervall) {
			glClearColor(0.5f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwSwapBuffers(game_window);
			old_timer += render_intervall;
		}
		render_timer = glfwGetTime();
#ifdef _WIN32
		Sleep(thread_sleep_time);
#else
		usleep(thread_sleep_time);
#endif
	}
	return (void*)0;
}

void* input_loop(void* arg) {
	double input_timer = glfwGetTime();
	double old_timer = glfwGetTime();

	float input_intervall = 1.0 / INPUT_TICK_SPEED;
	float thread_sleep_time = (input_intervall / 2.0) * 1000;

	while(1) {
		if((input_timer - old_timer) >= input_intervall) {
			pthread_mutex_lock(&game_window_mutex);
			glfwPollEvents();
			int input_status = 0;
			input_status = handle_input(game_window);
			if(input_status == -1) {
				pthread_mutex_lock(&close_game_mutex);
				close_game = 1;
				pthread_mutex_unlock(&close_game_mutex);
			}
			pthread_mutex_unlock(&game_window_mutex);
			old_timer += input_intervall;
		}
		input_timer = glfwGetTime();
#ifdef _WIN32
		Sleep(thread_sleep_time);
#else
		usleep(thread_sleep_time);
#endif
	}
	return (void*)0;
}


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	game_window = glfwCreateWindow(1920, 1080, "Barbaria", glfwGetPrimaryMonitor(), (void*)0);
	if(game_window == (void*)0) {
		printf("Could not create GLFW game window\n");
		close_game = 1;
	}
	glViewport(0, 0, 1920, 1080);
	glfwSetTime(0);

	pthread_t render_thread;
	pthread_t calculate_thread;
	pthread_t input_thread;

	int status = 0;

	/* Start the compute loop thread */
	status = pthread_create(&calculate_thread, (void*)0, &compute_loop, (void*)0);
	if(status != 0) {
		printf("Could not create compute thread\n");
		check_thread_creation_error(status);
	}

	status = pthread_create(&render_thread, (void*)0, &render_loop, (void*)0);
	/* Start the render loop thread */
	if(status != 0) {
		printf("Could not create render thread\n");
		check_thread_creation_error(status);
	}
	
	/* Start the input loop thread */
	status = pthread_create(&input_thread, (void*)0, &input_loop, (void*)0);
	if(status != 0) {
		printf("Could not create input thread\n");
		check_thread_creation_error(status);
	}

	/* Update the current time and check if to terminate the game */
	while(1) {
		pthread_mutex_lock(&close_game_mutex);
		if(close_game == 1) {
			pthread_cancel(input_thread);
			pthread_cancel(calculate_thread);
			pthread_cancel(render_thread);
			break;
		}
		pthread_mutex_unlock(&close_game_mutex);
	}

	glfwTerminate();
	return 0;
}
