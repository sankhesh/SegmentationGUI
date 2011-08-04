/**
  * \file: itkImgFilter.cxx
  * \author: Sankhesh Jhaveri
  * \brief: defines itkImgFilter class
**/

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
	vcl_try
	{
		this->cannyFilter->Update();
	}
	vcl_catch (itk::ExceptionObject &err)
	{
		vcl_cout << vcl_endl
               << "***" << vcl_endl
               << "Exception caught:" << vcl_endl
               << err << vcl_endl
               << "***" << vcl_endl
               << vcl_flush;
		return NULL;
	}
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