//
//  ExampleBase.cpp
//  cocos2d-shader-cookbook
//
//  Created by Steve Tranby on 8/31/14.
//
//

#include "ExampleBase.h"
#include <CocosGUI.h>

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

    auto titleLabel = Label::createWithSystemFont("this getExampleName", "Helvetica", 18);
    titleLabel->setNormalizedPosition(Vec2(0.5, 0.9));
    this->addChild(titleLabel);

    auto prev = Text::create("Prev", "Helvectica", 48);
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

    _examples.push_back( { "E01_SimplestShader", "Simple" } );
    _examples.push_back( { "E02_SpriteColor", "Sprite Color" } );
    _examples.push_back( { "E03_SpriteDistort", "Sprite Distort" } );
    _examples.push_back( { "E04_Grayscale", "Grayscale" } );
    _examples.push_back( { "E05_Chromatic", "Chromatic" } );
    _examples.push_back( { "E06_Shadow", "Shadow" } );
    _examples.push_back( { "E07_ColorFlash", "Color Flash" } );
    _examples.push_back( { "E08_Outline", "Outline" } );
    _examples.push_back( { "E09_Static", "Static Noise" } );
    _examples.push_back( { "E10_Water", "Water (MultiTexture)", "res/Caustic.png", "res/BisectionNoise.png" });
    _currentIndex = 0;
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

    auto vertFilename = std::string("res/shaders/").append(shaderInfo.name).append(".vsh");
    auto fragFilename = std::string("res/shaders/").append(shaderInfo.name).append(".fsh");

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

            auto locid = glprogram->getUniformLocationForName("uNoiseTextureSize");
            glprogram->setUniformLocationWith2f(locid, 30, 30);
            glprogramstate->setUniformVec2("u_NoiseTextureSize", tex2->getContentSizeInPixels());
            glprogramstate->setUniformTexture("u_NoiseTexture", tex2);
        }

    }

    // attach the ProgramState to a Node
    _exampleSprite->setGLProgram(glprogram);
}

//std::string ExampleBase::exampleName()
//{
//    std::string name("Steve");
////    auto className =
////    NSString *className = NSStringFromClass(self.class);
////
////    NSUInteger i = 0;
////    NSMutableString *name = [NSMutableString stringWithCapacity:0];
////
////    for(; i < className.length; i++){
////        unichar c = [className characterAtIndex:i];
////        if(c == '_'){
////            [name appendString:@": "];
////
////            i++;
////            break;
////        } else {
////            [name appendString:[NSString stringWithCharacters:&c length:1]];
////        }
////    }
////
////    for(; i < className.length; i++){
////        unichar c = [className characterAtIndex:i];
////        if('A' <= c && c <= 'Z'){
////            [name appendString:@" "];
////        }
////
////        [name appendString:[NSString stringWithCharacters:&c length:1]];
////    }
//
//    return name;
//}
//
////-(CCNode *)ExampleBase::exampleContent
////{
////    NSAssert(NO, @"Override me!");
////    return nil;
////}
////
////-(NSString *)ExampleBase::shaderName
////{
////    return NSStringFromClass(self.class);
////}
////
////-(NSString *)ExampleBase::sourceString
////{
////    NSString *shaderName = self.shaderName;
////
////    NSString *fragmentName = [shaderName stringByAppendingPathExtension:@"fsh"];
////    NSString *fragmentPath = [[CCFileUtils sharedFileUtils] fullPathForFilename:fragmentName];
////    NSAssert(fragmentPath, @"Failed to find '%@'.", fragmentName);
////    NSString *fragmentSource = [NSString stringWithContentsOfFile:fragmentPath encoding:NSUTF8StringEncoding error:nil];
////
////    NSString *vertexName = [shaderName stringByAppendingPathExtension:@"vsh"];
////    NSString *vertexPath = [[CCFileUtils sharedFileUtils] fullPathForFilename:vertexName];
////    NSString *vertexSource = (vertexPath ? [NSString stringWithContentsOfFile:vertexPath encoding:NSUTF8StringEncoding error:nil] : @"... Default Vertex Shader. Don't worry about it yet.");
////
////    return [NSString stringWithFormat:@"VERTEX SHADER:\n\n%@\n\nFRAGMENT SHADER:\n\n%@\n", vertexSource, fragmentSource];
////}
////
////-(CCLabelTTF *)sourceLabel
////{
////    CCLabelTTF *label = [CCLabelTTF labelWithString:self.sourceString fontName:@"Helvetica" fontSize:12.0];
////    label.color = [CCColor blackColor];
////
////    return label;
////}
////
////@end
////
////
////@implementation ExampleSlider
////
////+(instancetype)ExampleSlider::sliderNamed:(NSString *)name
////{
////    return [[self alloc] initWithName:name];
////}
////
////-(id)ExampleSlider::initWithName:(NSString *)name
////{
////    CCSpriteFrame *bg = [CCSpriteFrame frameWithImageNamed:@"slider-background.png"];
////    CCSpriteFrame *handle = [CCSpriteFrame frameWithImageNamed:@"slider-handle.png"];
////
////    if((self = [super initWithBackground:bg andHandleImage:handle])){
////        self.preferredSize = CGSizeMake(100, bg.originalSize.height);
////
////        [self setTarget:self selector:@selector(callback:)];
////        self.continuous = YES;
////
////        CCLabelTTF *label = [CCLabelTTF labelWithString:[name stringByAppendingString:@":"] fontName:@"Helvetica" fontSize:12];
////        label.color = [CCColor blackColor];
////        label.anchorPoint = ccp(1.0, 0.5);
////        label.positionType = CCPositionTypeNormalized;
////        label.position = ccp(-0.1, 0.5);
////
////        [self addChild:label];
////    }
////
////    return self;
////}
////
////@end
////
////
////@implementation FloatSlider {
////    CCLabelTTF *_valueLabel;
////}
////
////-(id)FloatSlider::initWithName:(NSString *)name
////{
////    if((self = [super initWithName:name])){
////        self.startValue = 0.0f;
////        self.endValue = 1.0f;
////
////        _valueLabel = [CCLabelTTF labelWithString:@"" fontName:@"Helvetica" fontSize:12];
////        _valueLabel.color = [CCColor blackColor];
////        _valueLabel.anchorPoint = ccp(0.0, 0.5);
////        _valueLabel.positionType = CCPositionTypeNormalized;
////        _valueLabel.position = ccp(1.1, 0.5);
////
////        [self addChild:_valueLabel];
////    }
////
////    return self;
////}
////
////-(void)FloatSlider::callback:(CCSlider *)slider
////{
////    float alpha = slider.sliderValue;
////    float value = (1.0f - alpha)*self.startValue + alpha*self.endValue;
////    _valueLabel.string = [NSString stringWithFormat:@"%.2f", value];
////
////    if(_valueBlock) _valueBlock(value);
////        }
////
////-(void)FloatSlider::setStartValue:(float)startValue
////{
////    _startValue = startValue;
////    [self callback:self];
////}
////
////-(void)FloatSlider::setEndValue:(float)endValue
////{
////    _endValue = endValue;
////    [self callback:self];
////}
////
////-(void)FloatSlider::setValueBlock:(void (^)(float))valueBlock
////{
////    _valueBlock = valueBlock;
////    [self callback:self];
////}
////
////@end
////
////
////@implementation ColorSlider {
////    CCNodeGradient *_gradient;
////}
////
////-(id)ColorSlider::initWithName:(NSString *)name
////{
////    if((self = [super initWithName:name])){
////        self.startColor = [CCColor whiteColor];
////        self.endColor = [CCColor blackColor];
////
////        _gradient = [CCNodeGradient nodeWithColor:self.startColor fadingTo:self.endColor alongVector:ccp(-1, 0)];
////        _gradient.contentSizeType = CCSizeTypeNormalized;
////        _gradient.contentSize = CGSizeMake(1.0, 1.0);
////
////        [self addChild:_gradient z:-1];
////    }
////
////    return self;
////}
////
////void ColorSlider::callback(CCSlider *slider)
////{
////    if(_colorBlock) _colorBlock([_startColor interpolateTo:_endColor alpha:self.sliderValue]);
////        }
////
////void ColorSlider::setStartColor:(CCColor *startColor)
////{
////    _startColor = startColor;
////    [self callback:self];
////
////    _gradient.startColor = startColor;
////    _gradient.startOpacity = startColor.alpha;
////}
////
////void ColorSlider::setEndColor:(CCColor *endColor)
////{
////    _endColor = endColor;
////    [self callback:self];
////
////    _gradient.endColor = endColor;
////    _gradient.endOpacity = endColor.alpha;
////}
//
////void ColorSlider::setColorBlock:(void (^)(CCColor *))colorBlock)
////{
////    _colorBlock = colorBlock;
////    [self callback:self];
////}
//
//@end