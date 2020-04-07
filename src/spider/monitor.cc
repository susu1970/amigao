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

#include"spider/shmctrl.h"

#include<signal.h>
#include<syslog.h>
#include<cstring>
#include<string>
#include<pthread.h>
#include<sys/shm.h>
#include<iostream>
#include<fstream>

using namespace std;

static int shmid;
static shmctrl*shmptr;

static void at_exit();
static void sig_alrm(int){}

void print_shmctrl(){
  cout<<"-------<shmctrl info>--------\nURL_SIZE: "<<URL_SIZE<<"\nURL_NUMS: "<<URL_NUMS<<"\nHTML_SIZE: "<<HTML_SIZE<<"\nHTML_NUMS: "<<HTML_NUMS<<"\nurl_head: "<<shmptr->url_head<<"\nurl_tail: "<<shmptr->url_tail<<"\nhtml_head: "<<shmptr->html_head<<"\nhtml_tail:"<<shmptr->html_tail<<"\n";
  /*
  if(pthread_mutex_trylock(&(shmptr->url_lock))){
    cout<<"url_lock: locked"<<endl;
  }else{
    cout<<"url_lock: unlocked"<<endl;
    pthread_mutex_unlock(&(shmptr->url_lock));
  }
  if(pthread_mutex_trylock(&(shmptr->html_lock))){
    cout<<"html_lock: locked"<<endl;
  }else{
    pthread_mutex_unlock(&(shmptr->html_lock));
    cout<<"html_lock: unlocked"<<endl;
  }
  */
  cout<<"---------</shmctrl info>---------"<<endl;
}

int main(int argc,char**argv){
  atexit(at_exit);
  key_t shmkey=ftok("./master",0);
  if((shmid=shmget(shmkey,0,0777))<0)
    cerr<<"---<monitor shmget error/>---"<<endl,exit(1);
  if((shmptr=(shmctrl*)shmat(shmid,0,0))==(shmctrl*)-1)
    cerr<<"---<monitor shmat error/>---"<<endl,exit(1);
  if(signal(SIGALRM,sig_alrm)==SIG_ERR)
    cerr<<"---<monitor signal error/>---"<<endl,exit(1);
  if(signal(SIGCONT,SIG_DFL)==SIG_ERR)
    cerr<<"---<monitor signal error/>---"<<endl,exit(1);
  cout<<"---<monitor pid>---"<<endl;
  cout<<getpid()<<endl;
  cout<<"---</monitor pid>---"<<endl;
  while(true){
    alarm(3);
    print_shmctrl();
    pause();
  }
  exit(0);
}
void at_exit(){
  cout<<"---<monitor exiting.../>---"<<endl;
  if(!shmdt((void*)shmptr))
    cerr<<"---<monitor shmdt error/>---"<<endl;
}
