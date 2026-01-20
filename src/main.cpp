#include <cmath>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  int width;
  int height;
  if (argc >= 3) {
    width = std::stoi(argv[1]);
    height = std::stoi(argv[2]);
  } else {
    width = 200;
    height = 200;
  }
  double fov = 60.0 * M_PI / 180.0;
  double aspect = double(width) / height;

  std::cout << "Ray Tracer "
            << "\n";
  std::cout << "...................................."
            << "\n";
  std::cout << "width = " << width << "\n";
  std::cout << "height = " << height << "\n";
  std::cout << "fov = " << fov << "\n";
  std::cout << "aspect = " << aspect << "\n";

  // Output Image in greyscale
  std::vector<unsigned char> image(height * width);

  // Naively iterate accross each pixel, get angle and integrate the
  // geodesic.
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
    }
  }

  return 0;
}
