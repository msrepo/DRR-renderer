#include <argparse.h>
#include <chrono>
#include <iostream>
#include <string>
#include "DRRgenerator.h"
using namespace std;
using namespace argparse;

int main(int argc, const char **argv) {
  ArgumentParser parser("Parse CT file path");
  parser.enable_help();
  parser.add_argument("-f", "--file", "file path", true);
  parser.add_argument("-x", "--resize", "resize factor", false);
  parser.add_argument("-r", "--rot", "rotation x y z ", false);
  parser.add_argument("-u", "--resolution", false);
  parser.add_argument("-i", "--inverse",
                      "invert x-ray? yes-> dark values for dense structure",
                      false);
  parser.add_argument("-o", "--output", "output file name", false);

  auto err = parser.parse(argc, argv);

  if (parser.exists("help")) {
    parser.print_help();
    return 0;
  }

  bool invert = false;
  if (parser.exists("i")) invert = true;
  float resizefactor = 1.0;
  if (parser.exists("x")) resizefactor = parser.get<float>("x");
  if (parser.exists("o"))
    std::cout << "Output " << parser.get<std::string>("o") << '\n';
  string filename = parser.get<std::string>("f");

  float roll = 0.0, pitch = 0.0, yaw = 0.0;
  if (parser.exists("r")) {
    auto rotations = parser.get<std::vector<float>>("r");
    roll = rotations[0];
    pitch = rotations[1];
    yaw = rotations[2];
  }

  const auto startct = chrono::system_clock::now();
  DRRgenerator drrgene(roll, pitch, yaw, resizefactor, invert);
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

  cv::namedWindow("DRR-" + filename, CV_WINDOW_KEEPRATIO);
  cv::imshow("DRR-" + filename, color);
  std::cout << "Matrix size:" << color.size() << std::endl;
  if (parser.exists("o")) {
    std::string out_filename = "output.png";
    out_filename = parser.get<std::string>("o");
    std::cout << "Writing DRR to file " << out_filename << std::endl;

    cv::imwrite(out_filename, color);
  }

  cv::waitKey(0);
  return 0;
}
