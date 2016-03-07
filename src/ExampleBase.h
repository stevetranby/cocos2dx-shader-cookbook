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
#include <CocosGUI.h>

#define kLabelTagName "valueLabel"

struct ShaderInfo
{
    std::string title;
    std::string fragmentShader;
    std::string vertexShader;
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
    void setupExamples();
    void setupGUI();
    void setupExampleBaseGUI();

    //
    void setupSliderWithName(const std::string& name);

    //
    void setupExample2();

protected:
    cocos2d::Label* _titleLabel;
    cocos2d::Sprite* _exampleSprite;
    float _time;
    int _currentIndex;
    std::vector<ShaderInfo> _examples;

    // GUI
    cocos2d::ui::Slider* _slider1;
    cocos2d::ui::Slider* _slider2;
    cocos2d::ui::Slider* _slider3;

    // State
};

#endif /* defined(__cocos2d_shader_cookbook__ExampleBase__) */
