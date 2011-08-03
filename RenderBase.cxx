/**
  * \file: RenderBase.cxx
  * \author: Sankhesh Jhaveri
  * \brief: defines render base class
**/

#include "RenderBase.h"
#include <vtkProperty.h>
#include <vtkImageProperty.h>
#include <vtkImageWriter.h>

RenderBase::RenderBase() :
	Ren(vtkSmartPointer<vtkRenderer>::New()),
	RenWin( NULL )
{
}

RenderImage::RenderImage() : 
	style(vtkSmartPointer<vtkInteractorStyleImage>::New()),
	ImageActor( vtkSmartPointer<vtkImageActor>::New() ),
	SegmentedImageActor( vtkSmartPointer<vtkImageActor>::New() ),
	SegmentedActor( vtkSmartPointer<vtkActor>::New() ),
	base( NULL )
{
	this->Ren->AddActor( this->ImageActor );
	this->Ren->AddActor( this->SegmentedImageActor );
	this->Ren->AddActor( this->SegmentedActor );
}

void RenderImage::AddRenderWindow( vtkSmartPointer<vtkRenderWindow> renwin)
{
	this->RenWin = renwin;
	this->RenWin->AddRenderer(this->Ren);
	this->RenWin->GetInteractor()->SetInteractorStyle( this->style );
}

void RenderImage::AddImageActor( vtkSmartPointer<vtkImageActor> actor )
{
	this->ImageActor->SetInput( actor->GetInput() );
}

void RenderImage::AddSegmentedImageActor( vtkSmartPointer<vtkImageActor> actor )
{
	this->ImageActor->GetProperty()->SetOpacity( 0.5 );
	this->SegmentedImageActor->SetInput( actor->GetInput() );
}

void RenderImage::setBase( baseFilter *baseF )
{
	this->base = baseF;
}

void RenderImage::WriteSegmentedImage( const char *name )
{
	vtkSmartPointer<vtkImageWriter> writer = vtkSmartPointer<vtkImageWriter>::New();
	writer->SetFileName( name );
	writer->SetInput( this->SegmentedImageActor->GetInput() );
	writer->Update();
}

void RenderImage::AddSegmentedActor( vtkSmartPointer<vtkActor> actor )
{
	this->ImageActor->GetProperty()->SetOpacity( 0.5 );
	this->SegmentedActor->SetMapper( actor->GetMapper() );
}