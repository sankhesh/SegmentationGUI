#include <QtImgSegment.h>


#include <qfiledialog.h>
#include <vcl_cstdio.h>
#include <vcl_string.h>
#include <qstringlist.h>

#include <itkImageFileReader.h>
#include <itkImageToVTKImageFilter.h>
#include <itkImageFileWriter.h>

#include <vtkRenderWindow.h>
#include <vtkImageFlip.h>
#include <vtkInteractorStyleImage.h>

typedef itk::ImageFileReader<IMPROC::baseFilter::RGBImageType> ReaderType;

//Constructor
QtImgSegment::QtImgSegment() :
   workingDir( "." ),
   Connections( vtkEventQtSlotConnect::New() ),
   ImageActor( vtkSmartPointer<vtkImageActor>::New() ),
   Ren( vtkSmartPointer<vtkRenderer>::New() ),
   CurrentImg( IMPROC::baseFilter::RGBImageType::New() )
{
	this->setupUi(this);
	this->Ren->AddActor(this->ImageActor);
	this->qVTKImg->GetRenderWindow()->AddRenderer( this->Ren );

	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	this->qVTKImg->GetInteractor()->SetInteractorStyle(style);
	
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
   
   ReaderType::Pointer imageReader = ReaderType::New();
   imageReader->SetFileName( fname );
   try
	{
		imageReader->Update();
		this->CurrentImg = imageReader->GetOutput();
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

   this->displayImage(true);
   this->actionLoad_Image->setDisabled( true );
   this->actionClose_Image->setEnabled( true );
   this->actionSave_Image->setEnabled( true );
}

void QtImgSegment::closeEvent(QCloseEvent * Event)
{
   qApp->exit();
}

void QtImgSegment::displayImage(bool reset_camera)
{
	typedef itk::ImageToVTKImageFilter<IMPROC::baseFilter::RGBImageType> ImageToVTKImageFilterType;
	ImageToVTKImageFilterType::Pointer itkTovtkFilter = ImageToVTKImageFilterType::New();

	itkTovtkFilter->SetInput(this->CurrentImg);
	
	vtkSmartPointer<vtkImageFlip> flipY = vtkSmartPointer<vtkImageFlip>::New();
	flipY->SetInput( itkTovtkFilter->GetOutput() );
	flipY->SetFilteredAxis(1);
	flipY->Update();

	this->ImageActor->SetInput(flipY->GetOutput());
	if (reset_camera)
	{
		this->Ren->ResetCamera();
	}
	this->ImageActor->SetVisibility( 1 );
	this->qVTKImg->GetRenderWindow()->Render();
}

void QtImgSegment::slotCloseImage( void )
{
	this->ImageActor->SetVisibility( 0 );
	this->qVTKImg->GetRenderWindow()->Render();
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

	typedef itk::ImageFileWriter<IMPROC::baseFilter::RGBImageType> writerType;
	writerType::Pointer writer = writerType::New();
	writer->SetInput( this->SegmentedImg );
	//writer->SetInput( this->CurrentImg );
	writer->SetFileName( fname );
	writer->Update();
}
