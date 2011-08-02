#ifndef _ITKCANNYFILTER_H_
#define _ITKCANNYFILTER_H_

#include "utils.h"
#include <itkCannyEdgeDetectionImageFilter.h>

class itkCannyFilter
{
	UTILS::ScalarImageType::Pointer scalarinput;
	UTILS::ScalarImageType::Pointer scalaroutput;
	
	typedef itk::CannyEdgeDetectionImageFilter< UTILS::ScalarImageType, UTILS::ScalarImageType > cannyfilterType;
	cannyfilterType::Pointer cannyFilter;

public:
	itkCannyFilter();
	~itkCannyFilter(){}

	void setRGBInput( UTILS::RGBImageType::Pointer rgbimage);
	
	void setScalarInput( UTILS::ScalarImageType::Pointer scalarImage)
	{ this->scalarinput = scalarImage )

	UTILS::ScalarImageType::Pointer getScalarOutput( void );
	
	UTILS::RGBImageType::Pointer getRGBOutput( void );

};

#endif //_ITKCANNYFILTER_H_