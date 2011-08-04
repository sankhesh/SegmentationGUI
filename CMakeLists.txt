PROJECT( SegmentationGUI )

cmake_minimum_required(VERSION 2.8)


FIND_PACKAGE(VXL REQUIRED)
INCLUDE(${VXL_CMAKE_DIR}/UseVXL.cmake)
MESSAGE(STATUS "Including <${VXL_CMAKE_DIR}/UseVXL.cmake>" )
INCLUDE_DIRECTORIES(
  ${VXLCORE_INCLUDE_DIR}
  ${GEL_INCLUDE_DIR}
)
# See VXL/config/cmake/Modules/UseVXL.cmake for all the params
INCLUDE_DIRECTORIES( ${VXL_GEL_INCLUDE_DIR}/mrc )
MESSAGE(STATUS "Library path for VXL <${VXL_LIBRARY_DIR}>" )

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})
MESSAGE(STATUS "Using <${ITK_USE_FILE}>" )
IF( ${ITK_USE_SYSTEM_VXL} )
   MESSAGE(STATUS "Using VXL from <${VXL_DIR}> for ITK" )
ELSE( ${ITK_USE_SYSTEM_VXL} )
   MESSAGE( "Please ensure that ITK_USE_SYSTEM_VXL is true in ITK build" )
ENDIF( ${ITK_USE_SYSTEM_VXL} )


FIND_PATH( ITK_SOURCE_DIR "CMakeLists.txt" )
SET ( ITKVTKGLUE_PATH "${ITK_SOURCE_DIR}/Wrapping/WrapITK/ExternalProjects/ItkVtkGlue/src" )
INCLUDE_DIRECTORIES(${ITKVTKGLUE_PATH})
MESSAGE(STATUS "Including <${ITKVTKGLUE_PATH}> as path for ItkVtkGlue" )

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
MESSAGE(STATUS "Using <${QT_USE_FILE}>" )

SET( QTCORETARGET
  ${CMAKE_SHARED_LIBRARY_PREFIX}QtCore4${CMAKE_SHARED_LIBRARY_SUFFIX}
)
FIND_PATH( QT_LIBRARY_DIRS QTCORETARGET )
MESSAGE(STATUS "Using <${QT_LIBRARY_DIRS}> as path for <${QTCORETARGET}>" )


FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
MESSAGE(STATUS "Using <${VTK_USE_FILE}>" )

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)
MESSAGE(STATUS "Including <${CMAKE_CURRENT_BINARY_DIR}>" )
MESSAGE(STATUS "Including <${CMAKE_CURRENT_SOURCE_DIR}>" )


SET( Sources
		# cxx files
		QtImgSegment.cxx
		RenderBase.cxx
		utils.cxx
		baseFilter.cxx
		itkImgFilter.cxx
		vtkImgFilter.cxx
		vxlImgFilter.cxx
)

SET( MOCInputs
		# h files
		QtImgSegment.h
		RenderBase.h
		utils.h
		baseFilter.h
		itkImgFilter.h
		vtkImgFilter.h
		vxlImgFilter.h
)

SET( UIInputs
		QtImgSegment.ui
)

SET( ResourceFiles
		ApplicationResources.qrc
)

QT4_WRAP_UI( UISources ${UIInputs} )
QT4_WRAP_CPP( MOCSources ${MOCInputs} )
QT4_ADD_RESOURCES( Resources ${ResourceFiles} )

SOURCE_GROUP( "UI" FILES ${UIInputs} )
SOURCE_GROUP( "Generated" FILES ${MOCSources} ${UISources} ${Resources} )
SOURCE_GROUP( "QtResource" FILES ${ResourceFiles} )

ADD_LIBRARY( gui 
  ${Sources}
  ${MOCInputs}
  ${MOCSources}
  ${UISources}
)

TARGET_LINK_LIBRARIES( gui
					vnl vnl_algo vcl vil vil_algo
					QVTK vtkHybrid
					${QT_LIBRARIES}
					${ITK_LIBRARIES}
)

ADD_EXECUTABLE( segmentationgui segmentationgui.cxx ${Resources})
TARGET_LINK_LIBRARIES( segmentationgui gui )

SET( APPNAME segmentationgui )
SET( PLUGIN_DEST_DIR bin )
SET( QTCONF_DEST_DIR bin )
SET( APP_INSTALL_DIR bin )
IF(APPLE)
  SET( APPNAME segmentationgui.app )
  SET( PLUGIN_DEST_DIR ${APPNAME}/Contents/MacOS )
  SET( QTCONF_DEST_DIR ${APPNAME}/Contents/Resources )
  SET( APP_INSTALL_DIR "." )
ENDIF(APPLE)
IF(WIN32)
  SET( APPNAME segmentationgui.exe )
ENDIF(WIN32)
SET( APPS "\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APPNAME}" )

INSTALL( TARGETS gui segmentationgui
   BUNDLE  DESTINATION .
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)

INSTALL( DIRECTORY
   "${QT_PLUGINS_DIR}/imageformats"
   DESTINATION ${PLUGIN_DEST_DIR}/plugins COMPONENT Runtime
)
MESSAGE( "Sourcing Qt Shared Libraries from <${QT_PLUGINS_DIR}/imageformats> and <${QT_LIBRARY_DIRS}>" )
MESSAGE( "Sourcing ITK Shared Libraries from <${ITK_LIBRARY_DIRS}>" )

INSTALL( CODE "
  file( WRITE \"\${CMAKE_INSTALL_PREFIX}/${QTCONF_DEST_DIR}/qt.conf\" \"\" )
  " COMPONENT Runtime
)

SET( DIRS ${QT_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS} )
INSTALL( CODE "
  file( GLOB_RECURSE QTPLUGINS
    \"\${CMAKE_INSTALL_PREFIX}/${PLUGIN_DEST_DIR}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
  include(BundleUtilities)
  fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
  " COMPONENT Runtime
)

