//
//  GameLayerw.h
//  Pinle
//
//  Created by 李辛 on 15/6/1.
//
//

#ifndef __Pinle__GameLayerw__
#define __Pinle__GameLayerw__

#include "../../framework/config.h"
#include "../IRoomData.h"
#include "../module/DebrisSprite.h"
#include "../../player/IPlayerData.h"
#include "../../gameini/GameIni.h"
#include "OverLayer.h"


#define PICTURE_WIDTH       750     //拼图宽度
#define PICTURE_HEIGTH      1000    //拼图高度
#define PICTURE_CLEARANCE   2       //图片间隙大小
#define BASE_POINT_X    36 + 2          //拼图拼盘基点x
#define BASE_POINT_Y    903+53 + 68 + 5        //拼图拼盘基点y
#define PLAY_AREA_WIDTH     677     //游戏区域宽度
#define PLAY_AREA_HEIGHT    903     //游戏区域高度

#define btn_back_reward    72
#define btn_close_reward 66
#define btn_changeinfo_reward   80
#define btn_voice_room  132
#define btn_check_room  131
#define btn_back_room   130

#define mesbox_back_room 1001
#define mesbox_check_room 1002
#define mesbox_reward_room 1003

#define check_picture_gold "check_picture_gold"    //纪录是否查看原图，扣除金币
#define change_reward_info "change_reward_info"     //提示修改个人信息



enum SwapType
{
    swap_none = 0,  //不做操作
    swap_touch,     //点击交换
    swap_move,      //拖动交换
    swap_select,    //选中一个碎片，设置高亮
    swap_cancel,    //取消一个碎片的选中
    swap_onlycancel //只是取消高亮选中
};

class GameLayerw : public IUILayer
{
public:
    int _time;
    GameLayerw();
    ~GameLayerw();
    IRoomData *iroomData;
    IPlayerData *iplayerData;
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
//    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameLayerw);
    
    
    virtual bool Notify(const string& name, Args& args);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event) ;
    virtual void onTouchEnded(Touch *touch, Event *unused_event) ;
    
public:
    //----------步数动画
    Armature *armature_gewei;//步数个位
    Armature *armature_shiwei;//步数十位
    void animationInit();
    
    //----------界面UI
    int shiwei_JiLu;    //步数动画纪录的十位数
    Node* rootNode;
    bool xianshi_WanZhengTu;    //是否显示完整图
    Layer *pSplitBoard;         //存放碎片的层
    Sprite * m_pSprite;       //
    int chanKanCiShu_jilu; //可以查看的次数
    Label *checkPictureLabel;   //label渲染
    Sprite *pWanCheng_Sprite;
    Text *_timeText;       //分
    Text *stepNumLabel;     //步数
    LoadingBar *gradeBar;       //等级进度条
    
    //---游戏
    int iSwapMode; //交换模式
    int iMoveCount; //移动步数
    int iSelectClickIndex;  //点击选择的碎片编号,判断在
    int iSelectDebrisIndex; //纪录点击一次选中的碎片
    int iRowCount;      //行数
    int iColCount;      //列数
    bool bSelectedNow;    //是否存在高亮碎片
    string ssrcJpgName;     //拼图名称
    GameLevelSprite *gameLevelSprite;   //拼图信息
    int iDebrisWidth;       //碎片宽度
    int iDebrisHeight;      //碎片高度
    float fScaleX;          //碎片X缩放比例
    float fScaleY;          //碎片Y缩放比例
    float fHalfDebrisWidth; //碎片宽度的1/2
    float fHalfDebrisHeight;    //碎片高度的1/2
    int debrisIndex[100];      //碎片顺序
    long lgameStartTime;          //游戏开始时间
    int iGameState; //游戏状态:0未开始 1倒计时 2游戏中 3结束
    Vec2 TPoffsetWithDS;    //触摸按下时相对于被选中碎片中点的位置
    Vec2 touchdownP;        //触摸按下的位置
    long loneStepTime;      //纪录每一步的时间
    map<int, long> timeMap; //保存每一步的时间
    

public:
    void setGameUI(int _type);  //初始化游戏界面
    void pvpGameUpdate(float ft);   //对战轮训定时器
    void gameBack(Ref *pSender,Widget::TouchEventType type);    //返回，推兔改场景
    void checkPicture(Ref *pSender,Widget::TouchEventType type);    //查看完整图方法
    void gameVoice(Ref *pSender,Widget::TouchEventType type);   //游戏声音
    void initSploitBord();  //初始化拼盘
    void gameStart();   //游戏开始，倒计时
    void gameTimeDowncount();   //游戏倒计时
    void openMusic321();    //声音延迟播放
    int  effectID;//音效编号
    void randomMoveDebris(Node *psender, int count);    //打乱碎片顺序
    void setMorePersonDebris();     //多人对战碎片打乱
    void updateGameTime(float ft);  //游戏倒计时
    int checkTouchPoint(Vec2 _touchP, bool isTouchDown);     //检测点击的焦点
    int getTouchDebris(Vec2 _touchP);       //获取点击碎片
    void swapDebris(int srcIndex, int desIndex, int swapType);  //交换图片
    void touchMoveDebris(Vec2 touchP, DebrisSprite *DS);    //移动碎片
    void updateStepNum();       //更新拼图步数
    void callbackN(Node *sender);   //游戏结算回调
    bool checkWin();            //检测游戏是否成功
    void btnTouchEnd(Ref *psender,Widget::TouchEventType type); //按钮触发事件
    void showTimeForPersonal();     //显示个人赛时间
    void showTimeForOther();        //显示其他比赛时间
    
    void llss(Node *psender,int d){};
    
    
    
    void Split_scale_callbackN0(float dt);
//    void Split_scale_callbackN1(Node* sender);
//    void Split_scale_callbackN2(Node* sender);
//    void Split_scale_callbackN3(Node* sender);
//    void Split_scale_callbackN4(Node* sender);
//    void Split_scale_callbackN5(Node* sender);
//    void Split_scale_callbackN6(Node* sender);
    
    
    void initActivityOver();    //活动结束初始化
    void reloadActivityInfo();  // 重新加载邮寄信息
    void updatePlayerInfo();    //更新玩家最新信息
    void showGuide();           //显示引导
    
private:
    Text *addressInfo;
    Text *phoneInfo;
    Text *nameInfo;
    
    UIMessage* messageBox;
    Sprite *voiceSprite;
    bool voiceOL;
    Button *gameBackBtn;
    Button *checkPictureBtn;
    Button* gameVoiceBtn;
    
    Sprite *headSp;     //玩家头像
    Text *playerNick;//玩家昵称
    Text *playerGold;//玩家金币
    Text *playerRank;//玩家等级
    
    bool detail_reward;     //判断了解详情是否给奖励了
    Node *gameOverNode;
    bool person_change_fail; //判断个人挑战是否胜利
    bool ar_push;       //提交活动完成
    OverLayer *overLayer;       //结束页面
    Args _args;
};



#endif /* defined(__Pinle__GameLayerw__) */
