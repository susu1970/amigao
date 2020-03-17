add_executable (master master.cc)
add_executable (parse_worker parse_worker.cc)
add_executable (crawl_worker crawl_worker.cc)
#add_executable (monitor monitor.cc)

target_link_libraries(master
  format_url maria_op pthread crypt_string mariadbclient
)
target_link_libraries(crawl_worker
  gbk2utf8 format_url parse_url parse_html maria_op crypt_string pthread gumbo mariadbclient
)
target_link_libraries(parse_worker
  gumbo parse_html format_url maria_op crypt_string pthread mariadbclient
)

