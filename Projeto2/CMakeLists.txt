cmake_minimum_required(VERSION 3.28)
project(Projeto2 C)

set(CMAKE_C_STANDARD 99)

add_executable(Projeto2 main.c algoritmos.c algoritmos.h utils.h utils.c
        algoritmoEvoluido.c
        algoritmoEvoluido.h
        utilsEvolutivo.c
        utilsEvolutivo.h)