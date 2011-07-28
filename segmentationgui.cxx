/*
 * \file segmentationgui.cxx
 * \author Sankhesh Jhaveri
 * \brief Defines main for the gui
 */
#include "QtImgSegment.h"
#include "utils.h"

#include <qapplication.h>

int main(int argc, char *argv[])
{
   QApplication app(argc,argv);

   UTILS::redirectVTKWarnings( "VTKMESSAGES.txt" );

   QtImgSegment myQtImgSegment;
   myQtImgSegment.show();

   return app.exec();
}

