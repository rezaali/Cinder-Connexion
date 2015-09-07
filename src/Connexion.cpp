#include "Connexion.h"

namespace reza { namespace con {

UInt16 Connexion::sClientID;
ConnexionDataSignal Connexion::sSignalUpdate;
ConnexionData Connexion::sConnexionData;
    
ConnexionData::ConnexionData()
{
    mTranslation = ci::vec3( 0.0f );
    mRotation = ci::vec3( 0.0f );
    mButton = -1;
    mTimeStamp = -1;
}
    
ConnexionData::ConnexionData( const ci::vec3& translation, const ci::vec3& rotation, const int& button, const double& timestamp ) : mTranslation( translation ), mRotation( rotation ), mButton( button ), mTimeStamp( timestamp ) { }
    
ConnexionData::ConnexionData( const ConnexionData& copy )
{
    mTranslation = copy.mTranslation;
    mRotation = copy.mRotation;
    mButton = copy.mButton;
    mTimeStamp = copy.mTimeStamp;
}

void Connexion::start()
{
    std::cout << "STARTING CONNEXION" << std::endl;
    InstallConnexionHandlers( driverHandler, 0L, 0L );
    std::string appName = "Concourse";
    unsigned char* pappName = new unsigned char[ appName.size() + 1 ];
    pappName[0] = ( unsigned char ) appName.size();
    memcpy( &( pappName[ 1 ] ), appName.c_str(), appName.size() );
    sClientID = RegisterConnexionClient( 0, (UInt8*) pappName, kConnexionClientModeTakeOver, kConnexionMaskAll );
    std::cout << sClientID << std::endl;
    delete [] pappName;
}

void Connexion::stop()
{
    std::cout << "STOPPING CONNEXION" << std::endl;
    UnregisterConnexionClient( sClientID );
    CleanupConnexionHandlers();
}

void Connexion::setLed( bool state )
{
    SInt32 result;
    ConnexionClientControl( sClientID, kConnexionCtlSetLEDState, state ? 0x00010001 : 0x00010000, &result );
}

void Connexion::driverHandler( io_connect_t connection, natural_t messageType, void *messageArgument )
{
    ConnexionDeviceStatePtr msg = ( ConnexionDeviceStatePtr ) messageArgument;
    switch( messageType )
    {
        case kConnexionMsgDeviceState:
        {
            switch ( msg->command )
            {
                case kConnexionCmdHandleAxis:
                {
                    sConnexionData.mTranslation = ci::vec3( msg->axis[0], msg->axis[1], msg->axis[2] );
                    sConnexionData.mRotation = ci::vec3( msg->axis[3], msg->axis[4], msg->axis[5] );
                }
                case kConnexionCmdHandleButtons:
                {
                    sConnexionData.mButton = msg->buttons;
                    sConnexionData.mTimeStamp = ci::app::getElapsedSeconds();
                }
                sSignalUpdate.emit( sConnexionData );
            }
        }
        break;
    }
}

} } // namespace reza::con