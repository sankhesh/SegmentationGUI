ADD_LIBRARY( imageproc
			baseFilter.cxx                 baseFilter.h
			# cannyEdgeFilter.cxx            cannyEdgeFilter.h
)

TARGET_LINK_LIBRARIES(
   imageproc
   ${ITK_LIBRARIES}
   vtkHybrid
)
