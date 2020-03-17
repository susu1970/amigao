/*
 * Copyright 2020 by susu1970
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Email : susu1970@yandex.com
 */

#include"shmctrl.h"
#include"strategy/reverse_valuate_strategy.hpp"
#include"db/sql_db_operation.hpp"

#include<cstring>
#include<string>
#include<stdlib.h>
#include<unistd.h>
#include<syslog.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<signal.h>
#include<gumbo.h>
#include<stdio.h>
#include<iostream>
#include<set>

using namespace std;

extern void get_links_contents(GumboNode*,
			       set<string>&links,
			       string&contents);
extern void find_title(const GumboNode*,string&title);

static amigao::DBOperationInterface*db_op;
static amigao::ValuateStrategyInterface*valuate_strategy;
static string db_conf_path="",cppjieba_conf_path="",url_lists="";
shmctrl*shmptr;
int shmid;

static void at_exit(){
  delete db_op;
  delete valuate_strategy;
  if(!shmdt((void*)shmptr))
    cerr<<"----------<parse_worker shmdt error/>--------"<<endl;
}
static void sig_alrm(int){}

int main(int argc,char**argv){
  if(argc!=4)
    cerr<<"usage: <"<<argv[0]<<"> <db conf path> <url lists> <cppjieba dict path>"<<endl,exit(1);
  db_conf_path=argv[1];
  url_lists=argv[2];
  cppjieba_conf_path=argv[3];
  db_op=new amigao::SqlDBOperation(db_conf_path);
  valuate_strategy=new amigao::ReverseValuateStrategy(cppjieba_conf_path);
  atexit(at_exit);
  key_t shmkey=ftok("./master",0);
  if((shmid=shmget(shmkey,0,0777))<0)
    syslog(LOG_ERR,"-----<parse_worker shmget error/>-------"),exit(1);
  if((shmptr=(shmctrl*)shmat(shmid,0,0))==(shmctrl*)-1)
    syslog(LOG_ERR,"-----<parse_worker shmget error/>-------"),exit(1);
  if(signal(SIGALRM,sig_alrm)==SIG_ERR)
    syslog(LOG_ERR,"-----<parse_worker signal:sig_alrm error/>-------"),exit(1);
  char work_url[URL_SIZE];
  while(true){
    string formated_html="";
    pthread_mutex_lock(&(shmptr->html_lock));
    if(shmptr->html_head==shmptr->html_tail){
      pthread_mutex_unlock(&(shmptr->html_lock));
      alarm(2);
      pause();
      continue;
    }
    {
      unsigned int i=0;
      for(;i<URL_SIZE&&shmptr->html_queue[shmptr->html_head][i]!='\0';++i)
	work_url[i]=shmptr->html_queue[shmptr->html_head][i];
      if(i<URL_SIZE)
	work_url[i]='\0';
      for(i=URL_SIZE;i<HTML_SIZE&&shmptr->html_queue[shmptr->html_head][i]!='\0';++i)
	formated_html+=shmptr->html_queue[shmptr->html_head][i];
      shmptr->html_head=(shmptr->html_head+1)%HTML_NUMS;
    }
    pthread_mutex_unlock(&(shmptr->html_lock));
    GumboOutput*output=gumbo_parse(formated_html.c_str());
    set<string>links;
    string title="",contents="";
    find_title(output->root,title);
    get_links_contents(output->root,links,contents);
    gumbo_destroy_output(&kGumboDefaultOptions,output);
    links.erase(work_url);
    db_op->valuate_page(work_url,title,contents,formated_html,valuate_strategy);
    for(auto iter=links.begin();iter!=links.end();){
      string tmp_url=*iter;
      if(db_op->get_website_nice(tmp_url)==0||
	 db_op->if_exist("url","amigao.visited_url","url",tmp_url))
	iter=links.erase(iter);
      else
	++iter;
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
      size_t i=0;
      while(i<tmp_url.size()&&i<URL_SIZE&&tmp_url[i]!='\0'){
	shmptr->url_queue[shmptr->url_tail][i]=tmp_url[i];
	++i;	
      }
      if(i<URL_SIZE)
	shmptr->url_queue[shmptr->url_tail][i]='\0';
      links.erase(tmp_url);
      shmptr->url_tail=(shmptr->url_tail+1)%URL_NUMS;
      --url_insert_size;
    }
    pthread_mutex_unlock(&(shmptr->url_lock));
    if(links.empty())continue;
    FILE*fp=fopen(url_lists.c_str(),"a+");
    if(!fp){
      cerr<<"-------<parse_worker fopen error/>------"<<endl;
      continue;
    }
    while(!links.empty()){
      string tmp_str=*links.begin();
      links.erase(tmp_str);
      fputs((tmp_str+"\n").c_str(),fp);
    }
    fclose(fp);
  }
}
