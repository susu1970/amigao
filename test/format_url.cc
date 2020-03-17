/*
  测试用例：
    1.空串
    2.串长度大于URL_SIZE
    3.包含大写
    4.包含前置空格、制表符
    5.包含前置'http'但不含'http://'
    6.包含前置'https'但不含'https://'
    7.包含前置'http://'
    8.包含前置'https://'
    9.包含前置'www.'
    10.包含前置'http://www.'
    11.包含前置'https://www.'
    12.包含后置空格、制表符、正斜杠或换行符
    13.包含所有前后置的中置
    14.包含已满足格式的url地址
    15.包含'javascript'
 */
//测试状态：ok
#include<string>
#include<cppunit/TestCase.h>
#include<cppunit/TestRunner.h>
#include<cppunit/TestResult.h>
#include<cppunit/TestResultCollector.h>
#include<cppunit/TextOutputter.h>

using namespace std;

extern void format_url(string&);

class FormatUrlTest:public CppUnit::TestCase{
  string url="";
public:
  void set_url(const string url_){
    url=url_;
    format_url(url);
  }
  string get_formated_url(){
    return url;
  }
  virtual void runTest()override{
    set_url("");
    CPPUNIT_ASSERT(get_formated_url().empty());
    set_url(string(4098,'s'));
    CPPUNIT_ASSERT(get_formated_url().empty());
    set_url(" \t BAIDU.COM/yds/\t  \n");
    CPPUNIT_ASSERT(get_formated_url().compare("baidu.com/yds")==0);
    set_url(" \t BAIDU.COM/yds/\t  \n/");
    CPPUNIT_ASSERT(get_formated_url().compare("baidu.com/yds")==0);    
    set_url("http://www.baidu.com/sf/");
    CPPUNIT_ASSERT(get_formated_url().compare("baidu.com/sf")==0);
    set_url("https://http.https.www.baidu.www.https.http.../http/https/.com/sl/");
    CPPUNIT_ASSERT(get_formated_url().compare("http.https.www.baidu.www.https.http.../http/https/.com/sl")==0);
    set_url("javascript/fkdj");
    //    CPPUNIT_ASSERT(get_formated_url().empty());
    CPPUNIT_ASSERT(get_formated_url().compare("javascript/fkdj")==0);
    set_url("google.com");
    CPPUNIT_ASSERT(get_formated_url().compare("google.com")==0);
    set_url("  javascript:dskfjskld kldsjfklsdj fdsjklsdksf/dslkkfjskldfjs");
    CPPUNIT_ASSERT(get_formated_url().empty());
    set_url("google.com/javascript:");
    CPPUNIT_ASSERT(get_formated_url().compare("google.com/javascript:")==0);
  }
};

int main(int argc,char**argv){
  FormatUrlTest* formatUrlTest=new FormatUrlTest;
  CppUnit::TestRunner runner;
  runner.addTest(formatUrlTest);
  CppUnit::TestResult result;
  CppUnit::TestResultCollector resultCollector;
  result.addListener(&resultCollector);
  runner.run(result);
  CppUnit::TextOutputter outputter(&resultCollector,std::cout);
  outputter.write();
}
