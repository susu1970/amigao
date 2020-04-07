#include"utils/op_redis.hpp"

#include<cppunit/TestCase.h>
#include<cppunit/TestRunner.h>
#include<cppunit/TestResult.h>
#include<cppunit/TestResultCollector.h>
#include<cppunit/TextOutputter.h>

using namespace std;

extern void format_url(string&);

class OpRedisTest:public CppUnit::TestCase{
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
  }
}
  int main(int argc,char**argv){
    OpRedisTest* redis_test=new OpRedisTest(argv[1]);
    CppUnit::TestRunner runner;
    runner.addTest(redis_test);
    CppUnit::TestResult result;
    CppUnit::TestResultCollector resultCollector;
    result.addListener(&resultCollector);
    runner.run(result);
    CppUnit::TextOutputter outputter(&resultCollector,std::cout);
    outputter.write();
  }
