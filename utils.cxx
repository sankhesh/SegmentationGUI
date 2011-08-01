#include "utils.h"

namespace UTILS {

void redirectVTKWarnings( const vcl_string &fname )
{
	vtkFileOutputWindow *outwin = vtkFileOutputWindow::New();
	outwin->SetFileName( fname.c_str() );
	outwin->SetGlobalWarningDisplay( 0 );
	outwin->SetInstance( outwin );
}

//vtkSmartPointer<vtkImageActor> ITKToVTKConvert( IMPROC::baseFilter::RGBImageType::ConstPointer IP )
vtkSmartPointer<vtkImageActor> ITKToVTKConvert( RGBImageType::ConstPointer IP )
{
	//typedef itk::ImageToVTKImageFilter<IMPROC::baseFilter::RGBImageType> itkImageTovtkImageFilterType;
	typedef itk::ImageToVTKImageFilter<RGBImageType> itkImageTovtkImageFilterType;
	itkImageTovtkImageFilterType::Pointer ImageTovtkImageFilter = itkImageTovtkImageFilterType::New();
	ImageTovtkImageFilter->SetInput( IP );
	ImageTovtkImageFilter->Update();
	
	vtkSmartPointer<vtkImageFlip> flipY = vtkSmartPointer<vtkImageFlip>::New();
	flipY->SetInput( ImageTovtkImageFilter->GetOutput() );
	flipY->SetFilteredAxis(1);
	flipY->Update();

	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
	actor->SetInput( flipY->GetOutput() );

	return actor;
}

//vtkSmartPointer<vtkImageActor> ITKToVTKConvert( IMPROC::baseFilter::RGBImageType::Pointer IP )
//{
//   return ITKToVTKConvert( IMPROC::baseFilter::RGBImageType::ConstPointer(IP.GetPointer()) );
//}

} //namespace UTILS