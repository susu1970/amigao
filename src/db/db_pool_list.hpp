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

#ifndef AMIGAO_DB_DB_POOL_LIST_H_
#define AMIGAO_DB_DB_POOL_LIST_H_

namespace amigao{
  //the datastructure for database pool
  //all member functions are not thread safety
  class DBPoolList{
    //the node of database pool list
    class DBPoolNode{
      friend DBPoolList;
      void *connection;//database connection object
      explicit DBPoolNode(void*conn):connection(conn),next(nullptr){}
      DBPoolNode*next=nullptr;
    public:
      ~DBPoolNode(){
	connection=nullptr;
      }
    };
    int node_numbers=0;
    DBPoolNode*next=nullptr;
    void move_node(DBPoolList*dest,void*connection){
      if(!connection||dest->if_exist(connection)||!if_exist(connection))return;
      DBPoolNode *p=next,*q=nullptr;//work pointer
      for(;p&&p->connection!=connection;q=p,p=p->next);
      if(!p)return;
      if(!q){
	q=next;
	next=next->next;
	--node_numbers;
	dest->add_node(q);
	return;
      }
      q->next=p->next;
      --node_numbers;
      dest->add_node(p);
      return;
    }
  public:
    bool if_exist(void *connection){
      if(node_numbers==0)return false;
      if(!connection)return true;
      DBPoolNode* p=next;
      for(;p&&p->connection!=connection;p=p->next);
      return p;
    }
    bool if_exist(DBPoolNode*db_pool_node){
      if(node_numbers==0)return false;
      if(!db_pool_node||if_exist(db_pool_node->connection))return true;
      DBPoolNode* p=next;
      for(;p&&p!=db_pool_node;p=p->next);
      return p;
    }
    void add_node(DBPoolNode*db_pool_node){
      if(if_exist(db_pool_node)&&node_numbers>0)return;
      db_pool_node->next=next;
      next=db_pool_node;
      ++node_numbers;
      return;
    }
    void add_node(void *connection){//only deletable pointer can be passed!
      if(if_exist(connection))return;
      DBPoolNode*db_pool_node=new DBPoolNode(connection);
      add_node(db_pool_node);
      return;
    }
    int get_node_numbers(){
      return node_numbers;
    }
    void delete_node(void*connection){
      if(!if_exist(connection)||!connection)return;
      DBPoolNode*p=next,*q=nullptr;
      for(;p&&p->connection!=connection;q=p,p=p->next);
      if(!p)return;
      if(!q){
	q=next;
	next=next->next;
	delete q;
	q=nullptr;
	--node_numbers;
	return;
      }
      q->next=p->next;
      delete p;
      p=nullptr;
      --node_numbers;
    }
    void delete_node(DBPoolNode*db_pool_node){
      if(!if_exist(db_pool_node)||!db_pool_node)return;
      DBPoolNode*p=next,*q=nullptr;
      for(;p&&p!=db_pool_node;q=0,p=p->next);
      if(!p)return;
      if(!q){
	q=next;
	next=next->next;
	delete q;
	q=nullptr;
	--node_numbers;
	return;
      }
      q->next=p->next;
      delete p;
      p=nullptr;
      --node_numbers;
      return;
    }
    void delete_node(){//delete head
      delete_node(next);
    }
    void*get_connection(){
      if(!next)return nullptr;
      return next->connection;
    }
    void*pop_connection2(DBPoolList*dest){
      return pop_connection2(dest,get_connection());
    }
    void*pop_connection2(DBPoolList*dest,void*connection){
      if(!dest||dest==this)return connection;
      move_node(dest,connection);
      return connection;
    }
    ~DBPoolList(){
      if(node_numbers==0)
	return;
      DBPoolNode *p=next,*q=p->next;//work pointer
      while(q){
	delete p;
	p=q;
	q=q->next;
      }
      delete p;
    }
  };
}
#endif
