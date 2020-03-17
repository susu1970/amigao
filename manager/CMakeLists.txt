add_executable(add_dict_table_from_file add_dict_table_from_file.cc)
target_link_libraries(add_dict_table_from_file
  maria_op mariadbclient crypt_string
  )
add_executable(tmp tmp.cc)
target_link_libraries(tmp
  maria_op mariadbclient crypt_string  crypt_string
  )
