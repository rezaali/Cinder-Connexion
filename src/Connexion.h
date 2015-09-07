#pragma once

#include "3DConnexionClient/ConnexionClient.h"
#include "3DConnexionClient/ConnexionClientAPI.h"
#include <IOKit/IOTypes.h>

namespace reza { namespace con {

class Connexion;
    
class ConnexionData {
public:
    ConnexionData(); 
    ConnexionData( const ci::vec3& translation, const ci::vec3& rotation, const int& button, const double& timestamp );
    ConnexionData( const ConnexionData& copy );
    ci::vec3 getTranslation() const { return mTranslation; }
    ci::vec3 getRotation() const { return mRotation; }
    int getButton( int button ) const { return ( mButton & ( 1 << button ) ) != 0; }
    double getTimeStamp() const { return mTimeStamp; }
    
protected:
    ci::vec3 mTranslation;
    ci::vec3 mRotation;
    int mButton;
    double mTimeStamp;
    friend class Connexion;
};
    
typedef ci::signals::Signal< void( ConnexionData ) > ConnexionDataSignal;
class Connexion
{
public:
    static void start();
    static void stop();
    static void setLed( bool state );    
    static ConnexionDataSignal& getSignalUpdate() { return sSignalUpdate; }
    
private:
    Connexion() { } 
    static UInt16 sClientID;
    static void driverHandler( io_connect_t connection, natural_t messageType, void *messageArgument );
    static ConnexionDataSignal sSignalUpdate;
    static ConnexionData sConnexionData;
};

} } //namespace reza::con