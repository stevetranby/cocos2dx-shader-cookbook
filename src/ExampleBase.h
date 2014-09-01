//
//  ExampleBase.h
//  cocos2d-shader-cookbook
//
//  Created by Steve Tranby on 8/31/14.
//
//

#ifndef __cocos2d_shader_cookbook__ExampleBase__
#define __cocos2d_shader_cookbook__ExampleBase__

#include "cocos2d.h"

struct ShaderInfo
{
    std::string name;
    std::string title;
    std::string texture1;
    std::string texture2;
};

class ExampleBase : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* scene();
    virtual bool init();
    CREATE_FUNC(ExampleBase);

    void refreshShader();

protected:
    cocos2d::Label* sourceLabel;
    cocos2d::Sprite* _exampleSprite;
    float _time;
    int _currentIndex;
    std::vector<ShaderInfo> _examples;
};

#endif /* defined(__cocos2d_shader_cookbook__ExampleBase__) */
