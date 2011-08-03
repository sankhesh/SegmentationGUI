#ifndef _VTKIMGFILTER_H_
#define _VTKIMGFILTER_H_

#include "baseFilter.h"

#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkStructuredPoints.h>
#include <vtkImageCast.h>
#include <vtkImageLuminance.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkImageGradient.h>
#include <vtkImageMagnitude.h>
#include <vtkImageNonMaximumSuppression.h>
#include <vtkImageConstantPad.h>
#include <vtkImageToStructuredPoints.h>
#include <vtkLinkEdgels.h>
#include <vtkThreshold.h>
#include <vtkGeometryFilter.h>
#include <vtkSubPixelPositionEdgels.h>
#include <vtkStripper.h>
#include <vtkImageMapper3D.h>
#include <vtkProperty.h>
#include <vtkImageProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
class vtkImgFilter : public baseFilter
{
	vtkSmartPointer<vtkImageReader2Factory> readerFactory;
	vtkSmartPointer<vtkImageReader2> reader;
	vtkSmartPointer<vtkImageLuminance> luminanceFilter;
	vtkSmartPointer<vtkImageCast> castFilter;
	vtkSmartPointer<vtkImageGaussianSmooth> gaussianFilter;
	vtkSmartPointer<vtkImageGradient> gradientFilter;
	vtkSmartPointer<vtkImageMagnitude> magnitudeFilter;
	vtkSmartPointer<vtkImageNonMaximumSuppression> nonMaxFilter;
	vtkSmartPointer<vtkImageConstantPad> padFilter;
	vtkSmartPointer<vtkImageToStructuredPoints> im2spFilter1;
	vtkSmartPointer<vtkLinkEdgels> linkFilter;
	vtkSmartPointer<vtkThreshold> threshFilter;
	vtkSmartPointer<vtkGeometryFilter> geomFilter;
	vtkSmartPointer<vtkImageToStructuredPoints> im2spFilter2;
	vtkSmartPointer<vtkSubPixelPositionEdgels> sppeFilter;
	vtkSmartPointer<vtkStripper> stripFilter;
	vtkSmartPointer<vtkPolyDataMapper> mapper;
	vtkSmartPointer<vtkActor> outputActor;

protected:
	virtual void setFilterInput( const vcl_string &filename );
	virtual vtkSmartPointer<vtkImageActor> getFilterOutput( void );

public:
	vtkImgFilter();
	void SetRadius( double x[3] );
	void SetStdDev( double x[3] );
	void SetThreshold( double x );
	vtkSmartPointer<vtkActor> getOutputActor( void );
};
#endif //_VTKIMGFILTER_H_