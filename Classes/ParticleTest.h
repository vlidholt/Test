//
//  ParticleTest.h
//  Test
//
//  Created by Viktor Lidholt on 8/5/15.
//
//

#ifndef __Test__ParticleTest__
#define __Test__ParticleTest__

#include <stdio.h>
#include "cocos2d.h"

class TestParticle : public cocos2d::Ref {
public:
    cocos2d::Vec2 pos;
    cocos2d::Vec2 startPos;
    
    float colorPos;
    float deltaColorPos;
    
    float size;
    float deltaSize;
    
    float rotation;
    float deltaRotation;
    
    float timeToLive;
    
    cocos2d::Vec2 dir;
    
    float radialAccel;
    float tangentialAccel;
    
    float simpleColorSequence[8];
    
    static TestParticle* create();
    
    cocos2d::Mat4 transform;
};

class TestParticleSystem : public cocos2d::Ref {
public:
    static void runTest();
    
    float life;
    float lifeVar;
    
    cocos2d::Vec2 posVar;
    
    float startSize;
    float startSizeVar;
    
    float endSize;
    float endSizeVar;
    
    float startRotation;
    float startRotationVar;
    
    float endRotation;
    float endRotationVar;
    
    bool rotateToMovement;
    
    float direction;
    float directionVar;
    
    float speed;
    float speedVar;
    
    float radialAcceleration;
    float radialAccelerationVar;
    
    float tangentialAcceleration;
    float tangentialAccelerationVar;
    
    cocos2d::Vec2 gravity;
    
    int maxParticles;
    int numParticlesToEmit;
    float emissionRate;
    
    cocos2d::Vector<TestParticle*> particles;
    
    float emitCounter;
    int numEmittedParticles;
    
    TestParticleSystem();
    
    static TestParticleSystem* create();

    void update(float dt);
    void paint();
    void addParticle();
};

double currentTime();

#endif /* defined(__Test__ParticleTest__) */