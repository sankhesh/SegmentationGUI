/**
  * \file: RenderBase.cxx
  * \author: Sankhesh Jhaveri
  * \brief: defines render base class
**/

#include "RenderBase.h"

RenderBase::RenderBase() :
	Ren(vtkSmartPointer<vtkRenderer>::New()),
	RenWin( NULL )
{
}

RenderImage::RenderImage() : 
	style(vtkSmartPointer<vtkInteractorStyleImage>::New()),
	ImageActor( vtkSmartPointer<vtkImageActor>::New() )
{
	this->Ren->AddActor( this->ImageActor );
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