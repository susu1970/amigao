add_executable(search_server search_server.cc)

target_link_libraries(search_server
  crypt_string escape_url maria_op fcgi str2html
)


