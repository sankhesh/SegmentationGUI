/**
  * \file: vxlImgFilter.cxx
  * \author: Sankhesh Jhaveri
  * \brief: defines vxlImgFilter class
**/
#include "vxlImgFilter.h"

#include <vil/vil_load.h>
#include <vil/algo/vil_gauss_filter.h>
#include <vil/algo/vil_sobel_3x3.h>
#include <vil/algo/vil_suppress_non_max_edges.h>
#include <vil/vil_math.h>

#include <itkImageRegionIterator.h>

//Constructor
vxlImgFilter::vxlImgFilter() :
	inputImage(),
	internalImage(),
	thresh1( 1.0f ),
	thresh2( 1.0f )
{
}

void vxlImgFilter::setFilterInput(const vcl_string &filename)
{
	this->inputImage = vil_load( filename.c_str(), false );
}

void vxlImgFilter::PerformFiltering( void )
{
	vil_image_view< UTILS::ScalarPixelType > smoothIm, grad_i, grad_j;

	vil_gauss_filter_5tap_params params( 1.0f );
	vil_gauss_filter_5tap( this->inputImage, smoothIm, params );
	
	vil_sobel_3x3( smoothIm, grad_i, grad_j);
	
	vil_suppress_non_max_edges(grad_i, grad_j, (double)this->thresh1, this->internalImage);
	
	UTILS::ScalarPixelType min, max;
	vil_math_value_range( this->internalImage, min, max);
	
	for (unsigned j = 0; j < this->internalImage.nj(); j++)
	{
		for( unsigned i = 0; i < this->internalImage.ni(); i++ )
		{
			this->internalImage(i,j) = (this->internalImage(i,j)-min)*255/(max-min);
			if(this->internalImage(i,j) >= this->thresh2 )
				this->internalImage(i,j) = 255;
			else
				this->internalImage(i,j) = 0;
		}
	}
}

UTILS::ScalarImageType::Pointer vxlImgFilter::ConvertVXLImgToITKImg( void )
{
	UTILS::ScalarImageType::Pointer image = UTILS::ScalarImageType::New();
	UTILS::ScalarImageType::SizeType size;
	size[0] = this->internalImage.ni();
	size[1] = this->internalImage.nj();

	UTILS::ScalarImageType::IndexType start;
	start[0] = 0;
	start[1] = 0;

	UTILS::ScalarImageType::RegionType region;
	region.SetSize( size );
	region.SetIndex( start );

	double spacing[2];
	spacing[0] = 1;
	spacing[1] = 1;

	double origin[2];
	origin[0] = 0;
	origin[1] = 0;

	image->SetRegions( region );
	image->Allocate();
	image->SetSpacing( spacing );
	image->SetOrigin( origin );

	typedef itk::ImageRegionIterator< UTILS::ScalarImageType > IteratorType;
	IteratorType It( image, region );
	It.GoToBegin();

	vil_image_view< UTILS::ScalarPixelType >::const_iterator internalImageIterator;
	internalImageIterator = this->internalImage.begin();

	while( !It.IsAtEnd() )
	{
		It.Set( *internalImageIterator );
		++It;
		++internalImageIterator;
	}
	return image;
}

vtkSmartPointer<vtkImageActor> vxlImgFilter::getFilterOutput( void )
{
	this->PerformFiltering();
	UTILS::ScalarImageType::Pointer image = this->ConvertVXLImgToITKImg();
	return UTILS::ITKToVTKConvert( UTILS::ComposeRGBImage( image ) );
}