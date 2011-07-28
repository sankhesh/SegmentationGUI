#ifndef __QTIMGSEGMENT_H__
#define __QTIMGSEGMENT_H__

#include <ui_QtImgSegment.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>

#include <ImageProc/baseFilter.h>

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
	vtkSmartPointer<vtkImageActor> ImageActor;
	void displayImage( bool reset_camera = false );
	IMPROC::baseFilter::RGBImageType::ConstPointer CurrentImg;
	IMPROC::baseFilter::RGBImageType::ConstPointer SegmentedImg;

public slots:

	void slotLoadImage( void );
	void slotSetWorkingDirectory( void );
	// void showInstructions();
	void closeEvent(QCloseEvent* Event);
	void slotCloseImage( void );
	void slotSaveImage( void );
	
protected:

	vtkEventQtSlotConnect* Connections;
	vtkSmartPointer<vtkRenderer> Ren;
};

#endif // __QTIMGSEGMENT_H__

