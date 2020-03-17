#include<string>
#include<cppunit/TestCase.h>
#include<cppunit/TestRunner.h>
#include<cppunit/TestResult.h>
#include<cppunit/TestResultCollector.h>
#include<cppunit/TextOutputter.h>
#include<queue>

/*
测试用例：使用format_url的测试用例
测试状态：ok
 */

using namespace std;

extern void parse_url(string,string&,string&);

class TestCase:public CppUnit::TestCase{
  queue<vector<string>>url_vec_queue;
public:
  virtual void runTest()override{
    while(!url_vec_queue.empty()){
      auto tmp_vec=url_vec_queue.front();
      url_vec_queue.pop();
      string tmp_url=tmp_vec[0];
      string host_url_=tmp_vec[1],page_path_=tmp_vec[2];
      string host_url,page_path;
      parse_url(tmp_url,host_url,page_path);
      CPPUNIT_ASSERT(host_url.compare(host_url_)==0);
      CPPUNIT_ASSERT(page_path_.compare(page_path)==0);
    }
  }
  void add_url_vec(const vector<string> url_vec){
    url_vec_queue.push(url_vec);
  }
};
int main(int argc,char**argv){
  TestCase*testCase=new TestCase();
  string url,host_url,page_path;
  testCase->add_url_vec(vector<string>{
      "\t BAIDU.COM/yds/\t  \n",
	"baidu.com",
	"/yds"
    });
  testCase->add_url_vec((vector<string>{
	" \t BAIDU.COM/yds/\t  \n/",
	  "baidu.com",
	  "/yds"
      }));
  testCase->add_url_vec(vector<string>{
      string(22222,'k'),
	"",
	"/"
	});
  testCase->add_url_vec((vector<string>{
	"http://www.baidu.com/fs/",
	  "baidu.com",
	  "/fs"
      }));
  testCase->add_url_vec(vector<string>{
      "https://http.https.www.baidu.com.www.ttthps.http.../http/https.com/sl/",
	"http.https.www.baidu.com.www.ttthps.http...",
	"/http/https.com/sl"
    });
  testCase->add_url_vec(vector<string>{
      "javascript/fkdj/",
	"javascript",
	"/fkdj"
    });
  testCase->add_url_vec(vector<string>{
      "javascript:dfsdfsdlkfjl/dsflksdj/",
	"",
	"/"
	});
  CppUnit::TestRunner runner;
  runner.addTest(testCase);
  CppUnit::TestResult result;
  CppUnit::TestResultCollector resultCollector;
  result.addListener(&resultCollector);
  runner.run(result);
  CppUnit::TextOutputter outputter(&resultCollector,std::cout);
  outputter.write();
}

