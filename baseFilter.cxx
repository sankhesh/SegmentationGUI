#include "baseFilter.h"

void baseFilter::SetInput( const vcl_string &filename )
{
	this->fname = filename;
	this->setFilterInput( this->fname );
}

vtkSmartPointer<vtkImageActor> baseFilter::GetOutput( void )
{
	this->ImActor->SetInput( (this->getFilterOutput())->GetInput() );
	return this->ImActor;	
}