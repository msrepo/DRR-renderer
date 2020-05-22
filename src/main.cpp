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
      .names({"-f", "--file"})
      .description("file path")
      .required(true);

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
  std::string CT_folder = "data/";
  std::string raw_filename = CT_folder + "FullHead.raw";
  std::string info_filename = CT_folder + "FullHead.mhd";

  const std::string DEFAULT_FILETYPE = "MHD";
  std::string filetype = DEFAULT_FILETYPE;

  if (parser.exists("t")) {
    filetype = parser.get<std::string>("t");
  }

  if (parser.exists("f")) {
    info_filename = parser.get<std::string>("f");
    info_filename = info_filename.substr(0, info_filename.rfind(".")) + ".mhd";
    raw_filename = info_filename.substr(0, info_filename.rfind(".")) + ".raw";
  }

  const auto startct = chrono::system_clock::now();

  drrgene.load_CT(raw_filename, info_filename);

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

  std::string out_filename =
      info_filename.substr(0, info_filename.rfind(".")) + ".png";
  std::cout << "Writing DRR to file " << out_filename << std::endl;

  std::cout << "Matrix size:" << color.size() << std::endl;
  cv::imwrite(out_filename, color);
  cv::waitKey(0);
  return 0;
}
