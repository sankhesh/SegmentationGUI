#include "vtkImgFilter.h"


//Constructor
vtkImgFilter::vtkImgFilter() :
	readerFactory( vtkSmartPointer<vtkImageReader2Factory>::New() ),
	reader( vtkSmartPointer<vtkImageReader2>::New() ),
	luminanceFilter( vtkSmartPointer<vtkImageLuminance>::New() ),
	castFilter( vtkSmartPointer<vtkImageCast>::New() ),
	gaussianFilter( vtkSmartPointer<vtkImageGaussianSmooth>::New() ),
	gradientFilter( vtkSmartPointer<vtkImageGradient>::New() ),
	magnitudeFilter( vtkSmartPointer<vtkImageMagnitude>::New() ),
	nonMaxFilter( vtkSmartPointer<vtkImageNonMaximumSuppression>::New() ),
	padFilter( vtkSmartPointer<vtkImageConstantPad>::New() ), 
	im2spFilter1( vtkSmartPointer<vtkImageToStructuredPoints>::New() ),
	linkFilter( vtkSmartPointer<vtkLinkEdgels>::New() ),
	threshFilter( vtkSmartPointer<vtkThreshold>::New() ),
	geomFilter( vtkSmartPointer<vtkGeometryFilter>::New() ),
	im2spFilter2( vtkSmartPointer<vtkImageToStructuredPoints>::New() ),
	sppeFilter( vtkSmartPointer<vtkSubPixelPositionEdgels>::New() ),
	stripFilter( vtkSmartPointer<vtkStripper>::New() ),
	mapper( vtkSmartPointer<vtkPolyDataMapper>::New() ),
	outputActor( vtkSmartPointer<vtkActor>::New() )
{
	castFilter->SetOutputScalarTypeToFloat();
	castFilter->SetInputConnection( luminanceFilter->GetOutputPort() );
	
	gaussianFilter->SetDimensionality( 2 );
	gaussianFilter->SetInputConnection( castFilter->GetOutputPort() );
	
	gradientFilter->SetDimensionality( 2 );
	gradientFilter->SetInputConnection( gaussianFilter->GetOutputPort() );

	magnitudeFilter->SetInputConnection( gradientFilter->GetOutputPort() );

	nonMaxFilter->SetDimensionality( 2 );
	nonMaxFilter->SetMagnitudeInput( magnitudeFilter->GetOutput() );
	nonMaxFilter->SetVectorInput( gradientFilter->GetOutput() );

	padFilter->SetInputConnection( gradientFilter->GetOutputPort() );
	padFilter->SetOutputNumberOfScalarComponents( 3 );
	padFilter->SetConstant( 0 );

	im2spFilter1->SetInputConnection( nonMaxFilter->GetOutputPort() );
	im2spFilter1->SetVectorInput( padFilter->GetOutput() );

	linkFilter->SetInputConnection( im2spFilter1->GetOutputPort() );
	linkFilter->SetGradientThreshold( 2 );

	threshFilter->SetInputConnection( linkFilter->GetOutputPort() );
	threshFilter->AllScalarsOff();

	geomFilter->SetInputConnection( threshFilter->GetOutputPort() );

	im2spFilter2->SetInputConnection( magnitudeFilter->GetOutputPort() );
	im2spFilter2->SetVectorInput( padFilter->GetOutput() );

	sppeFilter->SetInputConnection( geomFilter->GetOutputPort() );
	sppeFilter->SetGradMaps( im2spFilter2->GetStructuredPointsOutput() );

	stripFilter->SetInputConnection( sppeFilter->GetOutputPort() );
	
	mapper->SetInputConnection( stripFilter->GetOutputPort() );
	mapper->ScalarVisibilityOff();
	mapper->SetColorModeToDefault();

	outputActor->GetProperty()->SetAmbient(1.0);
	outputActor->GetProperty()->SetDiffuse(0.0);
}


void vtkImgFilter::setFilterInput(const vcl_string &filename)
{
	this->reader = this->readerFactory->CreateImageReader2( filename.c_str() );
	this->reader->SetFileName( filename.c_str() );
	this->luminanceFilter->SetInputConnection( this->reader->GetOutputPort() );
	this->luminanceFilter->Update();
}

vtkSmartPointer<vtkActor> vtkImgFilter::getOutputActor( void )
{
	this->mapper->Update();
	outputActor->SetMapper( mapper );
	return outputActor;
}

vtkSmartPointer<vtkImageActor> vtkImgFilter::getFilterOutput( void )
{
	return vtkSmartPointer<vtkImageActor>::New();
}

void vtkImgFilter::SetRadius( double x[3] )
{
	this->gaussianFilter->SetRadiusFactors( x );
}

void vtkImgFilter::SetStdDev( double x[3] ) 
{
	this->gaussianFilter->SetStandardDeviations( x );
}

void vtkImgFilter::SetThreshold( double x )
{
	this->threshFilter->ThresholdByUpper( x );
}