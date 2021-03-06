/**
  * \file: utils.h
  * \author: Sankhesh Jhaveri
  * \brief: declares UTILS namespace
**/

#ifndef _UTILS_H_
#define _UTILS_H_

#include <vcl_string.h>

#include <vtkFileOutputWindow.h>
#include <vtkSmartPointer.h>
#include <vtkImageFlip.h>
#include <itkRGBPixel.h>
#include <itkImageToVTKImageFilter.h>
#include <itkImageFileReader.h>
#include <vtkImageActor.h>


namespace UTILS {

typedef itk::RGBPixel< unsigned char > PixelType;
typedef itk::Image< PixelType, 2 > RGBImageType;
typedef itk::ImageFileReader< RGBImageType > ReaderType;
typedef double ScalarPixelType;
typedef itk::Image< ScalarPixelType, 2 > ScalarImageType;


void redirectVTKWarnings( const vcl_string &fname );

vtkSmartPointer<vtkImageActor> ITKToVTKConvert( RGBImageType::Pointer IP );

RGBImageType::Pointer ComposeRGBImage ( ScalarImageType::Pointer scalarImage );

} //namespace UTILS

#endif //_UTILS_H_