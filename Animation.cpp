/*
    Copyright (c) 2024 Strayfade
*/

#include "Animation.h"

#include <thread>
#include <chrono>
#include <cmath>

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

void Animation::Animation::ThreadDelegate(Animation* Instigator) {
    Instigator->StartTime = TimePointToTimestamp(std::chrono::high_resolution_clock::now());
    Instigator->OnStarted();

    // Loop
    while ((Instigator->InternalSpeed < 0) ? (Progress > 0) : (Progress < 1)) {
        AnimTimestampType CurrentTime = TimePointToTimestamp(std::chrono::high_resolution_clock::now());
        AnimTimestampType EndTime = StartTime + (Instigator->Duration * abs(Instigator->InternalSpeed));
        Instigator->Progress = Instigator->SmoothingFunction((EndTime - StartTime) / (CurrentTime - StartTime));
        if (Instigator->InternalSpeed < 0)
            Instigator->Progress = Instigator->Progress * -1 + 1;
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
    this->SetSpeed(PlayRate);
    std::thread Thread(ThreadDelegate, this);
    this->ThreadRef = &Thread;
}
void Animation::Animation::SetSpeed(float PlayRate) {
    this->InternalSpeed = 1 / PlayRate;
}
void Animation::Animation::Stop() {
    if (this->ThreadRef != nullptr)
        this->ThreadRef->join();
    this->ThreadRef = nullptr;
}