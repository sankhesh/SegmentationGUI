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
   img_rows( 0 ),
   img_cols( 0 ),
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
	
	vcl_try
	{
		imageReader->Update();
	}
	vcl_catch( itk::ExceptionObject &err )
	{
	  vcl_cout << vcl_endl
			   << "***" << vcl_endl
			   << "Exception caught:" << vcl_endl
			   << err << vcl_endl
			   << "***" << vcl_endl
			   << vcl_flush;
	  return;
	}
   
	this->img_rows = imageReader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
	this->img_cols = imageReader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
	this->setSliders();

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
	this->UpdateVTKImage( true );

	if( !this->vxlFilter )
	{
		this->vxlFilter = new vxlImgFilter;
	}
	this->vxlFilter->SetInput( fname );
	this->qvtk_vxl.setBase( vxlFilter );
	this->slotVXLNonMaxThresh( this->horizontalSlider_nonMaxthresh->value() );
	this->slotVXLMagThresh( this->horizontalSlider_Magthresh->value() );
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
	this->resetSliders();
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
	if( !this->itkFilter )
		return;
	this->itkFilter->SetThreshold( (float) value );
	UpdateITKImage();
}

void QtImgSegment::slotITKvar( int value )
{
	if( !this->itkFilter )
		return;
	this->itkFilter->SetVariance( (float) value*0.05 );
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
	if( !this->vtkFilter )
		return;
	double x[3];
	x[2] = 0;
	x[1] = (double) ((this->horizontalSlider_radiusy->value())*0.02);
	x[0] = (double) ((this->horizontalSlider_radiusx->value())*0.02);
	this->vtkFilter->SetRadius( x );
	this->UpdateVTKImage();
}

void QtImgSegment::slotVTKstddev( int value )
{
	if( !this->vtkFilter )
		return;
	double x[3];
	x[2] = 0;
	x[1] = (double) ((this->horizontalSlider_stdy->value())*0.01);
	x[0] = (double) ((this->horizontalSlider_stdx->value())*0.01);
	this->vtkFilter->SetStdDev( x );
	this->UpdateVTKImage();
}

void QtImgSegment::slotVTKthresh( int value )
{
	if( !this->vtkFilter )
		return;
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
	if( !this->vxlFilter )
		return;
	this->vxlFilter->SetMagnitudeThreshold( (float) value );
	this->UpdateVXLImage();
}

void QtImgSegment::slotVXLNonMaxThresh(int value)
{
	if( !this->vtkFilter )
		return;
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

void QtImgSegment::setSliders( void )
{
	unsigned val = (this->img_rows < this->img_cols) ? this->img_rows : this->img_cols;
	this->horizontalSlider_radiusx->setMaximum( val );
	this->horizontalSlider_radiusy->setMaximum( val );
	this->horizontalSlider_stdx->setMaximum( val );
	this->horizontalSlider_stdy->setMaximum( val );
	this->horizontalSlider_variance->setMaximum( val );
}

void QtImgSegment::resetSliders( void )
{
	this->horizontalSlider_radiusx->setValue( 1 );
	this->horizontalSlider_radiusy->setValue( 1 );
	this->horizontalSlider_stdx->setValue( 1 );
	this->horizontalSlider_stdy->setValue( 1 );
	this->horizontalSlider_variance->setValue( 1 );
	this->horizontalSlider_Magthresh->setValue( 1 );
	this->horizontalSlider_nonMaxthresh->setValue( 1 );
	this->horizontalSlider_thresh->setValue( 1 );
	this->horizontalSlider_vtkthresh->setValue( 1 );
}