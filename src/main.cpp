#include <argparse.h>
#include <chrono>
#include <iostream>
#include "DRRgenerator.h"
using namespace std;
using namespace argparse;

int main(int argc, const char **argv) {
  DRRgenerator drrgene;

  // we only load mhd file
  ArgumentParser parser("Parse CT file path");
  parser.add_argument()
      .names({"-d", "--dir"})
      .description("file directory")
      .required(true);
  parser.add_argument()
      .names({"-r", "--raw"})
      .description("raw file name")
      .required(true);
  parser.add_argument()
      .names({"-i", "--info"})
      .description("info (mhd) file")
      .required(true);
  parser.add_argument()
      .names({"-x", "--resize"})
      .description("resize factor")
      .required(false);
  parser.add_argument()
      .names({"-t", "--type"})
      .description("file type")
      .required(false);
  parser.enable_help();

  auto err = parser.parse(argc, argv);

  if (parser.exists("help")) {
    parser.print_help();
    return 0;
  }
  std::string CT_folder = "../data/";
  std::string filenameCT = CT_folder + "FullHead.raw";
  std::string info_filename = CT_folder + "FullHead.mhd";
  float resize_factor = 1.0f;
  const std::string DEFAULT_FILETYPE = "MHD";
  std::string filetype = DEFAULT_FILETYPE;

  if (parser.exists("t")) {
    filetype = parser.get<std::string>("t");
  }

  if (parser.exists("d")) {
    CT_folder = parser.get<std::string>("d");
  }
  if (parser.exists("r")) {
    filenameCT = CT_folder + parser.get<std::string>("r");
  }
  if (parser.exists("i")) {
    info_filename = CT_folder + parser.get<std::string>("i");
  }
  if (parser.exists("x")) {
    resize_factor = parser.get<float>("x");
  }

  const auto startct = chrono::system_clock::now();
  drrgene.load_CT(filenameCT, info_filename);
  const auto stopct = chrono::system_clock::now();
  const auto duratct =
      chrono::duration_cast<chrono::milliseconds>(stopct - startct).count();
  std::cout << "CT loaded in " << duratct << " ms" << std::endl;

  cv::Mat color;
  const auto start = chrono::system_clock::now();
  drrgene.raytracegpu(color, resize_factor);
  const auto stop = chrono::system_clock::now();
  const auto durat =
      chrono::duration_cast<chrono::milliseconds>(stop - start).count();
  std::cout << "DRR created in " << durat << " ms" << std::endl;
  cv::namedWindow("DRR", CV_WINDOW_KEEPRATIO);
  cv::imshow("DRR", color);
  std::string filename = parser.get<std::string>("r");
  filename = filename.substr(0, filename.find(".")) + ".png";
  std::cout << "Writing DRR to file " << filename << std::endl;
  std::cout << "Matrix size:" << color.size() << std::endl;
  cv::imwrite(filename, color);
  cv::waitKey(0);
  return 0;
}
