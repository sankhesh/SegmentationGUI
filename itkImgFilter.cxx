#include "itkImgFilter.h"


//Constructor
itkImgFilter::itkImgFilter() :
	  cannyFilter( cannyFilterType::New() ),
      reader( readerType::New() )
{
	  this->cannyFilter->SetInput( this->reader->GetOutput() );
	  this->cannyFilter->SetVariance( 1 );
	  this->cannyFilter->SetUpperThreshold( 1 );
}

void itkImgFilter::setFilterInput(const std::string &filename)
{
	this->reader->SetFileName( filename );
}

vtkSmartPointer<vtkImageActor> itkImgFilter::getFilterOutput( void )
{
	this->cannyFilter->Update();
	UTILS::ScalarImageType::Pointer img = this->cannyFilter->GetOutput();
	return UTILS::ITKToVTKConvert( UTILS::ComposeRGBImage( img ) );
}

void itkImgFilter::SetVariance( const float x )
{
	this->cannyFilter->SetVariance( x );
}

void itkImgFilter::SetThreshold( const float x )
{
	this->cannyFilter->SetUpperThreshold( x );
}