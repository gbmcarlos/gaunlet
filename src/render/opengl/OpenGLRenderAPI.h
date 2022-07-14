#pragma once

namespace engine {

    class OpenGLRenderApi {

    private:
        OpenGLRenderApi() {};

    public:
        static OpenGLRenderApi& getInstance(){
            static OpenGLRenderApi instance;
            return instance;
        }
        OpenGLRenderApi(OpenGLRenderApi const&) = delete;
        void operator=(OpenGLRenderApi const&)  = delete;

    public:
        void init();

        void setClearColor(float red, float green, float blue, float alpha);
        void clear();

    };

}