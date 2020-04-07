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

#include"db/sql_db_operation.hpp"
#include"strategy/reverse_valuate_strategy.hpp"
#include"shmctrl.h"

#include<sstream>
#include<netdb.h>
#include<cstring>
#include<string>
#include<syslog.h>
#include<iostream>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<fcntl.h>
#include<fstream>
#include<sys/stat.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<gumbo.h>
#include<unordered_set>
#include<regex>
#include<fstream>

using namespace std;

extern void format_url(string&);
extern void parse_url(string url,string&host_url,string& page_path);
extern int gbk2utf8(const string&,string&output);
extern void find_title(const GumboNode*,string&title);
extern void get_links_contents(GumboNode*,unordered_set<string>&links,string&contents);

static string gbk_pattern="<\\s*(meta){1}\\s*.*[(/>)(<\\s*/meta\\s*>)]{0}(charset)+\\s*={1}\\s*\"?'?\\s*(((gB)+)|((Gb)+)|((gb)+)|((GB)+)).*[(/>)(<\\s*/meta\\s*>)]{1}";
static regex gbk_reg((gbk_pattern));
static const int port=80;
shmctrl*shmptr;
int shmid;

void sig_alrm(int){}
void at_exit(){
  cout<<"-------<crawl_worker at_exit/>--------\n";
  if(!shmdt((void*)shmptr))
    cerr<<"----------<crawl_worker shmdt error/>--------"<<endl;
}
int main(int argc,char**argv){
  if(argc!=5)
    cerr<<"usage: <"<<argv[0]<<"> <db conf path> <read url> <write url> <cppjieba dict path>"<<endl,exit(1);
  string db_conf_path=argv[1];
  string read_url=argv[2];
  string write_url=argv[3];
  string cppjieba_conf_path=argv[4];
  amigao::DBOperationInterface*db_op=new amigao::SqlDBOperation(db_conf_path);
  amigao::ValuateStrategyInterface*valuate_strategy=new amigao::ReverseValuateStrategy(cppjieba_conf_path,db_op);
  key_t shmkey=ftok("./master",0);
  atexit(at_exit);
  if((shmid=shmget(shmkey,0,0777))<0)
    syslog(LOG_ERR,"------<crawl_worker shmget error/>-------"),exit(1);
  if((shmptr=(shmctrl*)shmat(shmid,0,0))==(shmctrl*)-1)
    syslog(LOG_ERR,"-------<crawl_worker shmat error/>--------"),exit(1);
  if(signal(SIGALRM,sig_alrm)==SIG_ERR)
    syslog(LOG_ERR,"-------<crawl_worker signal:sig_alrm error/>-------"),exit(1);
  char work_url[URL_SIZE];
  bool has_url=false;

  while(true){
  start:
    //try to get a url from shared memory
    if(has_url)goto has_url;
    pthread_mutex_lock(&(shmptr->url_lock));
    if(shmptr->url_head==shmptr->url_tail){//empty queue
      pthread_mutex_unlock(&(shmptr->url_lock));
      pthread_mutex_lock(&(shmptr->url_lists_lock));
      ifstream ifs(read_url);
      if(!ifs.is_open()){
	cerr<<"-------<crawl_worker fopen(r) error/>-------"<<endl;
	exit(1);
      }
      string buf="";
      getline(ifs,buf);
      ifs.close();
      if(!buf.empty()){
	string sed_rm="sed -i '1,1d' "+read_url;
	system(sed_rm.c_str());
	pthread_mutex_unlock(&(shmptr->url_lists_lock));
	goto has_url;
      }
      pthread_mutex_unlock(&(shmptr->url_lists_lock));
      goto start;
    }
    {
      //now, we can get a url from shared memory
      unsigned int i=0;
      for(;i<URL_SIZE&&shmptr->url_queue[shmptr->url_head][i]!='\0';++i)
	work_url[i]=shmptr->url_queue[shmptr->url_head][i];
      if(i<URL_SIZE)
	work_url[i]='\0';
      shmptr->url_head=(shmptr->url_head+1)%URL_NUMS;
    }
    pthread_mutex_unlock(&(shmptr->url_lock));
  has_url:
    has_url=false;
    if(db_op->get_website_nice(work_url)==0||
       db_op->if_exist("url","amigao.visited_url","url",crypt(work_url)))
      goto start;
    string host_url="",page_path="";
    parse_url(work_url,host_url,page_path);
    struct hostent*host;
    if(!(host=gethostbyname(host_url.c_str()))){
      db_op->op_website_nice(work_url,"0");
      goto start;
    }
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=((struct in_addr*)(host->h_addr))->s_addr;
    int sockfd;
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
      db_op->op_website_nice(work_url,"0");
      goto start;
    }
    string req_header=
      "GET "+page_path+" HTTP/1.1\r\n"+
      "Host: "+host_url+"\r\n"+
      "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"+
      "Referer: https://cn.bing.com/\r\n"+
      "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.132 Safari/537.36\r\n"+
      "Connection: Close\r\n\r\n";
    if(connect(sockfd,(const sockaddr*)&addr,sizeof(addr))<0){
      db_op->op_website_nice(work_url,"0");
      goto start;
    }
    if(send(sockfd,req_header.c_str(),req_header.size(),0)<0){
      db_op->op_website_nice(work_url,"0");
      goto start;
    }
    string formated_html="";
    {
      char html_buf[1024];
      string raw_html="";
      int len;
      while((len=recv(sockfd,html_buf,1024,0))>0){
	raw_html+=html_buf;
	if(raw_html.size()>HTML_SIZE){
	  db_op->op_website_nice(work_url,"0");
	  goto start;
	}
      }
      if(len<0){
	db_op->op_website_nice(work_url,"0");
	goto start;
      }
      string status_code="";
      {
	string end_header="\r\n\r\n";
	long i;
	if((i=raw_html.find(end_header))<=0){
	  db_op->op_website_nice(work_url,"0");
	  goto start;
	}
	long j;
	string resp_header=raw_html.substr(0,i);
	for(i=0;i<resp_header.size();++i)
	  if(resp_header[i]>='A'&&resp_header[i]<='Z')
	    resp_header[i]+=32;
	if((j=resp_header.find("http/"))<0){
	  db_op->op_website_nice(work_url,"0");
	  goto start;
	}
	if(i+4>=raw_html.size()){
	  db_op->op_website_nice(work_url,"0");
	  goto start;
	}
	formated_html=raw_html.substr(i+4,raw_html.size());
	while(j<resp_header.size()&&resp_header[j]!=' '&&resp_header[j]!='\t'&&resp_header[j]!='\r')++j;
	while(j<resp_header.size()&&(resp_header[j]==' '||resp_header[j]=='\t'))++j;
	long k=j;
	for(;k<resp_header.size()&&resp_header[k]!=' '&&resp_header[k]!='\t'&&
	      resp_header[k]!='\r';++k);
	if(j>=resp_header.size()){
	  db_op->op_website_nice(work_url,"0");
	  goto start;
	}
	status_code=resp_header.substr(j,k-j);
	if(status_code.empty()||status_code[0]!='2'){
	  db_op->op_website_nice(work_url,"0");
	  goto start;
	}
      }
    }
    if(regex_search(formated_html,gbk_reg))//convert gbk encode to utf-8
      gbk2utf8(string(formated_html),formated_html);
    pthread_mutex_lock(&(shmptr->html_lock));
    if((shmptr->html_tail+1)%HTML_NUMS==shmptr->html_head){
      pthread_mutex_unlock(&(shmptr->html_lock));
      //html queue is full,we can do parse job ourselve...~_~
      GumboOutput*output=gumbo_parse(formated_html.c_str());
      unordered_set<string>links;
      string title="",contents="";
      find_title(output->root,title);
      get_links_contents(output->root,links,contents);
      gumbo_destroy_output(&kGumboDefaultOptions,output);
      links.erase(work_url);
      string html_="";
      db_op->valuate_page(work_url,title,contents,html_,valuate_strategy);
      for(auto iter=links.begin();iter!=links.end();){
	string tmp_url=*iter;
	if(db_op->get_website_nice(tmp_url)==0||
	   db_op->if_exist("url","amigao.visited_url","url",crypt(tmp_url))){
	  iter=links.erase(iter);
	}else
	  ++iter;
      }
      if(links.empty())goto start;
      {
	string tmp_url=*links.begin();
	size_t i=0;
	for(;i<tmp_url.size();++i)work_url[i]=tmp_url[i];
	if(i<URL_SIZE)work_url[i]='\0';
	links.erase(tmp_url);
	has_url=true;
      }
      long url_insert_size;
      pthread_mutex_lock(&(shmptr->url_lock));
      if(shmptr->url_head==shmptr->url_tail)
	url_insert_size=URL_NUMS-1;
      else
	url_insert_size=(shmptr->url_head-shmptr->url_tail+URL_NUMS)%URL_NUMS-1;
      if(url_insert_size>links.size())
	url_insert_size=links.size();
      while(url_insert_size>0&&!links.empty()){
	string tmp_url=*links.begin();
	links.erase(tmp_url);
	size_t i=0;
	for(;i<tmp_url.size()&&i<URL_SIZE;++i)
	  shmptr->url_queue[shmptr->url_tail][i]=tmp_url[i];
	if(i<URL_SIZE)
	  shmptr->url_queue[shmptr->url_tail][i]='\0';
	shmptr->url_tail=(shmptr->url_tail+1)%URL_NUMS;
	--url_insert_size;
      }
      pthread_mutex_unlock(&(shmptr->url_lock));
      if(links.empty())goto start;
      FILE*fp=fopen(write_url.c_str(),"a+");
      if(!fp){
	cerr<<"---<crawl_worker fopen(a+) error/>---"<<endl;
	exit(1);
      }
      while(!links.empty()){
	string tmp_str=*links.begin();
	links.erase(tmp_str);
	fputs((tmp_str+"\n").c_str(),fp);
      }
      fclose(fp);
    }
    else{
      size_t i=0;
      for(;i<URL_SIZE&&work_url[i]!='\0';++i)
	shmptr->html_queue[shmptr->html_tail][i]=work_url[i];
      if(i<URL_SIZE)
	shmptr->html_queue[shmptr->html_tail][i]='\0';
      i=URL_SIZE;
      while(i<formated_html.size()&&i<HTML_SIZE){
	shmptr->html_queue[shmptr->html_tail][i]=formated_html[i];
	++i;
      }
      if(i<HTML_SIZE)
	shmptr->html_queue[shmptr->html_tail][i]='\0';
      shmptr->html_tail=(shmptr->html_tail+1)%HTML_NUMS;
      pthread_mutex_unlock(&(shmptr->html_lock));
    }
  }
}
