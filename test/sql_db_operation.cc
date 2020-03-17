/*
测试：
  1.停用词
  2.不存在网页
  3.存在网页且正常
  4.不存在关键字
  5.存在关键字，新结点在头
  6.存在关键字，新结点在尾
  7.存在关键字，新结点在中间
  8.存在关键字，但已満
  9.已存在网页
 */
#include<iostream>
#include<cppunit/TestCase.h>
#include<cppunit/TestRunner.h>
#include<cppunit/TestResult.h>
#include<cppunit/TestResultCollector.h>
#include<cppunit/TextOutputter.h>

#include"db/sql_db_operation.hpp"
#include"strategy/reverse_valuate_strategy.hpp"

using namespace std;

namespace amigao{
  class Test:public CppUnit::TestCase{
    DBOperationInterface*db_op;
    ValuateStrategyInterface*valuate_strategy;
  public:
    explicit Test(string sql_db_conf_path_,string cppjieba_dict_conf_ini_){
      db_op=new SqlDBOperation(sql_db_conf_path_);
      valuate_strategy=new ReverseValuateStrategy(cppjieba_dict_conf_ini_);
    }
    virtual void runTest()override{
      string url="test.paid",title="hey",
	contents="从前有座山，。",
	html="<html><meta httdfls";
      db_op->valuate_page(url,title,contents,html,valuate_strategy);
    }
    ~Test(){
      delete db_op;
      delete valuate_strategy;
    }
  };
}
int main(int argc,char**argv){
  using amigao::Test;
  Test*test=new Test(argv[1],argv[2]);
  CppUnit::TestRunner runner;
  runner.addTest(test);
  CppUnit::TestResult result;
  CppUnit::TestResultCollector resultCollector;
  result.addListener(&resultCollector);
  runner.run(result);
  CppUnit::TextOutputter outputter(&resultCollector,cout);
  outputter.write();
}








