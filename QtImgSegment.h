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
#include <vcl_exception.h>

class vtkEventQtSlotConnect;
class vtkObject;
class vtkCommand;

class QtImgSegment : public QMainWindow, public Ui::MainWindow_SegmentationGUI
{
	Q_OBJECT

		unsigned img_rows, img_cols;
public:

	QtImgSegment();
	~QtImgSegment();
	QString workingDir;

	void renderAll( int visibility = 1, bool reset_camera = false );
	void setSliders( void );
	void resetSliders( void );
	
	itkImgFilter *itkFilter;
	vtkImgFilter *vtkFilter;
	vxlImgFilter *vxlFilter;
	void UpdateITKImage( const bool reset_camera = false );
	void UpdateVTKImage( const bool reset_camera = false );
	void UpdateVXLImage( const bool reset_camera = false );
	
public slots:

	void slotLoadImage( void );
	void slotSetWorkingDirectory( void );
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
	RenderImage qvtk_original;
	RenderImage qvtk_itk;
	RenderImage qvtk_vtk;
	RenderImage qvtk_vxl;
};

#endif // __QTIMGSEGMENT_H__

