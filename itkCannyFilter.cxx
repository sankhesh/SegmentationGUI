#include "itkCannyFilter.h"
#include <itkRGBToLuminanceImageFilter.h>

itkCannyFilter::itkCannyFilter() :
	scalarinput( UTILS::ScalarImageType::New() ),
	scalaroutput( UTILS::ScalarImageType::New() ),
	cannyFilter( cannyfilterType::New() )
{
}

void itkCannyFilter::setRGBInput( UTILS::RGBImageType::Pointer rgbimage)
{
	typedef itk::RGBToLuminanceImageFilter< UTILS::RGBImageType, UTILS::ScalarImageType > luminancefilteType;
	luminancefilterType::Pointer luminanceFilter = luminancefilterType::New();

	luminanceFilter->SetInput( rgbimage );
	luminanceFilter->Update();

	this->scalarinput = luminanceFilter->GetOutput();
}


