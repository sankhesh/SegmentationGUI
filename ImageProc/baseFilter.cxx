/**
  * \file: baseFilter.cxx
  * \author: Sankhesh Jhaveri
  * \brief: defines baseFilter class methods
**/

#include <ImageProc/baseFilter.h>

namespace IMPROC {

void baseScalarFilter::setFilterInput( const RGBImageType::ConstPointer &img )
{
	this->luminancefilter->SetInput( img );
	this->scalarinput = this->luminancefilter->GetOutput();
	this->setScalarFilterInput( this->scalarinput );
}

baseScalarFilter::RGBImageType::ConstPointer baseScalarFilter::getFilterOutput( void )
{
   this->scalaroutput = this->getScalarFilterOutput();
   this->composefilter->SetInput1( this->scalaroutput );
   this->composefilter->SetInput2( this->scalaroutput );
   this->composefilter->SetInput3( this->scalaroutput );
   return this->composefilter->GetOutput();
}

baseScalarRescaledFilter::RGBImageType::ConstPointer baseScalarRescaledFilter::getFilterOutput( void )
{
   this->rescalefilter->SetInput( this->getScalarFilterOutput() );
   this->rescalefilter->SetOutputMinimum(0);
   this->rescalefilter->SetOutputMaximum(255);
   this->scalaroutput = this->rescalefilter->GetOutput();
   this->composefilter->SetInput1( this->scalaroutput );
   this->composefilter->SetInput2( this->scalaroutput );
   this->composefilter->SetInput3( this->scalaroutput );
   return this->composefilter->GetOutput();
}

}

