/**
  * \file: RenderBase.h
  * \author: Sankhesh Jhaveri
  * \brief: declares render base class
**/

#ifndef __RENDERBASE_H__
#define __RENDERBASE_H__

#include <vcl_iostream.h>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageActor.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>

#include "baseFilter.h"

class RenderBase
{
public:
	RenderBase();
	~RenderBase(){}
	vtkSmartPointer<vtkRenderer> Ren;
	vtkSmartPointer<vtkRenderWindow> RenWin;
	
	virtual void AddRenderWindow( vtkSmartPointer<vtkRenderWindow> ) = 0;
}; //class RenderBase

class RenderImage : public RenderBase
{
	vtkSmartPointer<vtkInteractorStyleImage> style;
public:
	RenderImage();
	~RenderImage(){}
	vtkSmartPointer<vtkImageActor> ImageActor;
	vtkSmartPointer<vtkImageActor> SegmentedImageActor;
	vtkSmartPointer<vtkActor> SegmentedActor;

	virtual void AddRenderWindow( vtkSmartPointer<vtkRenderWindow> );
	void AddImageActor( vtkSmartPointer<vtkImageActor> actor );
	void AddSegmentedImageActor( vtkSmartPointer<vtkImageActor> actor );
	void WriteSegmentedImage( const char * name);
	void AddSegmentedActor( vtkSmartPointer<vtkActor> actor );
	
	baseFilter *base;
	void setBase( baseFilter *baseF );
	
}; //class RenderImage

#endif // __RENDERBASE_H__