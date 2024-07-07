#include "audioTemplate.hpp"

int main() {
    audioTemplate<float> app(48000, 128, 2, 2);
    app.start();
    return 0;
}