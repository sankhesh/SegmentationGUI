/**
  * \file: RenderBase.cxx
  * \author: Sankhesh Jhaveri
  * \brief: defines render base and render Image classes
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

void RenderImage::AddSegmentedActor( vtkSmartPointer<vtkActor> actor )
{
	this->ImageActor->GetProperty()->SetOpacity( 0.5 );
	this->SegmentedActor->SetMapper( actor->GetMapper() );
	this->SegmentedActor->GetProperty()->SetAmbient(1.0);
	this->SegmentedActor->GetProperty()->SetDiffuse(0.0);
}