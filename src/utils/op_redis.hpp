#include<sstream>
#include<vector>
#include<iostream>
#include<stdio.h>
#include<string>
#include<string.h>
#include<hiredis/hiredis.h>

#include"utils/ini_parser.hpp"

namespace amigao{
  class Redis{
    redisContext*redis_context;
    redisReply*redis_reply;
  public:
    Redis(std::string redis_conf_path){
      using namespace std;
      IniParser ini_parser(redis_conf_path);
      string host=ini_parser.get_value("HOST");
      int port=atoi(ini_parser.get_value("PORT").c_str());
      string passwd=ini_parser.get_value("PASSWD");
      connect(host,port,passwd);
    }
    ~Redis(){
      if(redis_context)
	redisFree(redis_context);
    }
    int set_string(std::string key,std::string value){
      if(!(redis_reply=(redisReply*)redisCommand(redis_context,"SET %s %s",key.c_str(),value.c_str()))){
	std::cerr<<"---<Redis set_string error/>---"<<std::endl;
	exit(1);
      }
    }
  
    std::string get_string(std::string key){
      if(!(redis_reply=(redisReply*)redisCommand(redis_context,"GET %s",key.c_str()))||redis_reply->len<=0){
	std::cerr<<"---<Redis get_string error/>---"<<std::endl;
	exit(1);
      }
      std::stringstream ss;
      ss<<redis_reply->str;
      return ss.str();
    }
    int set_list(std::string key,std::vector<int>value){
      for(size_t i=0;i<value.size();++i){
	if(!(redis_reply=(redisReply*)redisCommand(redis_context,"RPUSH %s %d",key.c_str(),value[i]))){
	  std::cerr<<"---<Redis set_list error/>---"<<std::endl;
	  exit(1);
	}
      }
    }
    std::vector<int>get_list(std::string key){
      redis_reply=(redisReply*)redisCommand(redis_context,"LLEN %s",key.c_str());
      size_t len=redis_reply->integer;
      redis_reply=(redisReply*)redisCommand(redis_context,"LRANGE %s %d %d",key.c_str(),0,len-1);
      redisReply**reply_vec=redis_reply->element;
      std::vector<int>result;
      for(size_t i=0;i<len;++i){
	std::string temp=(*reply_vec)->str;
	int a=atoi(temp.c_str());
	result.push_back(a);
	++reply_vec;
      }
      return result;
    }
    void connect(std::string host,int port,std::string passwd=""){
      if((redis_context=redisConnect(host.c_str(),port))->err){
	redisFree(redis_context);
	std::cerr<<"---<redisConnect error/>---"<<std::endl;
	exit(1);
      }
    }
  };
 
}
