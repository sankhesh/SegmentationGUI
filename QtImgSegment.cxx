#include "QtImgSegment.h"

#include <qfiledialog.h>
#include <vcl_cstdio.h>
#include <qstringlist.h>

//#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <vtkCamera.h>

//Constructor
QtImgSegment::QtImgSegment() :
   workingDir( "." ),
   imageReader(UTILS::ReaderType::New()),
   //fname(""),
   Connections( vtkEventQtSlotConnect::New() )
   //ImageActor( NULL ),
   //Ren( vtkSmartPointer<vtkRenderer>::New() ),
   //CurrentImg( NULL ),
   //SegmentedImg( NULL )
{
	this->setupUi(this);

	vtkSmartPointer<vtkCamera> cam = this->qvtk_original.Ren->GetActiveCamera();
	this->qvtk_itk.Ren->SetActiveCamera( cam );
	this->qvtk_vtk.Ren->SetActiveCamera( cam );
	this->qvtk_vxl.Ren->SetActiveCamera( cam );

	this->qvtk_original.AddRenderWindow( this->qvtkwidget_originalimage->GetRenderWindow() );
	this->qvtk_itk.AddRenderWindow( this->qvtkwidget_segmentedITK->GetRenderWindow() );
	this->qvtk_vtk.AddRenderWindow( this->qvtkwidget_segmentedVTK->GetRenderWindow() );
	this->qvtk_vxl.AddRenderWindow( this->qvtkwidget_segmentedVXL->GetRenderWindow() );
	
	this->renderAll(0,true);

	connect(this->actionExit,SIGNAL(triggered()),qApp,SLOT(closeAllWindows()));
	connect(this->actionLoad_Image,SIGNAL(triggered()),this,SLOT(slotLoadImage()));
	connect(this->actionSet_Working_Directory,SIGNAL(triggered()),this,SLOT(slotSetWorkingDirectory()));
	connect(this->actionClose_Image,SIGNAL(triggered()),this,SLOT(slotCloseImage()));
	connect(this->actionSave_Image,SIGNAL(triggered()),this,SLOT(slotSaveImage()));
   // connect(this->actionInstructions,SIGNAL(triggered()),this,SLOT(showInstructions()));	   
}

//Destructor
QtImgSegment::~QtImgSegment()
{
   Connections->Delete();
}

void QtImgSegment::slotSetWorkingDirectory()
{
   this->workingDir = QFileDialog::getExistingDirectory( this, "Set Working Directory", this->workingDir, QFileDialog::ShowDirsOnly );
   vcl_cout << "Working directory set to " << workingDir.toAscii().data() << vcl_endl << vcl_flush;
}

// void QtImgSegment::showInstructions()
// {
   // instructions *showi = new instructions;
   // showi->show();
// }

void QtImgSegment::slotLoadImage( void )
{
   //File open dialog
   QString filename;
   filename = QFileDialog::getOpenFileName( this, "Open Image", this->workingDir, "Image Files (*.bmp *.jpg *.png *.hdr *.img)" );
   if (filename.isNull())
      return;

   vcl_string fname( filename.toAscii().data() );
   vcl_cout << vcl_endl << "Loading Image file: " << fname << vcl_endl;
   
   imageReader->SetFileName( fname );
   try
	{
		imageReader->Update();
		//this->CurrentImg = imageReader->GetOutput();
	}
   catch( itk::ExceptionObject &err )
   {
      vcl_cout << vcl_endl
               << "***" << vcl_endl
               << "Exception caught:" << vcl_endl
               << err << vcl_endl
               << "***" << vcl_endl
               << vcl_flush;
      return;
   }

	this->qvtk_original.AddImageActor( UTILS::ITKToVTKConvert( imageReader->GetOutput() ));
	this->qvtk_itk.AddImageActor( UTILS::ITKToVTKConvert( imageReader->GetOutput() ));
	this->qvtk_vtk.AddImageActor( UTILS::ITKToVTKConvert( imageReader->GetOutput() ));
	this->qvtk_vxl.AddImageActor( UTILS::ITKToVTKConvert( imageReader->GetOutput() ));

	this->renderAll( 1, true );

	this->actionLoad_Image->setDisabled( true );
	this->actionClose_Image->setEnabled( true );
	this->actionSave_Image->setEnabled( true );
}

void QtImgSegment::closeEvent(QCloseEvent * Event)
{
   qApp->exit();
}

void QtImgSegment::slotCloseImage( void )
{
	this->renderAll( 0, false );
	this->actionLoad_Image->setEnabled( true );
	this->actionClose_Image->setDisabled( true );
	this->actionSave_Image->setDisabled( true );
}

void QtImgSegment::slotSaveImage( void )
{
	QString filename;
	filename = QFileDialog::getSaveFileName( this, "Save Segmented Image", this->workingDir, "Image Files (*.bmp *.jpg *.png *.hdr *.img)" );
	if (filename.isNull())
      return;
	vcl_string fname( filename.toAscii().data() );

	//typedef itk::ImageFileWriter<IMPROC::baseFilter::RGBImageType> writerType;
	//writerType::Pointer writer = writerType::New();
	//writer->SetInput( this->SegmentedImg );
	////writer->SetInput( this->CurrentImg );
	//writer->SetFileName( fname );
	//writer->Update();
}

void QtImgSegment::renderAll( int visibility, bool reset_camera )
{
	this->qvtk_original.ImageActor->SetVisibility( visibility );
	this->qvtk_itk.ImageActor->SetVisibility( visibility );
	this->qvtk_vtk.ImageActor->SetVisibility( visibility );
	this->qvtk_vxl.ImageActor->SetVisibility( visibility );

	if (reset_camera)
	{
		this->qvtk_original.Ren->ResetCamera();
		this->qvtk_itk.Ren->ResetCamera();
		this->qvtk_vtk.Ren->ResetCamera();
		this->qvtk_vxl.Ren->ResetCamera();
	}

	this->qvtk_original.RenWin->Render();
	this->qvtk_itk.RenWin->Render();
	this->qvtk_vtk.RenWin->Render();
	this->qvtk_vxl.RenWin->Render();
}
