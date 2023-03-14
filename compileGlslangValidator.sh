#!/usr/bin/bash

glslangValidator -V core/shaders/shader2D.vert -o build/vert.spv
glslangValidator -V core/shaders/shader2D.frag -o build/frag.spv
