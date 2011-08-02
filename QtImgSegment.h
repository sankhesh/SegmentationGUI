#ifndef __QTIMGSEGMENT_H__
#define __QTIMGSEGMENT_H__

#include "ui_QtImgSegment.h"
#include "RenderBase.h"
#include "utils.h"

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
	//IMPROC::baseFilter::RGBImageType::ConstPointer CurrentImg;
	//IMPROC::baseFilter::RGBImageType::ConstPointer SegmentedImg;
	void renderAll( int visibility = 1, bool reset_camera = false );
	
public slots:

	void slotLoadImage( void );
	void slotSetWorkingDirectory( void );
	// void showInstructions();
	void closeEvent(QCloseEvent* Event);
	void slotCloseImage( void );
	void slotSaveImage( void );
	
protected:

	vtkEventQtSlotConnect* Connections;
	//vtkSmartPointer<vtkRenderer> Ren;
	RenderImage qvtk_original;
	RenderImage qvtk_itk;
	RenderImage qvtk_vtk;
	RenderImage qvtk_vxl;
	UTILS::ReaderType::Pointer imageReader;
};

#endif // __QTIMGSEGMENT_H__

