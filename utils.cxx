#include "utils.h"
#include <itkComposeRGBImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>

namespace UTILS {

void redirectVTKWarnings( const vcl_string &fname )
{
	vtkFileOutputWindow *outwin = vtkFileOutputWindow::New();
	outwin->SetFileName( fname.c_str() );
	outwin->SetGlobalWarningDisplay( 0 );
	outwin->SetInstance( outwin );
}

vtkSmartPointer<vtkImageActor> ITKToVTKConvert( RGBImageType::Pointer IP )
{
	//typedef itk::ImageToVTKImageFilter<IMPROC::baseFilter::RGBImageType> itkImageTovtkImageFilterType;
	typedef itk::ImageToVTKImageFilter<RGBImageType> itkImageTovtkImageFilterType;
	itkImageTovtkImageFilterType::Pointer ImageTovtkImageFilter = itkImageTovtkImageFilterType::New();
	ImageTovtkImageFilter->SetInput( IP );
	//ImageTovtkImageFilter->Update();
	
	vtkSmartPointer<vtkImageFlip> flipY = vtkSmartPointer<vtkImageFlip>::New();
	flipY->SetInput( ImageTovtkImageFilter->GetOutput() );
	flipY->SetFilteredAxis(1);
	flipY->Update();

	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
	actor->SetInput( flipY->GetOutput() );
	return actor;
}

RGBImageType::Pointer ComposeRGBImage( ScalarImageType::Pointer scalarImage )
{
	typedef itk::RescaleIntensityImageFilter<ScalarImageType,ScalarImageType> RescaleFilterType;
	RescaleFilterType::Pointer rescalefilter = RescaleFilterType::New();
	rescalefilter->SetInput( scalarImage );
	rescalefilter->SetOutputMinimum(0);
	rescalefilter->SetOutputMaximum(255);

	typedef itk::ComposeRGBImageFilter< ScalarImageType, RGBImageType > composefilterType;
	composefilterType::Pointer composeFilter = composefilterType::New();

	composeFilter->SetInput1( rescalefilter->GetOutput() );
	composeFilter->SetInput2( rescalefilter->GetOutput() );
	composeFilter->SetInput3( rescalefilter->GetOutput() );
	composeFilter->Update();
	return composeFilter->GetOutput();
}

} //namespace UTILS