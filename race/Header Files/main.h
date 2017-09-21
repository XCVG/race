#pragma once

#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <GLUT/glut.h>
#include <GLFW/glfw3.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#include <GL/glut.h>
#include <GL/GL.h>
#endif 
#include <stdio.h>
#include <string.h>
#include <iostream>