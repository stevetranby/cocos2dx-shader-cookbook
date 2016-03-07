//
//  ExampleBase.cpp
//  cocos2d-shader-cookbook
//
//  Created by Steve Tranby on 8/31/14.
//
//

#include "ExampleBase.h"

USING_NS_CC;
using namespace cocos2d::ui;

//+(NSArray *)examples
//{
//    NSMutableArray *arr = [NSMutableArray array];
//
//    int count = objc_getClassList(NULL, 0);
//    Class classes[count];
//    objc_getClassList(classes, count);
//
//    for(int i=0; i<count; i++){
//        Class klass = classes[i];
//        if(class_getSuperclass(klass) == self){
//            [arr addObject:klass];
//        }
//    }
//
//    return [arr sortedArrayUsingComparator:^NSComparisonResult(Class klass1, Class klass2) {
//            return [NSStringFromClass(klass1) compare:NSStringFromClass(klass2)];
//            }];
//}

Scene* ExampleBase::scene()
{
    auto scene = Scene::create();
    auto layer = ExampleBase::create();
    scene->addChild(layer);
    return scene;
}

bool ExampleBase::init()
{
    if(! LayerColor::initWithColor(Color4B::WHITE)) {
        return false;
    }

    auto prev = Text::create("Prev", "Helvectica", 48);
    prev->setColor(Color3B::BLACK);
    prev->setNormalizedPosition(Vec2(0.25, 0.1));
    prev->setTouchScaleChangeEnabled(true);
    prev->setTouchEnabled(true);
    prev->addTouchEventListener([this](Ref* senderRef, Widget::TouchEventType eventType) {
        switch(eventType) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                CCLOG("ended");
                // take action here
                _currentIndex--;
                if(_currentIndex < 0) {
                    _currentIndex = _examples.size() - 1;
                }
                this->refreshShader();
                break;
            }
            default:
                break;
        }
    });
    this->addChild(prev);

    auto next = Text::create("Next", "Helvectica", 48);
    next->setColor(Color3B::BLACK);
    next->setNormalizedPosition(Vec2(0.75, 0.1));
    next->setTouchScaleChangeEnabled(true);
    next->setTouchEnabled(true);
    next->addTouchEventListener([this](Ref* senderRef, Widget::TouchEventType eventType) {
        switch(eventType) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                CCLOG("ended");
                // take action here
                _currentIndex++;
                if(_currentIndex >= _examples.size()) {
                    _currentIndex = 0;
                }
                refreshShader();
                break;
            }
            default:
                break;
        }
    });
    this->addChild(next, 10);

    //    auto exampleContent = getExampleContent();
    //    exampleContent->setNormalizedPosition(Vec2(0.5, 0.5));
    //    this->addChild(_exampleContent);

    _exampleSprite = Sprite::create("res/Logo-hd.png");
    _exampleSprite->setNormalizedPosition(Vec2(0.5, 0.5));
    _exampleSprite->setAnchorPoint(Vec2(0.5,0.5));
    _exampleSprite->setColor(Color3B(Color4F(1.0,1.0,0.5,1.0)));
    this->addChild(_exampleSprite);

    _titleLabel = Label::createWithSystemFont("Example Name", "Arial", 18);
    _titleLabel->setColor(Color3B::BLACK);
    _titleLabel->setNormalizedPosition(Vec2(0.5, 0.9));
    this->addChild(_titleLabel);


    _currentIndex = 0;
    setupExamples();
    setupGUI();
    refreshShader();

    return true;
}

void ExampleBase::refreshShader()
{
    if(_currentIndex < 0 || _currentIndex >= _examples.size()) {
        return;
    }

    _exampleSprite->setTexture("res/Logo-hd.png");

    auto shaderInfo = _examples.at(_currentIndex);
    _titleLabel->setString(shaderInfo.title);

    // REFERENCE
    // Load the fragment shader in the file name E01_SimplestShader.fsh.
    // Also loads E01_SimplestShader.vsh as the vertex shader if it exists.
    //sprite.shader = [CCShader shaderNamed:self.shaderName];


    // TODO: use a default template shader if none specified
    auto vertFilename = std::string("res/shaders/").append(shaderInfo.vertexShader).append(".vsh");
    auto fragFilename = std::string("res/shaders/").append(shaderInfo.fragmentShader).append(".fsh");

    CCLOG("refresh shader with files: %s, %s", vertFilename.c_str(), fragFilename.c_str());

    // Load the fragment shader in the file name E01_SimplestShader.fsh.
    // Also loads E01_SimplestShader.vsh as the vertex shader if it exists.
    auto glprogram = GLProgram::createWithFilenames(vertFilename, fragFilename);
    GLProgramState* glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);

    auto winsize = Director::getInstance()->getWinSizeInPixels();
    glprogramstate->setUniformVec2("cc_ViewSizeInPixels", winsize);

    if(! shaderInfo.texture2.empty()) {
        // add extra texture
        auto textureCache = Director::getInstance()->getTextureCache();
        auto tex1 = textureCache->getTextureForKey(shaderInfo.texture1);
        if(! tex1) {
            tex1 = textureCache->addImage(shaderInfo.texture1);
        }
        if(tex1) {
            // TODO: should store in shaderinfo
            _exampleSprite->setTexture("res/Rocks-hd.png");
            // TODO: test trying to use default Texture1
            // TODO: convert shader and ShaderInfo property to u_Custom_Texture1
            glprogramstate->setUniformTexture("u_CausticTexture", tex1);
        }

        // 2nd Extra Texture
        auto tex2 = textureCache->getTextureForKey(shaderInfo.texture2);
        if(! tex2) {
            tex2 = textureCache->addImage(shaderInfo.texture2);
        }
        if(tex2) {
            Texture2D::TexParams texParams = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
            tex2->setTexParameters(texParams);

            // TODO: convert shader and ShaderInfo property to Custom_Texture2 or u_Noise_Texture
            auto locid = glprogram->getUniformLocationForName("uNoiseTextureSize");
            glprogram->setUniformLocationWith2f(locid, 30, 30);
            glprogramstate->setUniformVec2("u_NoiseTextureSize", tex2->getContentSizeInPixels());
            glprogramstate->setUniformTexture("u_NoiseTexture", tex2);
        }

    }

    // attach the ProgramState to a Node
    _exampleSprite->setGLProgram(glprogram);
}

void ExampleBase::setupGUI()
{
//    auto frameCache = SpriteFrameCache::getInstance();
//    frameCache->addSpriteFramesWithFile("interface-hd.plist");

    auto sliderBar = "res/slider-background-hd.png";
    auto sliderFilename = "res/slider-handle-hd.png";

    for(int i=0; i<3; i++) {

        auto sliderName = StringUtils::format("slider_%d", i);
        auto slider = ui::Slider::create();
        slider->loadBarTexture(sliderBar);

        slider->loadSlidBallTextures(sliderFilename, sliderFilename, sliderFilename);
        slider->setScale9Enabled(true);
        slider->setContentSize(Size(200,30));
        slider->setNormalizedPosition(Vec2(0.5, 0.25 + 0.1 * i));
        slider->setName(sliderName);

        auto lbl = Label::create("[name]", "Helvetica", 12);
        lbl->setColor(Color3B::BLACK);
        lbl->setAnchorPoint(Vec2(1.0, 0.5));
        lbl->setNormalizedPosition(Vec2(-0.1, 0.5));
        lbl->setName(kLabelTagName);
        slider->addChild(lbl);

        auto grad = LayerGradient::create(Color4B::WHITE, Color4B::BLACK, Vec2(-1, 0));
        grad->setContentSize(Size(200,40));
        slider->addChild(grad, -1);

        slider->addEventListener([this](Ref* senderRef, Slider::EventType eventType) {
            auto slider = static_cast<ui::Slider*>(senderRef);
            auto sliderLabel = static_cast<Label*>(slider->getChildByName(kLabelTagName));
            if(sliderLabel) {
                float alpha = slider->getPercent() / 100.0f;
                float value = (1.0f - alpha) * slider->getPercent()/100.0f + alpha * slider->getPercent()/100.0f;
                sliderLabel->setString(StringUtils::format("%.2f", value));
                CCLOG("alpha = %f, eventType = %d", alpha, eventType);
            }
        });
        this->addChild(slider);
        
        if(i == 0) _slider1 = slider;
        if(i == 1) _slider2 = slider;
        if(i == 2) _slider3 = slider;
    }
}

//void ExampleBase::setupSliderWithName(const std::string& name)
//{
//    if((self = [super initWithName:name])){
//        self.startColor = [CCColor whiteColor];
//        self.endColor = [CCColor blackColor];
//
//        _gradient = [CCNodeGradient nodeWithColor:self.startColor fadingTo:self.endColor alongVector:ccp(-1, 0)];
//        _gradient.contentSizeType = CCSizeTypeNormalized;
//        _gradient.contentSize = CGSizeMake(1.0, 1.0);
//
//        [self addChild:_gradient z:-1];
//    }
//
//    return self;
//}
//
//void ExampleBase::callback(CCSlider *slider)
//{
//    if(_colorBlock) _colorBlock([_startColor interpolateTo:_endColor alpha:self.sliderValue]);
//}
//
//void ExampleBase::setStartColor:(CCColor *startColor)
//{
//    _startColor = startColor;
//    [self callback:self];
//    _gradient.startColor = startColor;
//    _gradient.startOpacity = startColor.alpha;
//}
//
//void ExampleBase::setEndColor:(CCColor *endColor)
//{
//    _endColor = endColor;
//    [self callback:self];
//    _gradient.endColor = endColor;
//    _gradient.endOpacity = endColor.alpha;
//}
//
//void ExampleBase::setupExample2()
//{
//    ColorSlider *slider = [ColorSlider sliderNamed:@"Tint Color"];
//    slider.positionType = CCPositionTypeNormalized;
//    slider.position = ccp(0.5, 0.25);
//    slider.anchorPoint = ccp(0.5, 0.5);
//    slider.endColor = [CCColor magentaColor];
//    slider.colorBlock = ^(CCColor *color){sprite.color = color;};
//
//    CCLayoutBox *content = [CCLayoutBox node];
//    content.anchorPoint = ccp(0.5, 0.5);
//    content.direction = CCLayoutBoxDirectionVertical;
//
//    [content addChild:slider];
//    [content addChild:sprite];
//}
//
//void ExampleBase::setupExample7()
//{
//    ColorSlider *flashSlider = [ColorSlider sliderNamed:@"Flash Color"];
//    flashSlider.startColor = [CCColor colorWithRed:1 green:0 blue:0 alpha:0];
//    flashSlider.endColor = [CCColor redColor];
//    flashSlider.colorBlock = ^(CCColor *color){sprite.shaderUniforms[@"u_ColorFlash"] = color;};
//
//    ColorSlider *tintSlider = [ColorSlider sliderNamed:@"Tint Color"];
//    tintSlider.endColor = [CCColor colorWithRed:1 green:1 blue:1 alpha:0];
//    tintSlider.colorBlock = ^(CCColor *color){sprite.colorRGBA = color;};
//
//    CCLayoutBox *content = [CCLayoutBox node];
//    content.anchorPoint = ccp(0.5, 0.5);
//    content.direction = CCLayoutBoxDirectionVertical;
//
//    [content addChild:tintSlider];
//    [content addChild:flashSlider];
//    [content addChild:sprite];
//}
//void ExampleBase::setupExample8()
//{
//    olorSlider *colorSlider = [ColorSlider sliderNamed:@"Outline Color"];
//    colorSlider.startColor = [CCColor colorWithRed:0 green:0 blue:1 alpha:1];
//    colorSlider.endColor = [CCColor colorWithRed:0 green:0 blue:1 alpha:0];
//    colorSlider.colorBlock = ^(CCColor *color){sprite.colorRGBA = color;};
//
//    FloatSlider *widthSlider = [FloatSlider sliderNamed:@"Outline Width"];
//    widthSlider.endValue = 3.0;
//    widthSlider.sliderValue = 1.0;
//    widthSlider.valueBlock = ^(float value){sprite.shaderUniforms[@"u_OutlineWidth"] = @(value);};
//
//    CCLayoutBox *content = [CCLayoutBox node];
//    content.anchorPoint = ccp(0.5, 0.5);
//    content.direction = CCLayoutBoxDirectionVertical;
//
//    [content addChild:widthSlider];
//    [content addChild:colorSlider];
//    [content addChild:sprite];
//}
//
//void ExampleBase::setupExample9()
//{
//    CCTexture *noise = [CCTexture textureWithFile:@"gaussianNoise.png"];
//    noise.texParameters = &(ccTexParams){GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
//    sprite.shaderUniforms[@"u_NoiseTexture"] = noise;
//
//    FloatSlider *noiseSlider = [FloatSlider sliderNamed:@"Amount"];
//    noiseSlider.sliderValue = 1.0;
//    noiseSlider.valueBlock = ^(float value){sprite.shaderUniforms[@"u_NoiseAmount"] = [NSNumber numberWithFloat:value];};
//
//    CCLayoutBox *content = [CCLayoutBox node];
//    content.anchorPoint = ccp(0.5, 0.5);
//    content.direction = CCLayoutBoxDirectionVertical;
//
//    [content addChild:noiseSlider];
//    [content addChild:sprite];
//}
//
//void ExampleBase::setupExample10()
//{
//    sprite.shaderUniforms[@"u_CausticTexture"] = [CCTexture textureWithFile:@"Caustic.png"];
//
//    CCTexture *noise = [CCTexture textureWithFile:@"BisectionNoise.png"];
//    noise.texParameters = &(ccTexParams){GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
//    sprite.shaderUniforms[@"u_NoiseTexture"] = noise;
//}
//void ExampleBase::setupExample11()
//{
//    // Load the distortion texture, a noise texture which we use to determine how to offset individual fragments when we draw them.
//    CCTexture* noise = [CCTexture textureWithFile:@"gaussianNoise.png"];
//    // Nearest neighboor interpolating to create a pixely effect out of the distortion texture.
//    noise.texParameters = &(ccTexParams){GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
//
//    sprite.shaderUniforms[@"u_NoiseTexture"] = noise;
//
//    FloatSlider *blurSlider = [FloatSlider sliderNamed:@"Radius"];
//    blurSlider.endValue = 10.0f;
//    blurSlider.sliderValue = 0.5;
//    blurSlider.valueBlock = ^(float value){sprite.shaderUniforms[@"u_Radius"] = @(value);};
//
//    FloatSlider *animationSlider = [FloatSlider sliderNamed:@"Animation"];
//    animationSlider.sliderValue = 1.0;
//    // Round the number so it's on or off. 0 will disable the animation, 1 will enable it.
//    animationSlider.valueBlock = ^(float value){sprite.shaderUniforms[@"u_AnimationEnabled"] = @(roundf(value));};
//
//    FloatSlider *blockSizeSlider = [FloatSlider sliderNamed:@"BlockSize"];
//    blockSizeSlider.startValue = 1.0f;
//    blockSizeSlider.endValue = 256.0f;
//    blockSizeSlider.valueBlock = ^(float value){sprite.shaderUniforms[@"u_BlockSize"] = @(floorf(value));};
//
//    CCLayoutBox *content = [CCLayoutBox node];
//    content.anchorPoint = ccp(0.5, 0.5);
//    content.direction = CCLayoutBoxDirectionVertical;
//
//    [content addChild:blockSizeSlider];
//    [content addChild:animationSlider];
//    [content addChild:blurSlider];
//    [content addChild:sprite];
//}
//
//void ExampleBase::setupExample12()
//{
//    float _colorRotation;
//    float _colorScale;
//    float _saturationAdjustment;
//    CCSprite *_sprite;
//
//
//    _colorRotation = 0.0f;
//    _colorScale = 1.0f;
//    _saturationAdjustment = 1.0f;
//
//    _sprite = [CCSprite spriteWithImageNamed:@"Logo.png"];
//    _sprite.shader = [CCShader shaderNamed:self.shaderName];
//
//    _sprite.shaderUniforms[@"u_ColorMatrix"] = [NSValue valueWithGLKMatrix4:GLKMatrix4Identity];
//
//    FloatSlider *hueRotation = [FloatSlider sliderNamed:@"Hue Rotation"];
//    hueRotation.startValue = 0.0f;
//    hueRotation.endValue = 360.0f;
//    hueRotation.valueBlock = ^(float value){
//        _colorRotation = value*M_PI/180.0f;
//        [self updateColors];
//    };
//
//    FloatSlider *colorScaleSlider = [FloatSlider sliderNamed:@"Exposure"];
//    colorScaleSlider.endValue = 2.0f;
//    colorScaleSlider.sliderValue = 0.5f;
//    colorScaleSlider.valueBlock = ^(float value){
//        _colorScale = value;
//        [self updateColors];
//    };
//
//
//    FloatSlider *saturationSlider = [FloatSlider sliderNamed:@"Saturation"];
//    saturationSlider.endValue = 2.0f;
//    saturationSlider.sliderValue = 0.5f;
//    saturationSlider.valueBlock = ^(float value){
//        _saturationAdjustment = value;
//        [self updateColors];
//    };
//
//    CCLayoutBox *content = [CCLayoutBox node];
//    content.anchorPoint = ccp(0.5, 0.5);
//    content.direction = CCLayoutBoxDirectionVertical;
//
//    [content addChild:saturationSlider];
//    [content addChild:colorScaleSlider];
//    [content addChild:hueRotation];
//    [content addChild:_sprite];
//}
//
//void ExampleBase::setupExample13()
//{
//    CCSprite * _sprite;
//    float _blurStrength;
//
//    // exampleContent
//
//    _sprite = [CCSprite spriteWithImageNamed:@"Logo.png"];
//    _sprite.shader = [CCShader shaderNamed:self.shaderName];
//
//
//    // TODO: If you add the _sprite (and slider) to content node, the _sprite no longer moves.
//    /*
//     ColorSlider *blurStrengthSlider = [ColorSlider node];
//     blurStrengthSlider.sliderValue = 0.5f;
//     blurStrengthSlider.startColor = [CCColor colorWithRed:0 green:0 blue:0 alpha:0];
//     blurStrengthSlider.endColor = [CCColor colorWithRed:1 green:1 blue:1 alpha:1];
//     blurStrengthSlider.colorBlock = ^(CCColor *color){
//     _blurStrength = color.red * 10000.0;
//     };
//
//     CCNode *content = [CCNode node];
//     //content.contentSize = CGSizeMake(100.0f, 100.0f);
//     blurStrengthSlider.position = ccp(-200, -300);
//
//     [content addChild:blurStrengthSlider];
//     [content addChild:_sprite];
//     */
//
//    -(void)update:(CCTime)delta
//    {
//        [super update:delta];
//
//        CGPoint previousPosition = _sprite.position;
//
//        _sprite.position = ccp(0.5, 0.5);
//        _sprite.position = ccpAdd(_sprite.position, ccp(sinf(self.time * 2.0) / 10.0, cosf(self.time * 3.0) / 10.0));
//
//        // To find the amount we moved this frame, we can subtract our new position from the previous position...
//        CGPoint blurVector = ccpSub(_sprite.position, previousPosition);
//
//        // Cocos2D flips the y axis.
//        blurVector.y = -blurVector.y;
//
//        // Divide by the delta time to avoid inconsistent trail lengths when framerate is inconsistent. And multiply by blur strength.s
//        blurVector = CGPointMake(blurVector.x / delta * _blurStrength, blurVector.y / delta * _blurStrength);
//
//        CGPoint p = CGPointMake(blurVector.x / _sprite.texture.contentSize.width, blurVector.y / _sprite.texture.contentSize.height);
//        _sprite.shaderUniforms[@"u_BlurVector"] = [NSValue valueWithCGPoint:p];
//    }
//
//}
//
//void ExampleBase::setupExample14()
//{
//    CCTexture *noise = [CCTexture textureWithFile:@"BisectionNoise.png"];
//    noise.texParameters = &(ccTexParams){GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
//    sprite.shaderUniforms[@"u_NoiseTexture"] = noise;
//}
//void ExampleBase::setupExample15()
//{
//    [sprite runAction:[CCActionRepeatForever actionWithAction:[CCActionSequence actions:
//                                                               [CCActionScaleTo actionWithDuration:2 scale:4],
//                                                               [CCActionScaleTo actionWithDuration:2 scale:1],
//                                                               nil
//                                                               ]]];
//}
//
//void ExampleBase::setupExample15()
//{
//    CCRenderTexture *rt = [CCRenderTexture renderTextureWithWidth:size.width height:size.height];
//    rt.autoDraw = YES;
//    rt.clearColor = [CCColor clearColor];
//    rt.clearFlags = GL_COLOR_BUFFER_BIT;
//    rt.sprite.shader = [CCShader shaderNamed:self.shaderName];
//
//    // Run the render texture at a low resolution.
//    // No need to waste fillrate on a smooth effect.
//    rt.contentScale = 0.5;
//
//    // This is currently a private method, thus the private header above.
//    // Render textures default to using nearest neighbor (aliased/blocky) filtering.
//    // The effect will look really crummy if it's blocky.
//    rt.texture.antialiased = YES;
//
//    CCParticleSystem *particles = [CCParticleSystem particleWithFile:@"Fog.plist"];
//    particles.position = ccp(size.width/2, size.height/2);
//    [rt addChild:	particles];
//}




void ExampleBase::setupExamples()
{
    _examples.push_back( {
        "Simple",
        "E01_SimplestShader",
        "E01_SimplestShader"
    } );
    _examples.push_back( {
        "Sprite Color",
        "E02_SpriteColor",
        "E02_SpriteColor"
    } );
    _examples.push_back( {
        "Sprite Distort",
        "E03_SpriteDistort",
        "E03_SpriteDistort"
    } );
    _examples.push_back( {
        "Grayscale",
        "E04_Grayscale",
        "E04_Grayscale"
    } );
    _examples.push_back( {
        "Chromatic",
        "E05_Chromatic",
        "E05_Chromatic"
    } );
    _examples.push_back( {
        "Shadow",
        "E06_Shadow",
        "E06_Shadow"
    } );
    _examples.push_back( {
        "Color Flash",
        "E07_ColorFlash",
        "E07_ColorFlash"
    } );
    _examples.push_back( {
        "Outline",
        "E08_Outline",
        "E08_Outline"
    } );
    _examples.push_back( {
        "Static Noise",
        "E09_Static",
        "E09_Static"
    } );
    _examples.push_back( {
        "Water (MultiTexture)",
        "E10_Water",
        "E10_Water",
        "res/Caustic.png",
        "res/BisectionNoise.png"
    });
    _examples.push_back( {
        "Diffusion",
        "E11_Diffusion",
        "E11_Diffusion"
    } );
    _examples.push_back( {
        "Color Matrix",
        "E12_ColorMatrix",
        "E12_ColorMatrix"
    } );
    _examples.push_back( {
        "Motion Blur",
        "E13_MotionBlur",
        "E13_MotionBlur"
    } );
    _examples.push_back( {
        "Flag (Ripped)",
        "E14_Flag",
        "E14_Flag"
    } );
    _examples.push_back( {
        "Distance Field",
        "E15_SignedDistanceField",
        "E15_SignedDistanceField"
    } );
    _examples.push_back( {
        "Meta Particles",
        "E16_MetaParticles",
        "E16_MetaParticles"
    } );
}
