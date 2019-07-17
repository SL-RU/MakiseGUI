file(STRINGS SOURCES s)
file(STRINGS HEADERS h)

add_library (MakiseGUI ${s})
set(hh "${CMAKE_CURRENT_SOURCE_DIR}/")
foreach(he ${h})
  set(hh ${hh};${CMAKE_CURRENT_SOURCE_DIR}/${he})
endforeach()
message("hh ${hh}")
target_include_directories (MakiseGUI
  PUBLIC ${hh})
target_include_directories (MakiseGUI
  INTERFACE ${hh})


TARGET_LINK_LIBRARIES(MakiseGUI gui)
