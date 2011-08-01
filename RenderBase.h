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
#include <vtkRenderWindowInteractor.h>

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
public:
	RenderImage();
	~RenderImage(){}
	vtkSmartPointer<vtkInteractorStyleImage> style;
	vtkSmartPointer<vtkImageActor> ImageActor;

	virtual void AddRenderWindow( vtkSmartPointer<vtkRenderWindow> );
	void AddImageActor( vtkSmartPointer<vtkImageActor> actor );

}; //class RenderImage

#endif // __RENDERBASE_H__