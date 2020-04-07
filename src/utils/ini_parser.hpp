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

#ifndef AMIGAO_UTILS_INI_PARSER_H_
#define AMIGAO_UTILS_INI_PARSER_H_

#include<string>
#include<unordered_map>
#include<fstream>
#include<fcntl.h>
#include<iostream>
#include<queue>

namespace amigao{
  class IniParser{
    std::string ini_file_path="";
    std::string context="";
    std::unordered_map<std::string,std::string>result;
  public:
    //specify an xxx.ini file to initialize IniParser
    IniParser(std::string ini_file_path_){
      init_from_file(ini_file_path_);
    }
    IniParser(){
      
    }
    void init_from_file(std::string ini_file_path_){
      if(ini_file_path_.empty()){
	std::cerr<<"IniParser::init_from_file(std::string) argument size is zero"<<std::endl;
	return;	
      }
      std::ifstream ifs(ini_file_path_);
      if(!ifs.is_open()){
	std::cerr<<"IniParser::init_from_file(std::string) ifstream open "<<ini_file_path_<<" error"<<std::endl;
	return;
      }
      context="";//clear context
      //locale area
      {
	std::string tmp_str;
	while(std::getline(ifs,tmp_str)){
	  if(!tmp_str.empty()&&tmp_str[0]!='#')//'#' represent comments
	    context+=(tmp_str+"\n");
	}
      }//tmp_str die here
      ifs.close();
      parse();
    }
    void init_from_string(std::string ini_string_){
      context=ini_string_;
      parse();
    }
    void parse(){
      parse(context);
    }
    void parse(const std::string ini_string_){
      using namespace std;
      //locale area
      queue<string>line_queue;
      {
	size_t i=0,j=0;//work subscript of ini_string_
	while(i<ini_string_.size()){
	  //to push a line-based string into line_queue
	  for(j=i;j<ini_string_.size()&&ini_string_[j]!='\n';++j);
	  line_queue.push(ini_string_.substr(i,j-i));
	  i=j+1;
	}
	while(!line_queue.empty()){
	  //locale area
	  {
	    string tmp_str=line_queue.front();//not contains '\n'
	    line_queue.pop();
	    for(i=0;i<tmp_str.size()&&
		  (tmp_str[i]==' '||
		   tmp_str[i]=='\t');++i);//filte blank or other meaningless character
	    for(j=i;j<tmp_str.size()&&
		  tmp_str[j]!=' '&&
		  tmp_str[j]!='\t'&&
		  tmp_str[j]!='=';++j);//get the attribute part
	    if(i==tmp_str.size()||j==tmp_str.size())continue;
	    string attr=tmp_str.substr(i,j-i);
	    while(j<tmp_str.size()&&
		  tmp_str[j]!='=')++j;
	    if(j==tmp_str.size()){
	      continue;
	    }
	    ++j;
	    while(j<tmp_str.size()&&
		  (tmp_str[j]==' '||
		   tmp_str[j]=='\t'))++j;//filte meaningless prefix of value string
	    for(i=j;j<tmp_str.size()&&
		  tmp_str[j]!=' '&&
		  tmp_str[j]!='\t';++j);//get the value part
	    if(i<tmp_str.size()){
	      if(j>i)
		result.insert(std::make_pair(attr,tmp_str.substr(i,j-i)));//insert into result<attribute,value>
	      else
		result.insert(std::make_pair(attr,""));
	    }	      
	  }
	}
      }
    }
    std::unordered_map<std::string,std::string>get_result(){
      return result;
    }
    std::string get_value(std::string key_){
      return result[key_];
    }
  };
}
#endif
