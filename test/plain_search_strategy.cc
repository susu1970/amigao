
#include"strategy/plain_search_strategy.hpp"
#include"db/sql_db_operation.hpp"
#include"utils/ini_parser.hpp"

#include<cppunit/TestCase.h>
#include<cppunit/TestRunner.h>
#include<cppunit/TestResult.h>
#include<cppunit/TestResultCollector.h>
#include<cppunit/TextOutputter.h>
#include<string>
#include<iostream>

using namespace std;

static string sentence="";

namespace amigao{
  class PlainSearchStrategyTest:public CppUnit::TestCase{
    std::string DICT_PATH="";
    std::string HMM_PATH="";
    std::string USER_DICT_PATH=""; 
    std::string IDF_PATH="";
    std::string STOP_WORD_PATH="";
    std::string db_conf_path="";
    cppjieba::Jieba*jieba;
    DBOperationInterface*db_op;
    PlainSearchStrategy*search_strategy;
    PlainSearchResult search_result;
  public:
    PlainSearchStrategyTest(std::string db_conf_path_,
			    std::string cppjieba_conf_path_):db_conf_path(db_conf_path_){
      IniParser iniParser(cppjieba_conf_path_);
      DICT_PATH=iniParser.get_value("DICT_PATH");
      HMM_PATH=iniParser.get_value("HMM_PATH");
      USER_DICT_PATH=iniParser.get_value("USER_DICT_PATH");
      IDF_PATH=iniParser.get_value("IDF_PATH");
      STOP_WORD_PATH=iniParser.get_value("STOP_WORD_PATH");
      jieba=new cppjieba::Jieba(DICT_PATH,
				HMM_PATH,
				USER_DICT_PATH,
				IDF_PATH,
				STOP_WORD_PATH);
      db_op=new SqlDBOperation(db_conf_path);
      search_strategy=new PlainSearchStrategy(jieba);
    }
    virtual void runTest(){
      short start_page=1,results_per_page=10;
      search_strategy->handle(sentence,(void*)&search_result,db_op,start_page,results_per_page,false);
      for(size_t i=0;i<search_result.size();++i){
	cout<<"------------------------------------------------------------------------------------------------------------"<<endl;
	cout<<"url: "<<search_result.get_url(i)<<endl;
	cout<<"title: "<<search_result.get_title(i)<<endl;
	cout<<"contents: "<<search_result.get_contents(i)<<endl;
	cout<<"time: "<<search_result.get_time(i)<<endl;
	cout<<"------------------------------------------------------------------------------------------------------------------------"<<endl;
      }
      cout<<"size: "<<search_result.size()<<endl;
    }
    ~PlainSearchStrategyTest(){
      delete search_strategy;
      delete db_op;
      delete jieba;
    }
  };
}
int main(int argc,char**argv){
  if(argc!=3)
    cerr<<"usage: <"<<argv[0]<<"> <db conf path> <cppjieba conf path>"<<endl,exit(1);
  cin>>sentence;
  CppUnit::Test*test=new amigao::PlainSearchStrategyTest(argv[1],argv[2]);
  CppUnit::TestRunner runner;
  runner.addTest(test);
  CppUnit::TestResult result;
  CppUnit::TestResultCollector resultCollector;
  result.addListener(&resultCollector);
  runner.run(result);
  CppUnit::TextOutputter outputter(&resultCollector,cout);
  outputter.write();
}
