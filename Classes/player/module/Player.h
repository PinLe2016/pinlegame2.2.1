//
//  Player.h
//  PinLe
//
//  Created by Kennedy on 15/6/30.
//
//

#ifndef __PinLe__Player__
#define __PinLe__Player__

#include "cocos2d.h"
using namespace std;
class Player
{
public:
    void  setloginname(const string & str);
    string getloginname();
    Player();
    ~Player();
    int   cycle ;  //循环数
    int   _points;  //积分
    int   lookawardorder;
    int   lookawardcount;
    string   lookgoodsname;
    string   lookgoodsimageurl;
    int      Winningrank;//获奖名次
    int      Winningpoints;//积分
    string   Winningnickname;//玩家昵称
    string   Winningheadimageurl;//头像
    string   Winninggoodsname;//奖品名称
    string   Winninggoodsimageurl;//奖品图片
    string          userid;                 //  用户登录ID号
    string          userpsw;                //  用户密码
    string          nickname;               //	昵称	String
    string          email;                  //	邮箱	String
    string          mobile;                 //  手机	String
    string          registertime;           //  注册时间	String
    string          description;            //  备注	String
    string          idnumber;               //  身份证号	String
    string          firstname;              //  名	String
    string          lastname;               //  姓	String
    string          fullname;               //  全名	String
    string          company;                //  公司	String
    string          department;             //  部门	String
    string          duty;                   //  职务	String
    string          title;                  //  称呼	String
    string          countryname;            //	国家	String
    string          provincename;           //  省份	String
    string          cityname;               //  城市	String
    string          _invitecode;
    int            err_code;
    string          postalcode;             //  邮编	String
    string          address;                //  地址	String
    string          birthday;               //  生日	String
    string          fax;                    //  传真	String
    string          imageurl;               //  头像图片Url	String
    string          childrencount;          //  子女数量	String
    string          personalincome;         //  个人收入	String
    string          familyincome;           //  家庭收入	String
    string          industryname;           //  所属行业	String
    string          vocationname;           //  职业名称	String
    string          postname;               //  收货人姓名	String
    string          postphone;              //  收货人电话	String
    string          postprovincename;       //  收货人省份名称	String
    string          postcityname;           //  收货人城市名称	String
    string          postaddress;            //  收货人地址	String
    string          rankname;               //  等级称号
    string          nextrankname;           //  下一等级称号
    
//    string          recordgrade;            //  等级记录
    string          uHeadIndex;             //  头像地址
    string          myFriendId;               //我邀请的好友id  userid
    
    int             prizewinning;           //  玩家是否中奖，中的几等奖
    int             identity;               //  用户身份 0为游客 1为正式
    int             grade;                  //  等级
    int             points;                 //  积分
    int             pointscur;              //  当前等级积分数量
    int             pointslimit;            //  下一等级积分数量
    int             totalpoints;            //  总积分
    int             todaypoints;            //  今日获得积分
//    int             friendspoints;          //  还有积分收益
    int             golds;                  //	金币
    int             tempAddGolds;           //  临时增加金币的数量，比如查看详情增加的金币数量
    int             goldslimit;             //  每日金币上限
    int             totalgolds;             //  获得的总金币量
    int             todaygolds;             //  今日获得金币
    int             gamegolds;              //  拼图获得的总金币量
//    int             friendsgolds;           //  好友带来的金币量
    int             gender;                 //  性别
    int             ismarried;              //  婚否
    int             postprovinceid;         //  收货人省份编号
    int             provinceid;             //  当前所在省编号
    int             postcityid;             //  收货人城市编号
    int             cityid;                 //  当前所在市编号
    int             headIndex;              //  头像编号
    
    int             goodsnum;               //  商品个数
    int             lotterynum;             //  奖券个数
    int             ungetPoints;            //  当前好友提供的未领取积分
    
    int             friendsTotalPoints;          // 好友产生的总积分
    int             friendsTotalGolds;           // 好友产生的总金币
    int             friendspoints;              // 好友产生的积分(未领)
    int             friendsgolds;               // 好友产生的金币(未领)
    int            _isexsits;                  //昵称是否存在
    string          _nickname;                  //建议的昵称
    
    
    
    
    void            copy(Player* p);
    bool            compare(Player* p);
};

#endif /* defined(__PinLe__Player__) */
