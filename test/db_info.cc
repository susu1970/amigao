/*
测试：
  db_info.h中默认设置了数据库连接参数值，故只需设置需要另行设定的参数
 */
//测试状态：ok
#include<cppunit/TestCase.h>
#include<cppunit/TestRunner.h>
#include<cppunit/TestResult.h>
#include<cppunit/TestResultCollector.h>
#include<cppunit/TextOutputter.h>

#include<db/db_info.hpp>

using namespace std;
using amigao::DBInfo;

class Test:public DBInfo,public CppUnit::TestCase{
public:
  Test(string db_conf_path):DBInfo(db_conf_path){}
  virtual void runTest()override{
    cout<<"MAX_CONN: "<<MAX_CONN<<endl;
    cout<<"MAX_IDLE: "<<MAX_IDLE<<endl;
    cout<<"MIN_IDLE: "<<MIN_IDLE<<endl;
    cout<<"SLEEP: "<<SLEEP<<endl;
    cout<<"HOST: "<<HOST<<endl;
    cout<<"USER: "<<USER<<endl;
    cout<<"PASSWD: "<<PASSWD<<endl;
    cout<<"DB_NAME: "<<DB_NAME<<endl;
    cout<<"UNIX_SOCKET: "<<UNIX_SOCKET<<endl;
    cout<<"CLIENT_FLAG: "<<CLIENT_FLAG<<endl;
    cout<<"PORT: "<<PORT<<endl;
  }
};

int main(int argc,char**argv){
  if(argc<2)
    cerr<<"usage: <"<<argv[0]<<"> <db_info_path>"<<endl,exit(1);
  Test *test=new Test(argv[1]);
  CppUnit::TestRunner runner;
  runner.addTest(test);
  CppUnit::TestResult result;
  CppUnit::TestResultCollector resultCollector;
  result.addListener(&resultCollector);
  runner.run(result);
  CppUnit::TextOutputter outputter(&resultCollector,std::cout);
  outputter.write();
}
