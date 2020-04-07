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

#include"utils/ini_parser.hpp"
#include"db/sql_db_operation.hpp"
#include"spider/shmctrl.h"

#include<signal.h>
#include<vector>
#include<syslog.h>
#include<cstring>
#include<string>
#include<sched.h>
#include<pthread.h>
#include<sys/shm.h>
#include<iostream>
#include<fstream>
#include<sys/wait.h>
#include<unordered_map>

using namespace std;

extern void format_url(string &url);

static int shmid;
static shmctrl*shmptr;
pthread_mutexattr_t attr;
static string write_url;
static unordered_map<pid_t,string>pid_str;

static void at_exit();
static void sig_int(int);
static void sig_alrm(int){}
static void sig_cld(int){
  pid_t cld_pid;
  int stat;
  cout<<"----------<master sig_cld>---------\n";
  while((cld_pid=waitpid(-1,&stat,WNOHANG))>0)
    cout<<cld_pid<<"("<<pid_str[cld_pid]<<") exited!\n";
  cout<<"-----------</master sig_cld>-----------"<<endl;
}

void print_shmctrl(){
  cout<<"-------<shmctrl info>--------\nURL_SIZE: "<<URL_SIZE<<"\nURL_NUMS: "<<URL_NUMS<<"\nHTML_SIZE: "<<HTML_SIZE<<"\nHTML_NUMS: "<<HTML_NUMS<<"\nurl_head: "<<shmptr->url_head<<"\nurl_tail: "<<shmptr->url_tail<<"\nhtml_head: "<<shmptr->html_head<<"\nhtml_tail:"<<shmptr->html_tail<<"\n";
  cout<<"---------</shmctrl info>---------"<<endl;
}

int main(int argc,char**argv){
  if(argc!=5&&argc!=6)
    cerr<<"usage: <"<<argv[0]<<"> <db conf path> /<start url> <read_url > <write url> <cppjieba_dict> ",exit(1);
  if(URL_NUMS<2||HTML_NUMS<2)
    cerr<<"------<master URL_NUMS or HTML_NUMS should more than 1/>-------"<<endl,exit(1);
  cout<<"---<master pid: "<<getpid()<<"/>---"<<endl;
  atexit(at_exit);
  string db_conf_path=argv[1];
  string read_url=argc>5?argv[3]:argv[2];
  amigao::DBOperationInterface*db_op=new amigao::SqlDBOperation(db_conf_path);
  write_url=argc>5?argv[4]:argv[3];
  string cppjieba_conf_path=argc>5?argv[5]:argv[4];
  key_t shm_key=ftok("./master",0);
  if((shmid=shmget(shm_key,sizeof(shmctrl),0777|IPC_CREAT|IPC_EXCL))<0)
    syslog(LOG_ERR,"-----------<master shmid error/>-----------"),exit(1);
  if((shmptr=(shmctrl*)shmat(shmid,0,0))==(shmctrl*)-1)
    syslog(LOG_ERR,"-----------<master shmat error/>-----------"),exit(1);
  if(signal(SIGINT,sig_int)==SIG_ERR)
    syslog(LOG_ERR,"-----------<master signal:sig_int error/>-----------"),exit(1);
  if(signal(SIGALRM,sig_alrm)==SIG_ERR)
    syslog(LOG_ERR,"-----------<master signal:sig_alrm error/>-----------"),exit(1);
  if(signal(SIGCLD,sig_cld)==SIG_ERR)
    syslog(LOG_ERR,"-----------<master signal:sig_cld error/>-----------"),exit(1);
  pid_t pid;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
  memset(shmptr,0,sizeof(shmctrl));
  pthread_mutex_init(&(shmptr->url_lock),&attr);
  pthread_mutex_init(&(shmptr->url_lists_lock),&attr);
  pthread_mutex_init(&(shmptr->html_lock),&attr);
  shmptr->url_tail=shmptr->url_head=shmptr->html_head=shmptr->html_tail=0;
  if(argc>5){
    ifstream ifs(argv[2]);
    if(!ifs.is_open())
      cerr<<"--------------<master ifstream error/>-----------"<<endl;
    else{
      string tmp_str="";
      char url_buf[URL_SIZE];
      while(getline(ifs,tmp_str)&&
	    (shmptr->url_tail+1)%URL_NUMS!=shmptr->url_head){
	format_url(tmp_str);
	if(tmp_str.empty()||
	   db_op->if_exist("url","amigao.visited_url","url",crypt(tmp_str))||
	   db_op->get_website_nice(tmp_str)==0)
	  continue;
	size_t i=0;
	for(;i<tmp_str.size()&&i<URL_SIZE;++i)
	  shmptr->url_queue[shmptr->url_tail][i]=tmp_str[i];
	if(i<URL_SIZE)
	  shmptr->url_queue[shmptr->url_tail][i]='\0';
	shmptr->url_tail=(shmptr->url_tail+1)%URL_NUMS;
      }
      ifs.close();
    }
  }
  print_shmctrl();
  delete db_op;
  cpu_set_t mask;
  for(int i=0;i<WORKERS;++i){
    if((pid=vfork())<0)
      syslog(LOG_ERR,"---------<master vfork error/>---------"),exit(1);
    if(!pid){
      CPU_ZERO(&mask);
      CPU_SET(i,&mask);
      execl("./crawl_worker","crawl_worker",db_conf_path.c_str(),read_url.c_str(),write_url.c_str(),cppjieba_conf_path.c_str(),(char*)0);
      exit(0);
    }
    pid_str[pid]="crawl_worker";
  }
  for(int i=0;i<WORKERS;++i){
    if((pid=vfork())<0)
      syslog(LOG_ERR,"---------<master vfork error/>---------"),exit(1);
    if(!pid){
      CPU_ZERO(&mask);
      CPU_SET(i,&mask);
      execl("./parse_worker","parse_worker",db_conf_path.c_str(),read_url.c_str(),write_url.c_str(),cppjieba_conf_path.c_str(),(char*)0);
      exit(0);
    }
    pid_str[pid]="parse_worker";
  }
  string top_str="top -p ";
  cout<<"---<master children pid>---"<<endl;
  for(auto iter=pid_str.begin();iter!=pid_str.end();++iter){
    cout<<iter->first<<" type: "<<iter->second<<endl;
    top_str+=(to_string(iter->first)+",");
  }
  top_str=top_str.substr(0,top_str.size()-1);
  cout<<"---</master children pid>---"<<endl;
  cout<<"---<master top_str>---"<<endl;
  cout<<top_str<<endl;
  cout<<"---</master top_str>---"<<endl;
  while(true){
    pause();
  }
  exit(0);
}
void at_exit(){
  cout<<"--------<master at_exit...>-------\n";
  FILE *fp=fopen(write_url.c_str(),"a+");
  if(!fp){
    cerr<<"-----<master fopen error/>------\n";
    if(shmctl(shmid,IPC_RMID,0)<0)
      cerr<<"------<master shmctl error/>-----\n";
    return;
  }
  while(shmptr->url_head!=shmptr->url_tail){
    char url_buf[URL_SIZE+1];
    url_buf[URL_SIZE]='\0';
    unsigned int i=0;
    for(;i<URL_SIZE&&shmptr->url_queue[shmptr->url_head][i]!='\0';++i)
      url_buf[i]=shmptr->url_queue[shmptr->url_head][i];
    if(i<URL_SIZE)
      url_buf[i]='\0';
    fputs(url_buf,fp);
    fputc('\n',fp);
    shmptr->url_head=(shmptr->url_head+1)%URL_NUMS;
  }
  if(shmctl(shmid,IPC_RMID,0)<0)
    cerr<<"------<master shmctl error/>-----\n";
  for(auto iter=pid_str.begin();iter!=pid_str.end();++iter){
    if(kill(iter->first,SIGINT)==-1)
      cerr<<"kill "<<iter->first<<"("<<iter->second<<") error"<<endl;
  }
}
void sig_int(int){
  exit(1);
}
