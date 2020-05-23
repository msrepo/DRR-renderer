#ifndef NIFTIREADER_H
#define NIFTIREADER_H

#include <itkImageFileReader.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>
#include <string>
#include <vector>
#include "assert.h"

using PixelType = short;
const int Dimension = 3;
using ImgType = itk::Image<PixelType, Dimension>;
using ImgReaderType = itk::ImageFileReader<ImgType>;
using ConstIteratorType = itk::ImageRegionConstIterator<ImgType>;

class NiftiReader {
 public:
  void initialize() {
    assert(!filename.empty());
    imgreader = ImgReaderType::New();
    imgreader->SetFileName(filename);
    imgreader->Update();

    img = imgreader->GetOutput();
    ConstIteratorType constIter(img, img->GetRequestedRegion());
    imgIter = constIter;
  }

 public:
  NiftiReader() {}
  void setFilename(std::string fname) { filename = fname; }
  NiftiReader(std::string fname) {
    filename = fname;
    initialize();
  }
  ConstIteratorType imgIterator() { return imgIter; }

  template <int i>
  double getVoxelSize() {
    return imgreader->GetImageIO()->GetSpacing(i);
  }
  template <int i>
  int getSize() {
    return imgreader->GetImageIO()->GetDimensions(i);
  }
  template <int i>
  double getOrigin() {
    return imgreader->GetImageIO()->GetOrigin(i);
  }
  std::string PixelType() {
    return imgreader->GetImageIO()->GetComponentTypeAsString(
        imgreader->GetImageIO()->GetComponentType());
  }

  float Value(int i, int j, int k) {
    ImgType::IndexType index;
    index[0] = i;
    index[1] = j;
    index[2] = k;
    return img->GetPixel(index);
  }

 private:
  std::string filename;
  ImgReaderType::Pointer imgreader;
  ImgType::Pointer img;
  ConstIteratorType imgIter;
};

#endif  // NIFTIREADER_H
