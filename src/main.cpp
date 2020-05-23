#include <argparse.h>
#include <chrono>
#include <iostream>
#include "DRRgenerator.h"
using namespace std;
using namespace argparse;

int main(int argc, const char **argv) {
  ArgumentParser parser("Parse CT file path");
  parser.enable_help();
  parser.add_argument("-f", "--file", "file path", true);
  parser.add_argument("-t", "--type", "file type");
  parser.add_argument("-r", "--rot", "rotation x y z ");
  parser.add_argument("-o", "--output", "output file name");
  parser.add_argument("-s", "--size", "size of the output image");
  parser.add_argument("-i", "--inverse",
                      "invert x-ray? yes-> dark values for dense structure");

  auto err = parser.parse(argc, argv);

  bool invert = false;
  if (parser.exists("i")) invert = true;
  string filename = parser.get<std::string>("f");

  float roll = 0.0, pitch = 0.0, yaw = 0.0;
  if (parser.exists("r")) {
    auto rotations = parser.get<std::vector<float>>("r");
    roll = rotations[0];
    pitch = rotations[1];
    yaw = rotations[2];
  }

  int sx = 512, sy = 512;
  if (parser.exists("s")) {
    std::vector<int> size = parser.get<std::vector<int>>("s");
    sx = size[0];
    sy = size[1];
    std::for_each(size.begin(), size.end(),
                  [](const int &e) { std::cout << e << " "; });
  }
  const auto startct = chrono::system_clock::now();
  DRRgenerator drrgene(roll, pitch, yaw, sx, sy, invert);
  drrgene.load_CT(filename);

  const auto stopct = chrono::system_clock::now();
  const auto duratct =
      chrono::duration_cast<chrono::milliseconds>(stopct - startct).count();
  std::cout << "CT loaded in " << duratct << " ms" << std::endl;

  cv::Mat color;
  const auto start = chrono::system_clock::now();
  drrgene.raytracegpu(color);
  const auto stop = chrono::system_clock::now();
  const auto durat =
      chrono::duration_cast<chrono::milliseconds>(stop - start).count();
  std::cout << "DRR created in " << durat << " ms" << std::endl;

  cv::namedWindow("DRR", CV_WINDOW_KEEPRATIO);
  cv::imshow("DRR", color);

  if (parser.exists("output")) {
    std::string out_filename = "output.png";
    if (parser.exists("o")) {
      out_filename = parser.get<std::string>("o");
    }
    std::cout << "Writing DRR to file " << out_filename << std::endl;

    std::cout << "Matrix size:" << color.size() << std::endl;
    cv::imwrite(out_filename, color);
  }

  cv::waitKey(0);
  return 0;
}
