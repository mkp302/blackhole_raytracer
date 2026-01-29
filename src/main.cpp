#include "deflectionTable.h"
#include "deflectionTableBuilder.h"
#include "observer.h"
#include "schwarzschildGeodesic.h"
#include "schwarzschildMetric.h"
#include "vec3.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct HDRImage {
  int width;
  int height;
  int channels;
  std::vector<float> data; // RGBRGB...
};

HDRImage load_hdr(const std::string &filename) {
  HDRImage img;

  stbi_set_flip_vertically_on_load(true);

  float *pixels =
      stbi_loadf(filename.c_str(), &img.width, &img.height, &img.channels, 3);

  if (!pixels) {
    throw std::runtime_error("Failed to load HDR image");
  }

  img.channels = 3;
  img.data.assign(pixels, pixels + img.width * img.height * 3);

  stbi_image_free(pixels);
  return img;
}

Vec3 sample_hdr(const HDRImage &hdr, int hx, int hy) {
  // hx = (hx % hdr.width + hdr.width) % hdr.width; // wrap horizontally
  // hy = std::clamp(hy, 0, hdr.height - 1);        // clamp vertically

  int idx = 3 * (hy * hdr.width + hx);

  return {hdr.data[idx + 0], hdr.data[idx + 1], hdr.data[idx + 2]};
}

double interpolate_phi(double delta, const DeflectionTable &table) {
  if (delta <= table.table.front().delta)
    return table.table.front().chi;
  if (delta >= table.table.back().delta)
    return table.table.back().chi;
  // find interval
  auto it = std::lower_bound(
      table.table.begin(), table.table.end(), delta,
      [](const Deflection &d, double v) { return d.delta < v; });
  auto d1 = *(it - 1), d2 = *it;
  double t = (delta - d1.delta) / (d2.delta - d1.delta);
  return d1.chi + t * (d2.chi - d1.chi);
}

void render_hdr(std::vector<Vec3> &framebuffer, int W, int H, double fov,
                const DeflectionTable &table, const HDRImage &hdr) {
  double aspect = double(W) / double(H);
  double f = 1.0 / std::tan(0.5 * fov);
  double lon0 = 0.;
  double lat0 = 0.;

  for (int j = 0; j < H; ++j) {
    for (int i = 0; i < W; ++i) {

      double x = (2.0 * (i + 0.5) / W - 1.0) * aspect;
      double y = (1.0 - 2.0 * (j + 0.5) / H);
      Vec3 dir = {x, y, -1};
      double inv_len =
          1.0 / std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
      dir.x *= inv_len;
      dir.y *= inv_len;
      dir.z *= inv_len;

      double delta = acos(-dir.z);
      double psi = atan2(dir.y, dir.x);

      if (delta < table.deltaMin) {
        // framebuffer[j * W + i] = Vec3{0, 0, 0};
        // continue;
      }

      if (abs(delta - table.deltaMin) < 0.001) {
        // framebuffer[j * W + i] = Vec3{.9, .9, .9};
        // continue;
      }

      double chi = table.sample(delta);
      double delta_inf = delta; // + chi;

      Vec3 n;
      n.x = sin(delta_inf) * cos(psi);
      n.y = sin(delta_inf) * sin(psi);
      n.z = cos(delta_inf);

      double lon = atan2(n.y, n.x);
      if (lon < 0)
        lon += 2 * M_PI;

      double lat = acos(n.z);
      lat += lat0;
      lon += lon0;

      int hx = int(lon / (2 * M_PI) * hdr.width);
      int hy = int(lat / M_PI * hdr.height);

      framebuffer[j * W + i] = sample_hdr(hdr, hx, hy) * 60.0;
    }
  }
}

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

  std::vector<Vec3> image(height * width);

  const double M = 1.0;
  Observer observer = Observer({0., 30 * M, M_PI / 2.0, 0.0});

  SchwarzschildMetric metric = SchwarzschildMetric(M);
  observer.tetrad = metric.reference_tetrad(observer.position);
  SchwarzschildGeodesic geodesic = SchwarzschildGeodesic(observer, &metric);

  RayResult result = geodesic.integrate(1.3, true);
  DeflectionTableBuilder builder = DeflectionTableBuilder(geodesic, 5000);

  DeflectionTable table = builder.build();
  table.dumpTable("deflection.csv");
  HDRImage hdr = load_hdr("src/HDR_rich_blue_nebulae_2.hdr");

  render_hdr(image, width, height, M_PI / 2, table, hdr);
  std::ofstream out("blackhole.ppm", std::ios::binary);
  out << "P6\n" << width << " " << height << "\n255\n";

  for (const Vec3 &c : image) {
    unsigned char r = (unsigned char)(std::clamp(c.x, 0.0, 1.0) * 255.0);
    unsigned char g = (unsigned char)(std::clamp(c.y, 0.0, 1.0) * 255.0);
    unsigned char b = (unsigned char)(std::clamp(c.z, 0.0, 1.0) * 255.0);
    out.write((char *)&r, 1);
    out.write((char *)&g, 1);
    out.write((char *)&b, 1);
  }

  out.close();

  return 0;
}
