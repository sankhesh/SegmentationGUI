/**
  * \file: baseFilter.h
  * \author: Sankhesh Jhaveri
  * \brief: declares base filter type
**/

#ifndef __BASEFILTER_H__
#define __BASEFILTER_H__

#include <itkImage.h>
#include <itkRGBPixel.h>
#include <itkRGBToLuminanceImageFilter.h>
#include <itkComposeRGBImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>

namespace IMPROC {

typedef unsigned char ImagePixelType;

class baseFilter
{
public:

   typedef itk::Image< itk::RGBPixel< ImagePixelType >, 2 > RGBImageType;

protected:

   RGBImageType::ConstPointer input;
   RGBImageType::ConstPointer output;

   virtual void setFilterInput( const RGBImageType::ConstPointer &img ) = 0;
   virtual RGBImageType::ConstPointer getFilterOutput( void ) = 0;

public:

   baseFilter() :
      input( RGBImageType::New() ),
      output( RGBImageType::New() )
   {}
   virtual ~baseFilter() {}

   void setInput( const RGBImageType::ConstPointer &img ) { this->input = img; this->setFilterInput( this->input ); }
   void setInput( RGBImageType::Pointer &img ) { this->setInput( img.GetPointer() ); }
   RGBImageType::ConstPointer getOutput( void ) { this->output = this->getFilterOutput(); return this->output; }
};

class baseScalarFilter : public baseFilter
{
public:

   typedef itk::Image< ImagePixelType, 2 > ScalarImageType;

protected:

   typedef itk::RGBToLuminanceImageFilter<RGBImageType,ScalarImageType> LuminanceFilterType;
   typedef itk::ComposeRGBImageFilter<ScalarImageType,RGBImageType> ComposeRGBFilterType;

   ScalarImageType::ConstPointer scalarinput;
   ScalarImageType::ConstPointer scalaroutput;
   LuminanceFilterType::Pointer luminancefilter;
   ComposeRGBFilterType::Pointer composefilter;

   virtual void setFilterInput( const RGBImageType::ConstPointer &img );
   virtual void setScalarFilterInput( const ScalarImageType::ConstPointer &img ) = 0;
   virtual RGBImageType::ConstPointer getFilterOutput( void );
   virtual ScalarImageType::ConstPointer getScalarFilterOutput( void ) = 0;

public:

   baseScalarFilter() :
      scalarinput( ScalarImageType::New() ),
      scalaroutput( ScalarImageType::New() ),
      luminancefilter( LuminanceFilterType::New() ),
      composefilter( ComposeRGBFilterType::New() )
	{}
};

class baseScalarRescaledFilter : public baseScalarFilter
{
protected:

   typedef itk::RescaleIntensityImageFilter<ScalarImageType,ScalarImageType> RescaleFilterType;

   RescaleFilterType::Pointer rescalefilter;

   virtual RGBImageType::ConstPointer getFilterOutput( void );

public:

   baseScalarRescaledFilter() :
      rescalefilter( RescaleFilterType::New() )
   {}
};

}

#endif   // __BASEFILTER_H__
