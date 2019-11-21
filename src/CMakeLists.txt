include_directories(./include)


aux_source_directory(./renders/renderobj  RenderObj_src)
aux_source_directory(./renders/camera  Camera_src)
aux_source_directory(./renders Render_src)
aux_source_directory(./scenes Scenes_src)
aux_source_directory(./include/json Json_src)
aux_source_directory(./model Model_src)

add_library(src ${Scenes_src} ${Json_src} ${Render_src} ${Camera_src} ${RenderObj_src} ${Model_src})
