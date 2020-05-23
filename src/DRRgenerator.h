#ifndef DRRGENERATOR_H
#define DRRGENERATOR_H
#include <math.h>
#include <omp.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <ctime>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <vector>
#include "niftireader.h"

#define _USE_MATH_DEFINES

using namespace Eigen;
using namespace std;

struct CameraDataGPU {
  cv::Mat intrinsics_video;
  cv::Mat extrinsics_video;
  CameraDataGPU() {}
};

template <typename T>
class CTvolume {
 public:
  int size_x, size_y, size_z;
  float voxel_size_x, voxel_size_y, voxel_size_z;
  T default_value;
  T *data;
  float *offset;
  int typevalue;

  CTvolume() {}

  CTvolume(int sx, int sy, int sz, double voxel_sx, double voxel_sy,
           double voxel_sz, T const default_v)
      : size_x(sx),
        size_y(sy),
        size_z(sz),
        voxel_size_x(voxel_sx),
        voxel_size_y(voxel_sy),
        voxel_size_z(voxel_sz),
        default_value(default_v) {
    data = new T[sx * sy * sz];
    std::fill_n(data, sx * sy * sz, default_v);
    offset = new float[3];
    std::fill_n(offset, 3, 0);
    typevalue = 1;
  }

  T &at(int x, int y, int z) {
    return data[x + size_x * y + size_x * size_y * z];
  }
  T at(int x, int y, int z) const {
    return data[x + size_x * y + size_x * size_y * z];
  }
};

class DRRgenerator {
 private:
  CTvolume<short> CTvol;
  CameraDataGPU cam;
  void split(const string &s, vector<string> &elems);
  void setCameraExtrinsics();
  NiftiReader reader;
  float roll_, pitch_, yaw_;  // in degrees
  bool invert;                // if true, denser structure are shown darker
  double width, level;
  float resizefactor;
  struct CTnum_LAC {
    float ctnum;
    float LAC;
  };
  std::vector<CTnum_LAC> LUT = {
      {21, 0.000289},    {282, 0.0747},   {443, 0.11205},  {909, 0.13708},
      {911, 0.09568},    {970, 0.18216},  {1016, 0.153},   {1027, 0.230945},
      {1029, 0.159355},  {1113, 0.17135}, {1114, 0.29268}, {1143, 0.1888},
      {0.3744, 0.33813}, {1301, 0.5544},  {1329, 1.06485}, {1571, 0.90852},
      {2034, 1.6224},    {2516, 2.484},
  };

 public:
  DRRgenerator(float rx = 0.0, float ry = 0.0, float rz = 0.0,
               float resizefactor = 1.0, bool invert = false);

  void load_CT(std::string filename);
  Eigen::Isometry3f cv2eigeniso(cv::Mat transfo);
  void raytracegpu(cv::Mat &color);
  float trilinear_interpolation(cv::Point3f pt);
  float attenuation_lookup_hu(float pix_density);
  float attenuation_lookup_LUT(float pix_density);
  float attenuation_lookup(float pix_density);
  void findentryandexitpoint(Vector3f startpoint, Vector3f ray,
                             double &texitpoint, double &tentrypoint);
};

#endif  // DRRGENERATOR_H
