#ifndef _ITKIMGFILTER_H_
#define _ITKIMGFILTER_H_

#include "baseFilter.h"
#include <itkCannyEdgeDetectionImageFilter.h>

class itkImgFilter : public baseFilter
{
	typedef itk::CannyEdgeDetectionImageFilter< UTILS::ScalarImageType, UTILS::ScalarImageType > cannyFilterType;
	typedef itk::ImageFileReader< UTILS::ScalarImageType > readerType;
	
	cannyFilterType::Pointer cannyFilter;
	readerType::Pointer reader;

protected:
	virtual void setFilterInput( const vcl_string &filename );
	virtual vtkSmartPointer<vtkImageActor> getFilterOutput( void );

public:
	itkImgFilter();
	void SetVariance( const float x );
	void SetThreshold( const float x );
};

#endif //_ITKIMGFILTER_H_