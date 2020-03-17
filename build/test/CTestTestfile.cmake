# CMake generated Testfile for 
# Source directory: /home/ydssbbnc/study/hwork/graduationPrj/amigao/test
# Build directory: /home/ydssbbnc/study/hwork/graduationPrj/amigao/build/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(sql_db_operation_test1 "sql_db_operation_test" "data/mariadb_info.ini" "data/cppjieba_dict.ini")
set_tests_properties(sql_db_operation_test1 PROPERTIES  _BACKTRACE_TRIPLES "/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;10;add_test;/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;0;")
add_test(db_pool_list_test1 "db_pool_list_test")
set_tests_properties(db_pool_list_test1 PROPERTIES  _BACKTRACE_TRIPLES "/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;11;add_test;/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;0;")
add_test(parse_url_test1 "parse_url_test")
set_tests_properties(parse_url_test1 PROPERTIES  _BACKTRACE_TRIPLES "/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;12;add_test;/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;0;")
add_test(ini_parser_test1 "ini_parser_test" "test/data/ini_parser.ini")
set_tests_properties(ini_parser_test1 PROPERTIES  _BACKTRACE_TRIPLES "/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;13;add_test;/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;0;")
add_test(format_url_test1 "format_url_test")
set_tests_properties(format_url_test1 PROPERTIES  _BACKTRACE_TRIPLES "/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;14;add_test;/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;0;")
add_test(db_info_test1 "db_info_test" "test/data/db_info.ini")
set_tests_properties(db_info_test1 PROPERTIES  _BACKTRACE_TRIPLES "/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;15;add_test;/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;0;")
add_test(crypt_string_test1 "crypt_string_test")
set_tests_properties(crypt_string_test1 PROPERTIES  _BACKTRACE_TRIPLES "/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;16;add_test;/home/ydssbbnc/study/hwork/graduationPrj/amigao/test/CMakeLists.txt;0;")
