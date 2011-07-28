#ifndef _UTILS_H_
#define _UTILS_H_

#include <vcl_string.h>
#include <vtkFileOutputWindow.h>

namespace UTILS {

void redirectVTKWarnings( const vcl_string &fname )
{
   vtkFileOutputWindow *outwin = vtkFileOutputWindow::New();
   outwin->SetFileName( fname.c_str() );
   outwin->SetGlobalWarningDisplay( 0 );
   outwin->SetInstance( outwin );
}

} //namespace UTILS

#endif //_UTILS_H_