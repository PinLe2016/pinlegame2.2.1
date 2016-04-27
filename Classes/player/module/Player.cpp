//
//  Player.cpp
//  PinLe
//
//  Created by Kennedy on 15/6/30.
//
//

#include "Player.h"

static string _loginname;
void  Player::setloginname(const string & str){
    _loginname=str;
}
string Player::getloginname(){
    return _loginname;
}
Player::Player()
{
    userid  = "";
    userpsw = "";
    identity = 1;
    
    headIndex = -1;
    gender = -1;
    
    postprovinceid = -1;
    postcityid = -1;
    provinceid = -1;
    cityid = -1;
    
    postphone = "";
    nickname = "";
    postname = "";
    birthday = "";
    postprovincename = "";
    postcityname = "";
    postaddress = "";
    myFriendId = "";
    cityname = "";
    
    friendsTotalPoints = 0;
    friendsTotalGolds = 0;
    friendspoints = 0;
    friendsgolds = 0;
    
    golds = 0;

}

Player::~Player()
{
    
}

//void Player::setInit()
//{
//    userid  = "";
//    userpsw = "";
//    identity = 1;
//    
//    headIndex = -1;
//    gender = -1;
//    
//    postprovinceid = -1;
//    postcityid = -1;
//    provinceid = -1;
//    cityid = -1;
//    
//    postphone = "";
//    nickname = "";
//    postname = "";
//    birthday = "";
//    postprovincename = "";
//    postcityname = "";
//    postaddress = "";
//}


void Player::copy(Player* p)
{
    if(!p)
        return;
    headIndex = p->headIndex;
    uHeadIndex = p->uHeadIndex;
    gender = p->gender;
    postprovinceid = p->postprovinceid;
    postcityid = p->postcityid;
    provinceid = p->provinceid;
    provincename = p->provincename;
    cityid = p->cityid;
    cityname = p->cityname;
    postphone = p->postphone;
    nickname = p->nickname;
    postname = p->postname;
    birthday = p->birthday;
    postprovincename = p->postprovincename;
    postcityname = p->postcityname;
    postaddress = p->postaddress;
    myFriendId = p->myFriendId;
}



bool Player::compare(Player* p)
{
    if(!p)
        return false;
    if(headIndex != p->headIndex)                   return false;
//    if(uHeadIndex.compare(p->uHeadIndex) != 0)      return false;
    if(gender != p->gender)                         return false;
    if(postprovinceid != p->postprovinceid)         return false;
    if(postcityid != p->postcityid)                 return false;
    if(provinceid != p->provinceid)                 return false;
    if(cityid != p->cityid)                         return false;
    if(postphone.compare(p->postphone) != 0)        return false;
    if(nickname.compare(p->nickname) != 0)          return false;
    if(postname.compare(p->postname) != 0)          return false;
    if(birthday.compare(p->birthday) != 0)          return false;
//    if(postprovincename.compare(p->postprovincename) != 0)
//        return false;
//    if(postcityname.compare(p->postcityname) != 0)  return false;
    if(postaddress.compare(p->postaddress) != 0)    return false;
    if(myFriendId.empty())
    {
        if(p->myFriendId.empty())
        {
            return false;
        }
    }
    return true;
}




















