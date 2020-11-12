#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <sstream>

char http[]={"http://"};
char host[]={"localhost"};
char parm[]={"/"};


std::string getProtocol( std::string url )
{
    std::string protocol = "";

    int i = 0;

    for(i = 0; i < url.size(); i++)
    {
        if( url[i] != '/' || url[i+1] != '/'  )
        {
            protocol += url[i];
        }
        else
        {
            protocol += "//";
            break;
        }
    }

    return protocol;
}

std::string getHost( std::string url )
{
    std::string host = "";

    url.replace(0, getProtocol(url).size(), "");

    int i = 0;
    for(i = 0; i < url.size(); i++)
    {

        if( url[i] != '/' )
        {
            host += url[i];
        }
        else
        {
            break;
        }

    }

    return host;
}

std::string getAction( std::string url )
{
    std::string parm = "";

    url.replace(0, getProtocol(url).size()+getHost(url).size(), "");

    int i = 0;
    for(i = 0; i < url.size(); i++)
    {

        if( url[i] != '?' && url[i] != '#' )
        {
            parm += url[i];
        }
        else
        {
            break;
        }

    }

    return parm;
}

std::string getParams( std::vector< std::pair< std::string, std::string> > requestData )
{
    std::string parm = "";

    std::vector< std::pair< std::string, std::string> >::iterator itr = requestData.begin();

    for( ; itr != requestData.end(); ++itr )
    {
        if( parm.size() < 1 )
        {
            parm += "";
        }
        else
        {
            parm += "&";
        }
        parm += itr->first + "=" + itr->second;
    }

    return parm;
}


std::string GET( std::string url, std::vector< std::pair< std::string, std::string> > requestData )
{
    std::string http = getProtocol(url);
    std::string host = getHost(url);
    std::string script = getAction(url);
    std::string parm = getParams( requestData );

    char buf[1024];

    std::string header = "";

    header += "GET ";
    header += http + host + script + "?" + parm;
    header += (std::string)" HTTP/1.1" + "\r\n";
    header += (std::string)"Host: " + http + host + "/" + "\r\n";
    header += (std::string)"User-Agent: Mozilla/5.0" + "\r\n";
    //header += (std::string)"Accept: text/html" + "\r\n";
    header += (std::string)"Accept-Language: ru,en-us;q=0.7,en;q=0.3" + "\r\n";
    header += (std::string)"Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7" + "\r\n";
    header += (std::string)"Connection: keep-alive " + "\r\n";
    header += "\r\n";



    int sock;
    struct sockaddr_in addr;
    struct hostent* raw_host;

    std::cout << host.c_str() << std::endl;
    raw_host = gethostbyname("localhost");
    std::cout << raw_host->h_length << std::endl;
    if (raw_host == NULL)
    {
        std::cout<<"ERROR, no such host";
        exit(0);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1n");

    //bcopy( (char*)raw_host->h_addr, (char*)&addr.sin_addr, raw_host->h_length );
    //bcopy( raw_host->h_addr, &addr.sin_addr, raw_host->h_length );
    int err; 
    err = connect( sock, (struct sockaddr *)&addr, sizeof(addr) );
    perror("connect");
    std::cout << err << std::endl;


    char * message = new char[ header.size() ];
    for(int i = 0; i < header.size(); i++)
    {
        message[i] = header[i];
    }

    send(sock, message, header.size(), 0);
    recv(sock, buf, sizeof(buf), 0);

    std::string answer = "";

    for(int j = 0; j < 1024; j++)
    {
            answer += buf[j];
    }

    return answer;

}

int main(){
    GET("http://localhost:8080/", std::vector<std::pair<std::string, std::string> >());
    return 0;
}