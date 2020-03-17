/*测试用例：
    1.空
    2.正常串
    3.' " \ # 前中后置
 */
//测试状态：ok

#include<cppunit/TestCase.h>
#include<cppunit/TestRunner.h>
#include<cppunit/TestResult.h>
#include<cppunit/TestResultCollector.h>
#include<cppunit/TextOutputter.h>
#include<string>

using namespace std;

extern string crypt(const string&);
extern string decrypt(const string&);

class TestCase:public CppUnit::TestCase{
  string str;
public:
  TestCase(const string str_):str(str_){}
  virtual void runTest(){
    string crypt_str=crypt(str);
    string decrypt_str=decrypt(crypt_str);
    CPPUNIT_ASSERT(crypt_str.compare(decrypt_str)==0);
    CPPUNIT_ASSERT(crypt_str.find("\\")<0&&
		   crypt_str.find("\"")<0&&
		   crypt_str.find("'")<0);
  }
};
int main(int argc,char**argv){
  CppUnit::TestRunner runner;
  runner.addTest(new TestCase(""));
  runner.addTest(new TestCase("'\"\\##ksldfjk###\\\"\'"));
  runner.addTest(new TestCase("\"dksl'\\\"##"));
  runner.addTest(new TestCase("#kdfj#dksfj\""));
  CppUnit::TestResult result;
  CppUnit::TestResultCollector resultCollector;
  result.addListener(&resultCollector);
  runner.run(result);
  CppUnit::TextOutputter outputter(&resultCollector,std::cout);
  outputter.write();
}
