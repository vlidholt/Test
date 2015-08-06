//
//  ParticleTest.cpp
//  Test
//
//  Created by Viktor Lidholt on 8/5/15.
//
//

#include "ParticleTest.h"

USING_NS_CC;

#define NUM_SYSTEMS 1000
#define NUM_FRAMES 1000

void TestParticleSystem::runTest()
{
    double timeStart;
    timeStart = currentTime();
    
    // Create systems
    Vector<TestParticleSystem*> systems(NUM_SYSTEMS);
    for (int i = 0; i < NUM_SYSTEMS; i++) {
        systems.pushBack(TestParticleSystem::create());
    }
    
    double timeAfterCreate = currentTime();
    CCLOG("TIME creation %f\n", timeAfterCreate - timeStart);
    timeStart = currentTime();
    
    // Update systems
    for (int frame = 0; frame < NUM_FRAMES; frame++) {
        for (int i = 0; i < NUM_SYSTEMS; i++) {
            TestParticleSystem* system = systems.at(i);
            system->update(1.0 / 60.0);
        }
    }
    
    double timeAfterUpdates = currentTime();
    CCLOG("TIME updates %f\n", timeAfterUpdates - timeStart);
    timeStart = currentTime();
    
    // Update systems
    for (int frame = 0; frame < NUM_FRAMES; frame++) {
        for (int i = 0; i < NUM_SYSTEMS; i++) {
            TestParticleSystem* system = systems.at(i);
            system->paint();
        }
    }
    
    double timeAfterMatrices = currentTime();
    CCLOG("TIME matrices %f\n", timeAfterMatrices - timeStart);
}

TestParticleSystem* TestParticleSystem::create()
{
    TestParticleSystem* system = new TestParticleSystem();
    system->autorelease();
    return system;
}

TestParticleSystem::TestParticleSystem() : particles(100)
{
    life = 1.5;
    lifeVar = 0.0;
    posVar = Vec2();
    startSize = 2.5;
    startSizeVar = 0.5;
    endSize = 0.0;
    endSizeVar = 0.0;
    startRotation = 0.0;
    startRotationVar = 0.0;
    endRotation = 0.0;
    endRotationVar = 0.0;
    rotateToMovement = false;
    direction = 0.0;
    directionVar = 360.0;
    speed = 100.0;
    speedVar = 50.0;
    radialAcceleration = 0.0;
    radialAccelerationVar = 0.0;
    tangentialAcceleration = 0.0;
    tangentialAccelerationVar = 0.0;
    gravity = Vec2();
    maxParticles = 100;
    emissionRate = 50.0;
    numParticlesToEmit = 0;
}

void TestParticleSystem::update(float dt)
{
    // Create new particles
    float rate = 1.0 / emissionRate;
    
    if (particles.size() < maxParticles) {
        emitCounter += dt;
    }
    
    while (particles.size() < maxParticles
           && emitCounter > rate
           && (numParticlesToEmit == 0 || numEmittedParticles < numParticlesToEmit)) {
        addParticle();
        emitCounter -= rate;
    }
    
    // Iterate over all particles
    for (int i = particles.size() -1; i >= 0; i--) {
        TestParticle* particle = particles.at(i);
        
        // Manage life time
        particle->timeToLive -= dt;
        if (particle->timeToLive <= 0) {
            particles.erase(i);
            continue;
        }
        
        // Update the particle
        
        // Radial acceleration
        Vec2 radial = Vec2(particle->pos);
        radial.normalize();
        
        Vec2 tangential = Vec2(radial);
        radial.scale(particle->radialAccel);
        
        // Tangential acceleration
        float newY = tangential.x;
        tangential.x = - tangential.y;
        tangential.y = newY;
        tangential.scale(particle->tangentialAccel);
        
        // (gravity + radial + tangential) * dt
        Vec2 accel = radial + tangential + gravity;
        accel.scale(dt);
        particle->dir += accel;
        accel.scale(dt);
        
        // Update particle position
        particle->pos += accel;
        
        // Size
        particle->size = MAX(particle->size + particle->deltaSize * dt, 0.0);
        
        // Angle
        particle->rotation += particle->rotation * dt;
        
        // Color
        for (int i = 0; i < 4; i++) {
            particle->simpleColorSequence[i] += particle->simpleColorSequence[i + 4] * dt;
        }
    }
}

bool printed = false;

void TestParticleSystem::paint()
{
    if (!printed) {
        printed = true;
    }
    
    for (int i = particles.size() -1; i >= 0; i--) {
        TestParticle* particle = particles.at(i);
        particle->rotation += CCRANDOM_MINUS1_1();
        
        // Transform
        float c = cosf(CC_DEGREES_TO_RADIANS(particle->rotation));
        float s = sinf(CC_DEGREES_TO_RADIANS(particle->rotation));
        
        // Create transformation matrix for scale, position and rotation
        Mat4 matrix = Mat4(c * particle->size, s * particle->size, 0.0, 0.0,
             -s * particle->size, c * particle->size, 0.0, 0.0,
             0.0, 0.0, 1.0, 0.0,
             particle->pos.x, particle->pos.y, 0.0, 1.0);
        
        particle->transform.multiply(matrix);
    }
}

void TestParticleSystem::addParticle()
{
    TestParticle* particle = TestParticle::create();
    
    // Time to live
    particle->timeToLive = MAX(life + lifeVar * CCRANDOM_MINUS1_1(), 0.0);
    
    // Position
    Vec2 srcPos = Vec2();
    particle->pos = Vec2(srcPos.x + posVar.x * CCRANDOM_MINUS1_1(),
                         srcPos.y + posVar.y * CCRANDOM_MINUS1_1());
    
    // Size
    particle->size = MAX(startSize + startSizeVar * CCRANDOM_MINUS1_1(), 0.0);
    float endSizeFinal = MAX(endSize + endSizeVar * CCRANDOM_MINUS1_1(), 0.0);
    particle->deltaSize = (endSizeFinal - particle->size) / particle->timeToLive;
    
    // Rotation
    particle->rotation = startRotation + startRotationVar * CCRANDOM_MINUS1_1();
    float endRotationFinal = endRotation + endRotationVar * CCRANDOM_MINUS1_1();
    particle->deltaRotation = (endRotationFinal - particle->rotation) / particle->timeToLive;
    
    // Direction
    float dirRadians = CC_DEGREES_TO_RADIANS(direction + directionVar * CCRANDOM_MINUS1_1());
    Vec2 dirVector = Vec2(cosf(dirRadians), sin(dirRadians));
    float speedFinal = speed + speedVar * CCRANDOM_MINUS1_1();
    dirVector.scale(speedFinal);
    particle->dir = dirVector;
    
    // Accelerations
    particle->radialAccel = radialAcceleration + radialAccelerationVar * CCRANDOM_MINUS1_1();
    particle->tangentialAccel = tangentialAcceleration + tangentialAccelerationVar;
    
    // Colors
    particle->simpleColorSequence[0] = 255.0;
    particle->simpleColorSequence[1] = 255.0;
    particle->simpleColorSequence[2] = 255.0;
    particle->simpleColorSequence[3] = 255.0;
    
    particle->simpleColorSequence[4] = 255.0;
    particle->simpleColorSequence[5] = 0.0;
    particle->simpleColorSequence[6] = 0.0;
    particle->simpleColorSequence[7] = 0.0;
    
    particle->transform = Mat4();
    
    particles.pushBack(particle);
    numEmittedParticles++;
}

TestParticle* TestParticle::create()
{
    TestParticle* particle = new TestParticle();
    particle->autorelease();
    return particle;
}

double currentTime() {
    struct timeval nowTime;
    gettimeofday(&nowTime, NULL);
    double secs = nowTime.tv_sec;
    secs += nowTime.tv_usec / 1000000.0;
    
    return secs;
}
