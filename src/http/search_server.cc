// Copyright 2020 amigao. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: 758293230@qq.com
//

#include"utils/cppjieba/Jieba.hpp"
#include"utils/ini_parser.hpp"
#include"db/sql_db_operation.hpp"
#include"strategy/plain_search_strategy.hpp"

#include<unordered_map>
#include<string.h>
#include<fcgiapp.h>
#include<sstream>
#include<string>
#include<iostream>
#include<fcgi_stdio.h>
#include<vector>

using namespace std;

extern string deescape_url(const string&url);
extern string str2html(const string &);

const static string http_response_header="HTTP/1.1 200 OK\r\nContent-Type: text/html;charset=utf-8\r\n\r\n";
static amigao::DBOperationInterface*db_op;
static amigao::SearchStrategyInterface*search_strategy;
static cppjieba::Jieba *jieba;
static FCGX_ParamArray evnp;
static FCGX_Stream*in,*out,*err;
static stringstream ss;
int main(int argc,char**argv){
  if(argc!=3)
    cerr<<"usage: <"<<argv[0]<<"> <db conf path> <cppjieba conf path> "<<endl,exit(1);
  {
    string db_conf_path=argv[1];
    string cppjieba_conf_path=argv[2];
    amigao::IniParser ini_parser(cppjieba_conf_path);
    string DICT_PATH=ini_parser.get_value("DICT_PATH");
    string HMM_PATH=ini_parser.get_value("HMM_PATH");
    string USER_DICT_PATH=ini_parser.get_value("USER_DICT_PATH");
    string IDF_PATH=ini_parser.get_value("IDF_PATH");
    string STOP_WORD_PATH=ini_parser.get_value("STOP_WORD_PATH");
    db_op=new amigao::SqlDBOperation(argv[1]);
    jieba=new cppjieba::Jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH);
    search_strategy=new amigao::PlainSearchStrategy(jieba);
  }
  while(FCGX_Accept(&in,&out,&err,&evnp)>=0){
    string amigao_title="amigao";
    string amigao_body2body=" ";
    string final_response="";
    char*request_method=FCGX_GetParam("REQUEST_METHOD",evnp);
    if(strcmp(request_method,"GET")){
      final_response=http_response_header+"<html><head><title>Invalid Request Method(only GET supported)</title><meta charset='utf-8'><link rel='stylesheet' type='text/css' href='/css/a_link.css'><link rel='stylesheet' type='text/css' href='/css/scrollbar_orange.css'>	<style> html,body{background-color:#282a36;color:#868686;margin:0;}</style></head><body style='overflow-x:hidden'><h1 style='text-align:center;'>Invalid Request Method (only GET is allowed)</h1></body></html>";
      FCGX_FPrintF(out,final_response.c_str());
      continue;
    }
    string sentence="";
    short start_page=0,results_per_page=0;
    //format: /amigao?q=xxxxx&start_page=x&results_per_page=x
    {
      string get_param=(FCGX_GetParam("QUERY_STRING",evnp));
      size_t i=0;
      while(i<get_param.size()&&get_param[i]!='=')++i;
      size_t j=++i;
      if(j>=get_param.size())continue;
      while(i<get_param.size()&&get_param[i]!='&')++i;
      sentence=deescape_url(get_param.substr(j,i-j));
      while(i<get_param.size()&&get_param[i]!='=')++i;
      if((j=++i)>=get_param.size())goto start_search;
      while(i<get_param.size()&&get_param[i]!='&')++i;
      ss<<(get_param.substr(j,i-j));
      ss>>start_page;
      ss.clear();
      if((j=++i)>=get_param.size())goto start_search;
      while(i<get_param.size()&&get_param[i]!='=')++i;
      if((j=++i)>=get_param.size())goto start_search;
      while(i<get_param.size()&&get_param[i]!='&')++i;
      ss<<(get_param.substr(j,i-j));
      ss>>results_per_page;
      ss.clear();
    }
  start_search:
    amigao::PlainSearchResult search_result;
    cout<<"---<before>---"<<endl;
    cout<<"start_page: "<<start_page<<endl;
    cout<<"results_per_page: "<<results_per_page<<endl;
    cout<<"sentence: "<<sentence<<endl;
    cout<<"---</before>---"<<endl;
    search_strategy->handle(sentence,(void*)&search_result,db_op,start_page,results_per_page,true);
    cout<<"---<after>---"<<endl;
    cout<<"start_page: "<<start_page<<endl;
    cout<<"results_per_page: "<<results_per_page<<endl;
    cout<<"sentence: "<<sentence<<endl;
    cout<<"---</after>---"<<endl;
    if(start_page<0||results_per_page<0){
      final_response=http_response_header+"<html><head><title>amigao</title><meta charset='utf-8'><link rel='stylesheet' type='text/css' href='/css/a_link.css'><link rel='stylesheet' type='text/css' href='/css/scrollbar_orange.css'><link rel='stylesheet' type='text/css' href='/css/paging.css'></link> <style> html,body{background-color:#282a36;color:#868686;margin:0;}</style></head><body style='overflow-x:hidden'><h3>sorry,there are no result about:<br></h3>&nbsp;&nbsp;&nbsp;"+sentence+"</body></html>";
      FCGX_FPrintF(out,final_response.c_str());
      continue;
    }
    string html2body="<html><head><title>amigao</title><meta charset='utf-8'><link rel='stylesheet' type='text/css' href='/css/a_link.css'><link rel='stylesheet' type='text/css' href='/css/scrollbar_orange.css'><link rel='stylesheet' type='text/css' href='/css/paging.css'></link> <style> html,body{background-color:#282a36;color:#868686;margin:0;}</style><script>function paging(page){window.parent.amigao_paging('"+sentence+"',page,"+to_string(results_per_page)+");}</script></head><body style='overflow-x:hidden'><h3 style='text-align:center'>"+sentence+"</h3>";
    string search_a="";
    for(size_t i=0;i<search_result.size();++i){
      string url=search_result.get_url(i);
      string contents=str2html(search_result.get_contents(i));
      string title=str2html(search_result.get_title(i));
      string time=search_result.get_time(i);
      if(url.empty())url="#empty";
      if(contents.empty())contents="#empty";
      if(title.empty())title=url;
      if(time.empty())time="#empty";
      search_a+=("<div><a href='http://"+url+"' title='"+url+"' target='_blank' style='font-size:18px'>"+title+"</a>&nbsp;&nbsp;&nbsp;&nbsp;"+time+"<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+contents+"...<br><i>"+url+"</i></div><br>");
    }
    string paging="";
    short i=start_page-DFL_PAGES/2;
    short pre=start_page-1,next=start_page+1;
    if(i<1)i=1;
    short j=i+DFL_PAGES-1;
    if(j>results_per_page)j=results_per_page;
    if(pre<1)pre=results_per_page;
    if(next>j)next=1;
    string str_pre=to_string(pre),str_next=to_string(next);
    paging="<div style='text-align:center;'><ul class='paging'><li><a href='javascript:paging("+str_pre+")'>«</a></li>";
    while(i<=j){
      string str_i=to_string(i);
      if(i==start_page)
	paging+=("<li><a href='#' class='active'>"+str_i+"</a></li>");
      else
	paging+=("<li><a href='javascript:paging("+str_i+")'>"+str_i+"</a></li>");
      ++i;
    }
    paging+="<li><a href='javascript:paging("+str_next+")'>»</a></li></ul></div></body></html>";
    final_response=http_response_header+html2body+search_a+paging;
    FCGX_FPrintF(out,final_response.c_str());
  }
}
