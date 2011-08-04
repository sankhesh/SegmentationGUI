/**
  * \file: vxlImgFilter.h
  * \author: Sankhesh Jhaveri
  * \brief: declares vxlImgFilter class. This class reads a vil image, performs edge detection
  *			and converts the vil image to ITK image to return a VTK image actor.
**/

#ifndef _VXLIMGFILTER_H_
#define _VXLIMGFILTER_H_

#include "baseFilter.h"
#include <vil/vil_image_view.h>
#include <vxl_config.h>

class vxlImgFilter : public baseFilter
{
	float thresh1, thresh2;
	vil_image_view<vxl_byte> inputImage;
	vil_image_view<UTILS::ScalarPixelType> internalImage;
	void PerformFiltering( void );
	UTILS::ScalarImageType::Pointer ConvertVXLImgToITKImg( void );

protected:
	virtual void setFilterInput( const vcl_string &filename );
	virtual vtkSmartPointer<vtkImageActor> getFilterOutput( void );

public:
	vxlImgFilter();
	void SetNonMaxSuppressionThreshold( const float x )
	{
		this->thresh1 = x;
	}
	void SetMagnitudeThreshold( const float x )
	{
		this->thresh2 = x;
	}
};

#endif //_VXLIMGFILTER_H_