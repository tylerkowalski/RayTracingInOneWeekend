#include <iostream>

int main() {
    // image
    static constexpr int IMAGE_WIDTH = 256;
    static constexpr int IMAGE_HEIGHT = 256;

    // render
    std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

    for (int m = 0; m < IMAGE_HEIGHT; ++m) {
        std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - m) << ' ' << std::flush;
        for (int n = 0; n < IMAGE_WIDTH; ++n ) {
            auto r = double(n) / (IMAGE_WIDTH - 1);
            auto g = double(m) / (IMAGE_HEIGHT - 1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }   
    }
    std::clog << "\rDone.\n"; 
} 
