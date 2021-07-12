#include "../inc/engine_draw.hpp"

namespace draw {
    void EngineDraw::initialize(std::vector<data::Vertex2D> vertices, graphics::EngineGraphics* userGraphics, uint32_t primitiveCount) {
        engGraphics = userGraphics;
        //check if pipeline exists
        if (checkPipelineSuitability(primitiveCount)) {
            //create new graphics pipeline
        }

        //TODO: can save a lot of performance by doing a check for if the vertex buffer has the same memory allocated, but lets
        //      first see what the performance would look like to see how viable this method is

        //check if vertex buffer exists and if its appropriate
        engGraphics->createVertexBuffer(vertices);
        
        engGraphics->createCommandBuffers(vertices);
        engGraphics->drawFrame();

    }
    EngineDraw::~EngineDraw() {
        //std::cout << "draw destruction..." << std::endl;

        engGraphics->cleanupRender();
    }

    bool EngineDraw::checkPipelineSuitability(uint32_t primitiveCount) {
        if (&engGraphics->graphicsPipeline == nullptr || engGraphics->bindingCount < primitiveCount)  {
            return true;
        }
        return false;
    }
}