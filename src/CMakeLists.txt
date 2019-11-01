include_directories(./scenes)

aux_source_directory(./scenes EulerOperations_src_source_files)

add_library(src ${EulerOperations_src_source_files})