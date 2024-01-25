/*
    Copyright (c) 2024 Strayfade
*/

#include <iostream>
#include <thread>

#ifndef ANIM_H

// Config
#define AnimProgressType long double // Stores progress amount for each animation
#define AnimValueType long double // Stores position (output) amount for each animation
#define AnimTimestampType long double // Stores (ideally) UNIX timestamps to millisecond precision
#define AnimPi static_cast<AnimProgressType>(3.14159265)

namespace Animation {
    struct AnimValue {
        AnimValueType x, y, z;
    };

    AnimProgressType ExampleSmoothingFunction(AnimProgressType x);
    void ExampleStateCallback();
    void ExampleProgressCallback(AnimProgressType CurrentProgress, AnimValue CurrentValue);

    class Animation {
    private:
        // State
        AnimTimestampType StartTime;
        float InternalSpeed = 1.0f;
        
        // Threads
        std::thread* ThreadRef = nullptr;
        void ThreadDelegate();
    public:
        // State
        AnimProgressType Progress;
        AnimValue CurrentValue;

        // Appearance
        AnimProgressType(*SmoothingFunction)(AnimProgressType) = ExampleSmoothingFunction;
        int Duration = 1000; // Milliseconds
        int Throttle = 1; // Milliseconds between animation ticks
        AnimValue StartPosition, EndPosition; // x, y, z

        // Callbacks
        void(*OnStarted)() = ExampleStateCallback;
        void(*OnCompleted)() = ExampleStateCallback;
        void(*OnProgressChanged)(AnimProgressType, AnimValue) = ExampleProgressCallback;

        // User Control
        void Start(float PlayRate = 1.0f);
        void Stop();
    };
}

#define ANIM_H
#endif