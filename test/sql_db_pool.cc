#include<db/sql_db_pool.hpp>

#include<cppunit/TestCase.h>
#include<cppunit/TestRunner.h>
#include<cppunit/TestResult.h>
#include<cppunit/TestResultCollector.h>
#include<cppunit/TextOutputter.h>
#include<string>
#include<iostream>

using namespace std;
using amigao::SqlDBPool;
class Test:public CppUnit::TestCase{
  string sql_db_conf_path="";
public:
  explicit Test(string sql_db_conf_path_)
    :sql_db_conf_path(sql_db_conf_path_){    
  }
  virtual void runTest()override{
    SqlDBPool *db_pool=new SqlDBPool(sql_db_conf_path);
    CPPUNIT_ASSERT(db_pool&&
		   db_pool->get_used_pool_node_numbers()==0&&
		   db_pool->get_ununsed_pool_node_numbers()==1);
    void*v1=db_pool->get_connection();
    CPPUNIT_ASSERT(db_pool&&
		   db_pool->get_used_pool_node_numbers()==1&&
		   db_pool->get_ununsed_pool_node_numbers()==1);
    void*v2=db_pool->get_connection();
    void*v3=db_pool->get_connection();
    void*v4=db_pool->get_connection();
    CPPUNIT_ASSERT(db_pool&&
		   db_pool->get_used_pool_node_numbers()==4&&
		   db_pool->get_ununsed_pool_node_numbers()==1);    
    void*v5=(void*)new string("ll");
    db_pool->recycle_connection(v5);
    CPPUNIT_ASSERT(db_pool&&
		   db_pool->get_used_pool_node_numbers()==4&&
		   db_pool->get_ununsed_pool_node_numbers()==1);
    delete (string*)v5;
    db_pool->recycle_connection(v4);
    CPPUNIT_ASSERT(db_pool&&
		   db_pool->get_used_pool_node_numbers()==3&&
		   db_pool->get_ununsed_pool_node_numbers()==2);
    db_pool->recycle_connection(v3);
    CPPUNIT_ASSERT(db_pool&&
		   db_pool->get_used_pool_node_numbers()==2&&
		   db_pool->get_ununsed_pool_node_numbers()==3);
    db_pool->recycle_connection(v2);
    CPPUNIT_ASSERT(db_pool&&
		   db_pool->get_used_pool_node_numbers()==1&&
		   db_pool->get_ununsed_pool_node_numbers()==4);
    db_pool->recycle_connection(v1);
    CPPUNIT_ASSERT(db_pool&&
		   db_pool->get_used_pool_node_numbers()==0&&
		   db_pool->get_ununsed_pool_node_numbers()==4);
  }
};
int main(int argc,char**argv){
  Test*test=new Test(argv[1]);
  CppUnit::TestRunner runner;
  CppUnit::TestResult result;
  CppUnit::TestResultCollector resultCollector;
  runner.addTest(test);
  result.addListener(&resultCollector);
  runner.run(result);
  CppUnit::TextOutputter outputter(&resultCollector,std::cout);
  outputter.write();
}
