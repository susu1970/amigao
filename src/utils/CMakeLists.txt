add_library(format_url STATIC format_url.cc)
add_library(parse_html STATIC parse_html.cc)
add_library(parse_url STATIC parse_url.cc)
add_library(gbk2utf8 STATIC gbk2utf8.cc)
add_library(crypt_string STATIC crypt_string.cc)
add_library(maria_op SHARED maria_op.cc)

target_link_libraries(maria_op
  mariadbclient
)
target_link_libraries(parse_url
  format_url
)
target_link_libraries(parse_html
  format_url gumbo
)

