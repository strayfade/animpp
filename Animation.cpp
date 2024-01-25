/*
    Copyright (c) 2024 Strayfade
*/

#include "Animation.h"

#include <chrono>
#include <cmath>
#include <functional>

AnimProgressType Animation::ExampleSmoothingFunction(AnimProgressType x) {
    // Ease in/out
    // https://www.desmos.com/calculator/qf505gcetr
    return (cos(x * AnimPi) * -1 + 1) / 2;
}
void Animation::ExampleStateCallback() {
    return;
}
void Animation::ExampleProgressCallback(AnimProgressType CurrentProgress, AnimValue CurrentValue) {
    return;
}

AnimTimestampType TimePointToTimestamp(const std::chrono::steady_clock::time_point& TimePoint) {
    auto Duration = TimePoint.time_since_epoch();
    auto Milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(Duration);
    return static_cast<AnimTimestampType>(Milliseconds.count());
}
Animation::AnimValue LinearInterpolate(Animation::AnimValue Start, Animation::AnimValue End, AnimProgressType Progress) {
    Animation::AnimValue Out;
    Out.x = (End.x - Start.x) * Progress + Start.x;
    Out.y = (End.y - Start.y) * Progress + Start.y;
    Out.z = (End.z - Start.z) * Progress + Start.z;
    return Out;
}

void Animation::Animation::ThreadDelegate() {
    Animation* Instigator = this;
    Instigator->StartTime = TimePointToTimestamp(std::chrono::high_resolution_clock::now());
    Instigator->OnStarted();

    // Loop
    AnimTimestampType EndTime = Instigator->StartTime + (Instigator->Duration * abs(Instigator->InternalSpeed));
    AnimTimestampType CurrentTime = TimePointToTimestamp(std::chrono::high_resolution_clock::now());
    while (CurrentTime < EndTime) {
        CurrentTime = TimePointToTimestamp(std::chrono::high_resolution_clock::now());
        Instigator->Progress = Instigator->SmoothingFunction((CurrentTime - Instigator->StartTime) / (EndTime - Instigator->StartTime));
        Instigator->CurrentValue = LinearInterpolate(Instigator->StartPosition, Instigator->EndPosition, Instigator->Progress);
        Instigator->OnProgressChanged(Instigator->Progress, Instigator->CurrentValue);
        std::this_thread::sleep_for(std::chrono::milliseconds(Instigator->Throttle));
    }
    Instigator->Progress = (Instigator->InternalSpeed < 0) ? 0 : 1;

    // Exit
    Instigator->OnProgressChanged(Instigator->Progress, Instigator->CurrentValue);
    Instigator->OnCompleted();
}

void Animation::Animation::Start(float PlayRate) {
    this->InternalSpeed = 1 / PlayRate;
    std::thread Thread(std::bind(&Animation::ThreadDelegate, this));
    //this->ThreadRef = &Thread;
}
void Animation::Animation::Stop() {
    //if (this->ThreadRef != nullptr)
    //    this->ThreadRef->join();
    //this->ThreadRef = nullptr;
}