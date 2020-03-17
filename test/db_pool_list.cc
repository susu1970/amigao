/*
测试：
 1.构造无法delete的connection
 2.正常构造
 3.每个函数多面调用一次
 */
//测试状态：ok
#include<cppunit/TestCase.h>
#include<cppunit/TestRunner.h>
#include<cppunit/TestResult.h>
#include<cppunit/TestResultCollector.h>
#include<cppunit/TextOutputter.h>
#include<iostream>
#include<unistd.h>

#include<db/db_pool_list.hpp>

using namespace std;
using amigao::DBPoolList;

class Test:public CppUnit::TestCase{
public:
  virtual void runTest()override{
    DBPoolList* dbPooList=new DBPoolList();
    DBPoolList *dbPooList2=new DBPoolList();

    int*i1=new int(1);
    int*i2=new int(2);
    int*i3=new int(3);
    string *s1=new string("1");
    string *s2=new string("2");
    string *s3=new string("3");
    dbPooList2->add_node((void*)s1);
    dbPooList2->add_node((void*)s2);
    dbPooList2->add_node((void*)s3);
    dbPooList->add_node((void*)i1);
    dbPooList->add_node((void*)i2);
    dbPooList->add_node((void*)i3);
    CPPUNIT_ASSERT(dbPooList->get_node_numbers()==3&&
		   dbPooList2->get_node_numbers()==3);
    dbPooList->pop_connection2(dbPooList2,(void*)i1);
    CPPUNIT_ASSERT(dbPooList->get_node_numbers()==2&&
		   dbPooList2->get_node_numbers()==4);
    dbPooList->pop_connection2(dbPooList2,(void*)s1);
    CPPUNIT_ASSERT(dbPooList->get_node_numbers()==2&&
		   dbPooList2->get_node_numbers()==4);
    dbPooList->pop_connection2(dbPooList2);
    CPPUNIT_ASSERT(dbPooList->get_node_numbers()==1&&
		   dbPooList2->get_node_numbers()==5);
    dbPooList->pop_connection2(dbPooList2);
    dbPooList->pop_connection2(dbPooList2);
    dbPooList->pop_connection2(dbPooList2);
    dbPooList->pop_connection2(dbPooList2);
    dbPooList->pop_connection2(dbPooList2);
    CPPUNIT_ASSERT(dbPooList->get_node_numbers()==0&&
		   dbPooList2->get_node_numbers()==6);
    CPPUNIT_ASSERT(dbPooList2->if_exist((void*)i1));
    CPPUNIT_ASSERT(dbPooList->if_exist((void*)i1)==false);
    dbPooList->delete_node();
    dbPooList2->delete_node((void*)i1);    
    CPPUNIT_ASSERT(dbPooList2->if_exist((void*)i1)==false&&
		   dbPooList->get_node_numbers()==0&&
		   dbPooList2->get_node_numbers()==5);
    delete dbPooList;
    delete dbPooList2;
  }
};
int main(int argc,char**argv){
  Test*test=new Test();
  CppUnit::TestRunner runner;
  runner.addTest(test);
  CppUnit::TestResult result;
  CppUnit::TestResultCollector resultCollector;
  result.addListener(&resultCollector);
  runner.run(result);
  CppUnit::TextOutputter outputter(&resultCollector,std::cout);
  outputter.write();
}
