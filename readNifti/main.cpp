
#include <argparse.h>
#include <itkImageFileReader.h>
#include <itkImageRegionIterator.h>
#include <itkNiftiImageIO.h>
#include <iostream>
#include "niftireader.h"

using namespace std;
using namespace argparse;
using BaseImage = itk::ImageIOBase;

BaseImage::Pointer getImageIO(string input) {
  BaseImage::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(
      input.c_str(), itk::ImageIOFactory::ReadMode);
  imageIO->SetFileName(input);
  imageIO->ReadImageInformation();
  return imageIO;
}

BaseImage::IOComponentType component_type(BaseImage::Pointer imageIO) {
  return imageIO->GetComponentType();
}

BaseImage::IOPixelType pixel_type(BaseImage::Pointer imageIO) {
  return imageIO->GetPixelType();
}

size_t num_dimensions(BaseImage::Pointer imageIO) {
  return imageIO->GetNumberOfDimensions();
}

int main(int argc, const char** argv) {
  ArgumentParser parser("parse Nifti file");

  parser.add_argument()
      .names({"-f", "--file"})
      .description("filepath")
      .required(true);
  parser.parse(argc, argv);

  if (parser.exists("help")) {
    parser.print_help();
    return 0;
  }
  string filepath;
  if (parser.exists("f")) {
    filepath = parser.get<string>("f");
  } else {
    parser.print_help();
    return 0;
  }
  BaseImage::Pointer imageIO = getImageIO(filepath);

  // In order to read a image, we need its dimensionality and component type
  std::cout << "numDimensions: " << num_dimensions(imageIO) << std::endl;
  std::cout << "component type: "
            << imageIO->GetComponentTypeAsString(component_type(imageIO))
            << std::endl;
  // The pixel type is not necessary. This is just to let you know that it
  // exists
  std::cout << "pixel type: "
            << imageIO->GetPixelTypeAsString(pixel_type(imageIO)) << std::endl;

  using Img = itk::Image<float, 3>;
  using ImgReader = itk::ImageFileReader<Img>;
  using ImgIterator = itk::ImageRegionIterator<Img>;

  ImgReader::Pointer imgreader = ImgReader::New();
  imgreader->SetFileName(imageIO->GetFileName());
  imgreader->Update();

  Img::Pointer img = imgreader->GetOutput();
  Img::RegionType region;
  Img::SizeType size;
  Img::IndexType start;
  for (int i = 0; i < num_dimensions(imageIO); i++) {
    start[i] = 0;
    size[i] = imageIO->GetDimensions(i);
  }
  region.SetIndex(start);
  region.SetSize(size);
  ImgIterator imgIter(img, region);

  NiftiReader filereader(filepath);
  std::cout << "Niftireader(voxel size): " << filereader.getVoxelSize<0>()
            << " " << filereader.getVoxelSize<1>() << " "
            << filereader.getVoxelSize<2>() << std::endl;
  std::cout << "Niftireader(Dimensions): " << filereader.getSize<0>() << " "
            << filereader.getSize<1>() << " " << filereader.getSize<2>()
            << std::endl;
  std::cout << "Niftireader(Offset): " << filereader.getOrigin<0>() << " "
            << filereader.getOrigin<1>() << " " << filereader.getOrigin<2>()
            << std::endl;
  std::cout << "Niftireader(Pixeltype): " << filereader.PixelType() << '\n';
  int i = 0, j = 0, k = 0;
  std::cout << "Niftireader(PixelValue)[0,0,0]: " << filereader.Value(i, j, k)
            << '\n';
  i = 100;
  j = 100;
  k = 10;
  std::cout << "Niftireader(PixelValue)[0,0,0]: " << filereader.Value(i, j, k)
            << '\n';

  return 0;
}
