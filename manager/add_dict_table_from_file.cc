#include"db/sql_db_operation.hpp"
#include<string>
#include<cstring>
#include<iostream>

using namespace std;

int main(int argc,char**argv){
  if(argc!=4)
    cerr<<"usage: <"<<argv[0]<<"> <db conf path> <dict name> <dict file>"<<endl,exit(1);
  string db_conf_path=argv[1];
  string dict_name=argv[2];
  string dict_path=argv[3];
  amigao::DBOperationInterface*db_op=new amigao::SqlDBOperation(db_conf_path);
  db_op->add_dict_table(dict_name,dict_path);
  delete db_op;
  cout<<"add dict table completed!"<<endl;
}
