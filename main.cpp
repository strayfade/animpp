#include <iostream>
#include <Windows.h>
#include "Animation.h"

void Callback(AnimProgressType CurrentProgress, Animation::AnimValue CurrentValue) {
    std::cout << CurrentProgress << std::endl;
}

// https://www.desmos.com/calculator/qf505gcetr
AnimProgressType Smoothing(AnimProgressType x) {
    return (cos(x * AnimPi) * -1 + 1) / 2;
}

int main() {
    Animation::Animation Anim;
    Anim.StartPosition = Animation::AnimValue{ 0, 0, 0 };
    Anim.StartPosition = Animation::AnimValue{ 5, 10, 0 };
    Anim.OnProgressChanged = Callback;

    Anim.Duration = 5000; // ms
    Anim.SmoothingFunction = Smoothing;
    Anim.Start(0.5);

    //std::this_thread::sleep_for(std::chrono::minutes(10));
    Sleep(99999);
}