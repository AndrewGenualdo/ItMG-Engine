//
// Created by zeben.brown on 11/22/2024.
//

#ifndef TORUSGEN_H
#define TORUSGEN_H

#pragma once
#include "../../ew/external/glad.h"
#include <GL/gl.h>
#include <vector>
#include <GL/gl.h>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class Torus {

    //
    // Created by Zeben on 11/10/2024.
    //



    //Major Radius(R): distance from origin to the center of the tube
    //Minor Radius(r): radius of the tube
    //Sectors: Number of sectors of the tube
    //Sides: Number of sides of the tube
    //Smooth: Smooth or flat shading
    //Up Axis:  Facing Direction, X = 1 Y = 2, Z = 3
    public:

        //Constructor
        Torus(float outerRadius, float innerRadius, int step, int subDivisions);
        //Destructor
        ~Torus() = default;

        struct Vertex
        {
            glm::vec3 Position;
            glm::vec3 Normals;
            glm::vec2 uvCoordinates;
        };


        //draw in vertexArray mode
        void draw() const; //draw surface

    private:

        //functions
        void buildTorus(float outerRadius, float innerRadius, int steps, int subDivisions);

        //variables
        std::vector<Vertex> torusVertices;
        std::vector<unsigned int> torusIndices;

        unsigned int torusVAO;
    };



#endif //TORUSGEN_H
