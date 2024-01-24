/*
    Copyright (c) 2024 Strayfade
*/

#include <iostream>

#ifndef ANIM_H

// Config
#define AnimProgressType float // Stores progress amount for each animation
#define AnimValueType float // Stores position (output) amount for each animation
#define AnimTimestampType long double // Stores (ideally) UNIX timestamps to millisecond precision
#define AnimPi 3.14159265

namespace Animation {
    AnimProgressType ExampleSmoothingFunction(AnimProgressType x);
    void ExampleStateCallback();
    void ExampleProgressCallback(AnimProgressType CurrentProgress, AnimValue CurrentValue);

    struct AnimValue {
        AnimValueType x, y, z;
    };

    class Animation {
    private:
        // State
        AnimTimestampType StartTime;
        float InternalSpeed = 1.0f;
        
        // Threads
        void ThreadDelegate(Animation* Instigator);
        std::thread* ThreadRef = nullptr;
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
        void(*OnStarted)() = ExampleCallback;
        void(*OnCompleted)() = ExampleCallback;
        void(*OnProgressChanged)(AnimProgressType, AnimValue) = ExampleProgressCallback;

        // User Control
        void Start(float PlayRate = 1.0f);
        void SetSpeed(float PlayRate = 1.0f);
        void Stop();
    };
}

#define ANIM_H
#endif