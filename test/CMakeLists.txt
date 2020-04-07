add_executable(plain_search_strategy_test plain_search_strategy.cc)

add_test(plain_search_strategy_test1 plain_search_strategy_test ~/conf/mariadb_info.ini ~/do/document/src/prj/amigao/data/cppjieba_dict.ini)

target_link_libraries(plain_search_strategy_test
  cppunit maria_op crypt_string 
)


#[[
add_executable(ini_parser_test ini_parser.cc)
add_executable(format_url_test format_url.cc)
add_executable(db_info_test db_info.cc)
add_executable(parse_url_test parse_url.cc)
add_executable(crypt_string_test crypt_string.cc)
add_executable(db_pool_list_test db_pool_list.cc)
add_executable(sql_db_pool_test sql_db_pool.cc)
add_executable(sql_db_operation_test sql_db_operation.cc)

add_test(sql_db_operation_test1 sql_db_operation_test data/mariadb_info.ini data/cppjieba_dict.ini)
add_test(db_pool_list_test1 db_pool_list_test)
add_test(parse_url_test1 parse_url_test)
add_test(ini_parser_test1 ini_parser_test test/data/ini_parser.ini)
add_test(format_url_test1 format_url_test)
add_test(db_info_test1 db_info_test test/data/db_info.ini)
add_test(crypt_string_test1 crypt_string_test)

target_link_libraries(sql_db_operation_test
  maria_op crypt_string cppunit 
)
target_link_libraries(sql_db_operation_test
  cppunit maria_op crypt_string parse_url
)
target_link_libraries(sql_db_pool_test
  cppunit maria_op
)
target_link_libraries(parse_url_test
  parse_url cppunit
)
target_link_libraries(db_pool_list_test
  cppunit
)
target_link_libraries(crypt_string_test
  crypt_string cppunit
)
target_link_libraries(format_url_test
  cppunit format_url
)
target_link_libraries(db_info_test
  cppunit
  )
target_link_libraries(ini_parser_test
  cppunit 
  )
]]
