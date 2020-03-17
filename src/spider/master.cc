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
// Author: susu1970@yandex.com 
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

using namespace std;

extern void format_url(string &url);

static int shmid;
static shmctrl*shmptr;
static string url_lists="";
static string db_conf_path="";
static string cppjieba_conf_path="";
static int WORKERS = sysconf(_SC_NPROCESSORS_CONF);
static cpu_set_t mask;
pthread_mutexattr_t attr;

static void at_exit();
static void sig_int(int);
static void sig_alrm(int){}
static void sig_cld(int){
  pid_t cld_pid;
  int stat;
  cld_pid=wait(&stat);
  cout<<"----------<master sig_cld>---------\nchild: "<<cld_pid<<" exited!\n-----------</master sig_cld>-----------"<<endl;
}
void print_shmctrl(){
  cout<<"-------<shmctrl info>--------\nURL_SIZE: "<<URL_SIZE<<"\nURL_NUMS: "<<URL_NUMS<<"\nHTML_SIZE: "<<HTML_SIZE<<"\nHTML_NUMS: "<<HTML_NUMS<<"\nurl_head: "<<shmptr->url_head<<"\nurl_tail: "<<shmptr->url_tail<<"\nhtml_head: "<<shmptr->html_head<<"\nhtml_tail:"<<shmptr->html_tail<<"\n---------</shmctrl info>---------"<<endl;
}

int main(int argc,char**argv){
  if(argc!=4&&argc!=5)
    cerr<<"usage: <"<<argv[0]<<"> <db conf path> /<start_url_lists> <url_lists> <cppjieba_dict>\n",exit(1);
  if(URL_NUMS<2||HTML_NUMS<2)
    cerr<<"------<master URL_NUMS or HTML_NUMS should more than 1/>-------"<<endl,exit(1);
  db_conf_path=argv[1];
  amigao::DBOperationInterface*db_op=new amigao::SqlDBOperation(db_conf_path);
  url_lists=argc>4?argv[3]:argv[2];
  cppjieba_conf_path=argc>4?argv[4]:argv[3];
  atexit(at_exit);
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
  pthread_mutex_init(&(shmptr->html_lock),&attr);
  shmptr->url_tail=shmptr->url_head=shmptr->html_head=shmptr->html_tail=0;
  if(argc>4){
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
	   db_op->if_exist("url","amigao.visited_url","url",tmp_str)||
	   db_op->get_website_nice(tmp_str)==0)
	  continue;
	size_t i=0;
	for(;i<tmp_str.size();++i){
	  shmptr->url_queue[shmptr->url_tail][i]=tmp_str[i];
	}
	if(i<URL_SIZE)
	  shmptr->url_queue[shmptr->url_tail][i]='\0';
	shmptr->url_tail=(shmptr->url_tail+1)%URL_NUMS;
      }
      ifs.close();
    }
  }
  print_shmctrl();
  delete db_op;
  cout<<"url_lists: "<<url_lists<<"\ndb_conf_path: "<<db_conf_path<<"\ncppjieba: "<<cppjieba_conf_path<<endl;
  for(int i=0;i<WORKERS;++i){
    if((pid=vfork())<0)
      syslog(LOG_ERR,"---------<master vfork error/>---------"),exit(1);
    if(!pid){
      CPU_ZERO(&mask);
      CPU_SET(i,&mask);
      execl("./crawl_worker","crawl_worker",db_conf_path.c_str(),url_lists.c_str(),cppjieba_conf_path.c_str(),(char*)0);
      exit(0);
    }
  }
  for(int i=0;i<WORKERS;++i){
    if((pid=vfork())<0)
      syslog(LOG_ERR,"---------<master vfork error/>---------"),exit(1);
    if(!pid){
      CPU_ZERO(&mask);
      CPU_SET(i,&mask);
      execl("./parse_worker","parse_worker",db_conf_path.c_str(),url_lists.c_str(),cppjieba_conf_path.c_str(),(char*)0);
      exit(0);
    }
  }
  while(true){
    alarm(10);
    pause();
    print_shmctrl();
  }
  exit(0);
}
void at_exit(){
  cout<<"--------<master at_exit...>-------\n";
  FILE *fp=fopen(url_lists.c_str(),"a+");
  if(!fp){
    cerr<<"-----<master fopen error/>------\n";
    if(shmctl(shmid,IPC_RMID,0)<0)
      cerr<<"------<master shmctl error/>-----\n";
    return;
  }
  pthread_mutex_lock(&(shmptr->url_lock));
  pthread_mutex_lock(&(shmptr->html_lock));
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
  pthread_mutex_destroy(&(shmptr->url_lock));
  pthread_mutex_destroy(&(shmptr->html_lock));
  pthread_mutexattr_destroy(&attr);
  if(shmctl(shmid,IPC_RMID,0)<0)
    cerr<<"------<master shmctl error/>-----\n";
}
void sig_int(int){
  exit(1);
}
