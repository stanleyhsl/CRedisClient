#include "Command.h"
#include "CRedisClient.h"
uint64_t CRedisClient::stringToVecString(string& str, CRedisClient::VecString& vec)
{
    uint64_t i=0;
    uint64_t len=str.length();
    uint64_t posStart=0;
    while(i<len)
    {
        if (str[i]=='\n')
        {
            vec.push_back(str.substr(posStart,i-posStart));
            posStart=i+1;
        }
        ++i;
    }
    return vec.size();

}
string CRedisClient::bgrewriteaof()
{
    Command cmd( "BGREWRITEAOF" );
    string str;
    _getStatus(cmd,str);
    return str;
}



string CRedisClient::bgsave()
{
    Command cmd( "BGSAVE" );
    string str;
    _getStatus(cmd,str);
    return str;
}
string  CRedisClient::clientGetname()
{
    Command cmd( "CLIENT" );
    cmd<<"GETNAME";
    string str;
    _getString(cmd,str);
    return str;

}
bool CRedisClient::clientKill(const string& ip,const UInt16 port)
{
    Command cmd( "CLIENT" );
    cmd<<"KILL";
    std::stringstream ss;
    ss << ip<<":"<< port;
    cmd<<ss.str();
    string status;
    _getStatus( cmd, status );
    return ("OK"==status? true : false );

}
/*
void CRedisClient::clientList(CResult& result)
{
    Command cmd( "CLIENT" );
    cmd<<"LIST";
    _socket.clearBuffer();
    _sendCommand(cmd);
    _getReply(result);

}
*/
uint64_t CRedisClient::clientList(CRedisClient::VecString & reply)
{
    Command cmd( "CLIENT" );
    cmd<<"LIST";
    string str;
    _getString(cmd,str);
    return stringToVecString(str,reply);
}
bool CRedisClient::clientSetname (const string& connectionName)
{
    Command cmd( "CLIENT" );
    cmd<<"SETNAME";
    cmd<<connectionName;

    string status;

    _getStatus( cmd, status );
    return ("OK"==status? true : false );

}
uint64_t CRedisClient::configGet(const string& parameter,CRedisClient::VecString& reply)
{
    Command cmd( "CONFIG" );
    cmd<<"GET";
    cmd<<parameter;
    return   _getArry(cmd,reply);
}
void CRedisClient::configResetstat()
{
    Command cmd( "CONFIG" );
    cmd<<"RESETSTAT";
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        throw ProtocolErr( "CONFIG RESETSTAT: data recved is not OK" );
}
bool CRedisClient::configRewrite()
{
    Command cmd( "CONFIG" );
    cmd<<"REWRITE";
    string status;
    _getStatus( cmd, status );
    return ("OK"==status? true : false );
}
bool CRedisClient::configSet(const string& parameter,const string& value)
{
    Command cmd( "CONFIG" );
    cmd<<"SET";
    cmd<<parameter<<value;
    string status;
    _getStatus( cmd, status );
    return ("OK"==status? true : false );
}
uint64_t CRedisClient::dbsize()
{
    Command cmd( "DBSIZE" );
    int64_t num;
    _getInt(cmd,num);
    return num;

}
string CRedisClient::debugObject(const string& key)
{
    Command cmd( "DEBUG" );
    cmd<<"OBJECT";
    cmd<<key;
    string str;
    _getString(cmd,str);
    return str;

}
void CRedisClient::debugSegfault()
{
    Command cmd( "DEBUG" );
    cmd<<"SEGFAULT";
    _socket.clearBuffer();
    _sendCommand(cmd);

}
void CRedisClient::flushall()
{
    Command cmd( "FLUSHALL" );
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        throw ProtocolErr( "FLUSHALL: data recved is not OK" );
}
void CRedisClient::flushdb()
{
    Command cmd( "FLUSHDB" );
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        throw ProtocolErr( "FLUSHDB: data recved is not OK" );
}

void CRedisClient::info(CRedisClient::VecString& reply,const string& section)
{
    Command cmd( "INFO" );
    string str;
    if(""!=section)
        cmd << section;
    _getString(cmd,str);
    stringToVecString(str,reply);

}
uint64_t CRedisClient::lastsave()
{
    Command cmd( "LASTSAVE" );
    int64_t num;
    _getInt(cmd,num);
    return num;

}

void CRedisClient::monitor(uint64_t timeout, string& reply)
{
     Command cmd("MONITOR");
    _socket.clearBuffer();
    _socket.setReceiveTimeout(timeout);
    _sendCommand(cmd);
    while(1)
    {
        _socket.readLine(reply);
        reply=reply.substr(1);
        //std::cout<<reply<<std::endl;
    }

}

bool CRedisClient::save()
{
    Command cmd( "SAVE" );
    string status;
    _getStatus( cmd, status );
    return ("OK"==status? true : false );

}

string CRedisClient::shutdown()
{
    Command cmd( "SHUTDOWN" );
    string status;
    _getStatus( cmd, status );
    return status;
}

void CRedisClient::slaveof(const string& host,const UInt16 port)
{
    Command cmd( "SLAVEOF" );
    cmd<<host<<port;
    string status;
    _getStatus( cmd, status );
    std::cout<<status<<std::endl;
    if (('O'==status[0])&&('K'==status[1]))
            return;
        throw ProtocolErr( "SLAVEOF: data recved is not OK" );
}

void CRedisClient::slowlog(const CRedisClient::VecString &subcommand, CResult &reply)
{
    Command cmd( "SLOWLOG" );
    _socket.clearBuffer();
    VecString::const_iterator it = subcommand.begin();
    VecString::const_iterator  end=subcommand.end();
    for ( ; it !=end; ++it )
    {
        cmd << *it;
    }
    _sendCommand(cmd);
    _getReply(reply);
}


void CRedisClient::time(string& currentseconds,string& microseconds)
{
    CResult result;
    Command cmd( "TIME" );
    _getArry(cmd,result);
    CResult::ListCResult::const_iterator it = result.getArry().begin();
    currentseconds=it->getString();
    ++it;
    microseconds=it->getString();
}
