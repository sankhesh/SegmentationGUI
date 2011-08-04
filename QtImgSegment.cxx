/**
  * \file: QtImgSegment.cxx
  * \author: Sankhesh Jhaveri
  * \brief: defines QtImgSegment class
**/

#include "QtImgSegment.h"

#include <qfiledialog.h>
#include <vcl_cstdio.h>
#include <qstringlist.h>

#include <vtkCamera.h>

//Constructor
QtImgSegment::QtImgSegment() :
   workingDir( "." ),
   Connections( vtkEventQtSlotConnect::New() ),
   itkFilter( NULL ),
   vtkFilter( NULL ),
   vxlFilter( NULL )
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
	connect(this->horizontalSlider_thresh,SIGNAL(valueChanged(int)),this,SLOT(slotITKthresh(int)));
	connect(this->horizontalSlider_variance,SIGNAL(valueChanged(int)),this,SLOT(slotITKvar(int)));
	connect(this->horizontalSlider_radiusx,SIGNAL(valueChanged(int)),this,SLOT(slotVTKradius(int)));
	connect(this->horizontalSlider_radiusy,SIGNAL(valueChanged(int)),this,SLOT(slotVTKradius(int)));
	connect(this->horizontalSlider_stdx,SIGNAL(valueChanged(int)),this,SLOT(slotVTKstddev(int)));
	connect(this->horizontalSlider_stdy,SIGNAL(valueChanged(int)),this,SLOT(slotVTKstddev(int)));
	connect(this->horizontalSlider_vtkthresh,SIGNAL(valueChanged(int)),this,SLOT(slotVTKthresh(int)));
	connect(this->horizontalSlider_nonMaxthresh,SIGNAL(valueChanged(int)),this,SLOT(slotVXLNonMaxThresh(int)));
	connect(this->horizontalSlider_Magthresh,SIGNAL(valueChanged(int)),this,SLOT(slotVXLMagThresh(int)));
   // connect(this->actionInstructions,SIGNAL(triggered()),this,SLOT(showInstructions()));	   
}

//Destructor
QtImgSegment::~QtImgSegment()
{
   Connections->Delete();
   if( this->itkFilter )
   {
	   delete this->itkFilter;
   }
   if( this->vtkFilter )
   {
	   delete this->vtkFilter;
   }
   if( this->vxlFilter )
   {
	   delete this->vxlFilter;
   }
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
   filename = QFileDialog::getOpenFileName( this, "Open Image", this->workingDir, "Image Files (*.bmp *.jpg *.png *.hdr *.img )" );
   if (filename.isNull())
      return;

   vcl_string fname( filename.toAscii().data() );
   vcl_cout << vcl_endl << "Loading Image file: " << fname << vcl_endl;
   UTILS::ReaderType::Pointer imageReader = UTILS::ReaderType::New();
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

	if ( !this->itkFilter )
	{
		this->itkFilter = new itkImgFilter;
	}
	this->qvtk_itk.setBase( itkFilter );
	this->itkFilter->SetInput( fname );
	this->slotITKthresh( this->horizontalSlider_thresh->value() );
	this->slotITKvar( this->horizontalSlider_variance->value() );
	//this->qvtk_itk.AddSegmentedImageActor( this->itkFilter->GetOutput() );
	this->UpdateITKImage( true );

	if ( !this->vtkFilter )
	{
		this->vtkFilter = new vtkImgFilter;
	}
	this->qvtk_vtk.setBase( vtkFilter );
	this->vtkFilter->SetInput( fname );
	this->slotVTKradius( 0 );
	this->slotVTKstddev( 0 );
	this->slotVTKthresh( this->horizontalSlider_vtkthresh->value() );
	//this->qvtk_vtk.AddSegmentedActor( this->vtkFilter->getOutputActor() );
	this->UpdateVTKImage( true );

	if( !this->vxlFilter )
	{
		this->vxlFilter = new vxlImgFilter;
	}
	this->vxlFilter->SetInput( fname );
	this->qvtk_vxl.setBase( vxlFilter );
	this->slotVXLNonMaxThresh( this->horizontalSlider_nonMaxthresh->value() );
	this->slotVXLMagThresh( this->horizontalSlider_Magthresh->value() );
	//this->qvtk_vxl.AddSegmentedImageActor( this->vxlFilter->GetOutput() );
	this->UpdateVXLImage( true );

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
	if ( this->itkFilter )
	{
		this->itkFilter = NULL;
	}
	if ( this->vtkFilter )
	{
		this->vtkFilter = NULL;
	}
	if ( this->vxlFilter )
	{
		this->vxlFilter = NULL;
	}
	this->actionLoad_Image->setEnabled( true );
	this->actionClose_Image->setDisabled( true );
	this->actionSave_Image->setDisabled( true );
}

void QtImgSegment::renderAll( int visibility, bool reset_camera )
{
	this->qvtk_original.ImageActor->SetVisibility( visibility );
	this->qvtk_itk.ImageActor->SetVisibility( visibility );
	this->qvtk_vtk.ImageActor->SetVisibility( visibility );
	this->qvtk_vxl.ImageActor->SetVisibility( visibility );

	this->qvtk_itk.SegmentedImageActor->SetVisibility( visibility );
	this->qvtk_vtk.SegmentedActor->SetVisibility( visibility );
	this->qvtk_vxl.SegmentedImageActor->SetVisibility( visibility );
	

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

void QtImgSegment::slotITKthresh( int value )
{
	this->itkFilter->SetThreshold( (float) value );
	UpdateITKImage();
}

void QtImgSegment::slotITKvar( int value )
{
	this->itkFilter->SetVariance( (float) value*10.0/100 );
	UpdateITKImage();
}

void QtImgSegment::UpdateITKImage( const bool reset_camera )
{
	this->qvtk_itk.AddSegmentedImageActor( this->itkFilter->GetOutput() );
	if (reset_camera)
	{
		this->qvtk_itk.Ren->ResetCamera();
	}
	this->qvtk_itk.RenWin->Render();
}

void QtImgSegment::slotVTKradius( int value )
{
	double x[3];
	x[2] = 0;
	x[1] = (double) (this->horizontalSlider_radiusy->value());
	x[0] = (double) (this->horizontalSlider_radiusx->value());
	this->vtkFilter->SetRadius( x );
	this->UpdateVTKImage();
}

void QtImgSegment::slotVTKstddev( int value )
{
	double x[3];
	x[2] = 0;
	x[1] = (double) (this->horizontalSlider_stdy->value());
	x[0] = (double) (this->horizontalSlider_stdx->value());
	this->vtkFilter->SetStdDev( x );
	this->UpdateVTKImage();
}

void QtImgSegment::slotVTKthresh( int value )
{
	this->vtkFilter->SetThreshold( (double) value );
	this->UpdateVTKImage();
}

void QtImgSegment::UpdateVTKImage( const bool reset_camera )
{
	this->qvtk_vtk.AddSegmentedActor( this->vtkFilter->getOutputActor() );
	if (reset_camera)
	{
		this->qvtk_vtk.Ren->ResetCamera();
	}
	this->qvtk_vtk.RenWin->Render();
}

void QtImgSegment::slotVXLMagThresh(int value)
{
	this->vxlFilter->SetMagnitudeThreshold( (float) value );
	this->UpdateVXLImage();
}

void QtImgSegment::slotVXLNonMaxThresh(int value)
{
	this->vxlFilter->SetNonMaxSuppressionThreshold((float) value );
	this->UpdateVXLImage();
}

void QtImgSegment::UpdateVXLImage( const bool reset_camera )
{
	this->qvtk_vxl.AddSegmentedImageActor( this->vxlFilter->GetOutput() );
	if (reset_camera)
	{
		this->qvtk_vxl.Ren->ResetCamera();
	}
	this->qvtk_vxl.RenWin->Render();
}