#include "ofApp.h"
#define BLUR_MARGIN 100
#define TEXT "テスト"

//--------------------------------------------------------------
void ofApp::setup(){
    // https://twitter.com/naokiring/status/362151841498284032
    // https://www.khronos.org/opengles/sdk/docs/man/xhtml/glBlendFuncSeparate.xml
    // http://ofxtips.hateblo.jp/entry/2013/09/01/181839
    // GL_SRC_ALPHA: srcRGB にA(=Alpha)をかける
    // GL_ONE: dstRGB に1をかける（＝そのまま） srcRGBと足し算
    // GL_ONE_MINUS_SRC_ALPHA: 1 - A * srcAlpha
    // GL_ONE: dstAlpha に1をかける（＝そのまま） srcAlphaと足し算
    glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA,GL_ONE);
    
#ifdef TARGET_OPENGLES
    cout << "using shadersES2" << endl;
    shaderBlurX.load("shadersES2/shaderBlurX");
    shaderBlurY.load("shadersES2/shaderBlurY");
#else
    if(ofIsGLProgrammableRenderer()){
        cout << "using shadersGL3" << endl;
        shaderBlurX.load("shadersGL3/shaderBlurX");
        shaderBlurY.load("shadersGL3/shaderBlurY");
    }else{
        cout << "using shadersGL2" << endl;
        shaderBlurX.load("shadersGL2/shaderBlurX");
        shaderBlurY.load("shadersGL2/shaderBlurY");
    }
#endif

    
    //http://docs.openframeworks.cc/ofTrueTypeFont.html#LoadFont
    // antiAliased = true
    font.loadFont("mplus-1m-regular.ttf", 25, true);

    bgImage.load("bg.jpg");
    sparkImage.load("spark.png");
    
    fboSparkBlurOnePass.allocate(sparkImage.getWidth(), sparkImage.getHeight(), GL_RGBA);
    fboSparkBlurTwoPass.allocate(sparkImage.getWidth(), sparkImage.getHeight(), GL_RGBA);
    
    fboText.allocate(300, 300, GL_RGBA);
    fboTextBlurOnePass.allocate(300, 300, GL_RGBA);
    fboTextBlurTwoPass.allocate(300, 300, GL_RGBA);
    
    ofEnableAlphaBlending();
    ofDisableAlphaBlending();

}

//--------------------------------------------------------------
void ofApp::update(){
    blurX = ofMap(mouseX, 0, ofGetWidth(), 0, 10, true);
    blurY = ofMap(mouseY, 0, ofGetHeight(), 0, 10, true);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
    if(bgFlag) bgImage.draw(0, 0);
    
    /*
     ・Textはシェーダーの外に書かないとだめ、シェーダー中に書くとブラックボックスになる
     ・Fboのbeginはdrawの中でやらないとだめ、setupでつくると、グリッチする
     ・Fboを作る段階で、ofEnableAlphaBlending されてるとグリッチするので、
     　スクリーンに描写するときだけ ofEnableAlphaBlending とする。
     ・描写されてるスクリーンを前提に動作するっぽいので、複数beginしてまとめてdrawはだめ
     　都度、beginした後にdrawして、またbeginしなおす必要がある。
     ・起動時に一瞬グリッチな画面がdrawされる、setupで一回ofEnableAlphaBlendingさせとくと大丈夫っぽい
     　初回のofEnableAlphaBlendingをいきなりdraw中でやらせるとダメなのか
     */
    
    //---
    
    if(sparkFlag){
        fboSparkBlurOnePass.begin();
        {
            shaderBlurX.begin();
            {
                shaderBlurX.setUniform1f("blurAmnt", blurX);
                sparkImage.draw(0, 0);
            }
            shaderBlurX.end();
        }
        fboSparkBlurOnePass.end();
        
        fboSparkBlurTwoPass.begin();
        {
            shaderBlurY.begin();
            {
                shaderBlurY.setUniform1f("blurAmnt", blurY);
                fboSparkBlurOnePass.draw(0, 0);
            }
            shaderBlurY.end();
        }
        fboSparkBlurTwoPass.end();
        
        ofEnableAlphaBlending();
        {
            fboSparkBlurTwoPass.draw(0, 0);
        }
        ofDisableAlphaBlending();
    }
    
    //---
    
    if(textFlag) {
        fboText.begin();
        {
            ofSetColor(255, 255, 255, 0);
            ofDrawRectangle(0, 0, 300, 300);
            ofSetColor(255, 255, 255, 255);
            font.drawString(TEXT, BLUR_MARGIN, BLUR_MARGIN + font.stringHeight(TEXT));
        }
        fboText.end();
        
        fboTextBlurOnePass.begin();
        {
            shaderBlurX.begin();
            {
                shaderBlurX.setUniform1f("blurAmnt", blurX);
                fboText.draw(0, 0);
            }
            shaderBlurX.end();
        }
        fboTextBlurOnePass.end();
        
        fboTextBlurTwoPass.begin();
        {
            shaderBlurY.begin();
            {
                shaderBlurY.setUniform1f("blurAmnt", blurY);
                fboTextBlurOnePass.draw(0, 0);
            }
            shaderBlurY.end();
        }
        fboTextBlurTwoPass.end();
        
        ofEnableAlphaBlending();
        {
            fboTextBlurTwoPass.draw(mouseX - BLUR_MARGIN - font.stringWidth(TEXT), mouseY - BLUR_MARGIN);
            fboText.draw(mouseX - BLUR_MARGIN - font.stringWidth(TEXT), mouseY - BLUR_MARGIN);
            fboText.draw(mouseX - BLUR_MARGIN - font.stringWidth(TEXT), mouseY - BLUR_MARGIN + font.stringHeight("テスト") * 2); // 比較用の通常テキスト
        }
        ofDisableAlphaBlending();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case '1':
            bgFlag = !bgFlag;
            break;
        case '2':
            sparkFlag = !sparkFlag;
            break;
        case '3':
            textFlag = !textFlag;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
