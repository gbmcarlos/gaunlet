#pragma once

namespace gaunlet::Core {

    class TimeStep {

    private:
        float time;

    public:
        TimeStep(float time = 0)
            : time(time) {};

        operator float() const {return time;}

        inline float getSeconds() {return time;}
        inline float getMilliseconds() {return time * 1000;}

    };

}