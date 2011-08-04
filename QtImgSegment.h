/**
  * \file: QtImgSegment.h
  * \author: Sankhesh Jhaveri
  * \brief: declares QtImgSegment class. This is the main GUI class that handles all connections.
**/

#ifndef __QTIMGSEGMENT_H__
#define __QTIMGSEGMENT_H__

#include "ui_QtImgSegment.h"
#include "RenderBase.h"
#include "utils.h"
#include "itkImgFilter.h"
#include "vtkImgFilter.h"
#include "vxlImgFilter.h"

#include <vtkEventQtSlotConnect.h>
#include <vtkImageActor.h>

#include <vcl_string.h>

class vtkEventQtSlotConnect;
class vtkObject;
class vtkCommand;

class QtImgSegment : public QMainWindow, public Ui::MainWindow_SegmentationGUI
{
	Q_OBJECT

public:

	QtImgSegment();
	~QtImgSegment();
	QString workingDir;
	//vtkSmartPointer<vtkImageActor> ImageActor;
	//void displayImage( bool reset_camera = false );
	//vcl_string fname;
	//IMPROC::baseFilter::RGBImageType::Pointer CurrentImg;
	//IMPROC::baseFilter::RGBImageType::Pointer SegmentedImg;
	void renderAll( int visibility = 1, bool reset_camera = false );

	itkImgFilter *itkFilter;
	vtkImgFilter *vtkFilter;
	vxlImgFilter *vxlFilter;
	void UpdateITKImage( const bool reset_camera = false );
	void UpdateVTKImage( const bool reset_camera = false );
	void UpdateVXLImage( const bool reset_camera = false );
	
public slots:

	void slotLoadImage( void );
	void slotSetWorkingDirectory( void );
	// void showInstructions();
	void closeEvent(QCloseEvent* Event);
	void slotCloseImage( void );
	void slotITKthresh( int value );
	void slotITKvar( int value );
	void slotVTKradius( int value );
	void slotVTKstddev( int value );
	void slotVTKthresh( int value );
	void slotVXLNonMaxThresh( int value );
	void slotVXLMagThresh( int value );

protected:

	vtkEventQtSlotConnect* Connections;
	//vtkSmartPointer<vtkRenderer> Ren;
	RenderImage qvtk_original;
	RenderImage qvtk_itk;
	RenderImage qvtk_vtk;
	RenderImage qvtk_vxl;
};

#endif // __QTIMGSEGMENT_H__

