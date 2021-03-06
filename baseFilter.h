/**
  * \file: baseFilter.cxx
  * \author: Sankhesh Jhaveri
  * \brief: declares baseFilter class. Abstract base class that defines input/output methods
  *			for filtration operations.
**/

#ifndef _BASEFILTER_H_
#define _BASEFILTER_H_

#include "utils.h"
#include <vcl_string.h>
#include <vcl_exception.h>

class baseFilter
{
	vcl_string fname;
	vtkSmartPointer<vtkImageActor> ImActor;
	virtual void setFilterInput( const vcl_string &filename ) = 0;
	virtual vtkSmartPointer<vtkImageActor> getFilterOutput( void ) = 0;
public:
	baseFilter() :
		fname(""),
		ImActor( vtkSmartPointer<vtkImageActor>::New() )
	{
	}
	virtual ~baseFilter(){}
	
	void SetInput( const vcl_string &filename );
	vtkSmartPointer<vtkImageActor> GetOutput( void );


};
#endif //_BASEFILTER_H_