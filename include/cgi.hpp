
#ifndef CGI_HPP
#define CGI_HPP

#include "./request.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <string>
#include <sstream>
#include <cstring>
#include <fstream>
#include <map>




class Cgi
{
    enum ENV_LIST
    {
        AUTH_TYPE,
        CONTENT_LENGTH,
        CONTENT_TYPE,
        GATEWAY_INTERFACE,
        PATH_INFO,
        PATH_TRANSLATED,
        QUERY_STRING,
        REMOTE_ADDR,
        REMOTE_IDENT,
        REMOTE_USER,
        REQUEST_METHOD,
        REQUEST_URI,
        SCRIPT_FILENAME,
        SERVER_NAME,
        SERVER_PORT,
        SERVER_PROTOCOL,
        SERVER_SOFTWARE,
        REDIRECT_STATUS,
        DEFAULT,
    };


    public:
        Cgi(void);
        ~Cgi();

        int execute(Request req, std::string cgiFilePath, std::string file);
        std::string getCgiRespoHeader(void);
        std::string getCgiRespoBody(void);

    private:
        static const std::string env_list[DEFAULT];
        char **env;
        size_t alloc_size;
        std::string cgiRespoHeader;
        std::string cgiRespoBody;
        std::string cgiInput;

        std::map<std::string, std::string> env_Map(Request &req, std::string file);
        void setEnviron(Request &req, std::string file);
        std::string getInput(std::string file);
        void setCgiRespoHeader(std::vector<std::string>&v);
        void setCgiRespoBody(std::vector<std::string> &v);
};

#endif

