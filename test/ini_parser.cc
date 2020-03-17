/*
  测试用例：
    1.空路径构造
    2.无参构造
    3.非法路径构造
    4.文本前置空格、制表
    5.文本无'='
    6.文本中置空格、制表
    7.文本后置空格、制表
    8.文本无换行、多个等号
    9.文本空
 */
//测试状态：ok

#include<cppunit/TestCase.h>
#include<cppunit/TestRunner.h>
#include<cppunit/TestResult.h>
#include<cppunit/TestResultCollector.h>
#include<cppunit/TextOutputter.h>

#include"utils/ini_parser.hpp"

using namespace std;
using amigao::IniParser;

char*ini_file_path;

class Test:public CppUnit::TestCase{
public:
  virtual void runTest()override{
    IniParser iniParser1("");
    IniParser iniParser2();
    IniParser iniParser3("fff");
    IniParser iniParser4(ini_file_path);
    auto result = iniParser4.get_result();
    for(auto iter=result.begin();iter!=result.end();++iter)
      cout<<iter->first<<"\n"<<iter->second<<"\n----------------"<<endl;
  }
};
int main(int argc,char**argv){
  ini_file_path=argv[1];
  Test *test=new Test();
  CppUnit::TestRunner runner;
  runner.addTest(test);
  CppUnit::TestResult result;
  CppUnit::TestResultCollector resultCollector;
  result.addListener(&resultCollector);
  runner.run(result);
  CppUnit::TextOutputter outputter(&resultCollector,std::cout);
  outputter.write();
}
